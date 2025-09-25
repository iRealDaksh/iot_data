#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "MAX30105.h"
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi credentials - UPDATE THESE
const char* ssid = "OnePlus 11R 5G";
const char* password = "1223334444";

// Web server details - SIMPLIFIED APPROACH
// Update this IP address to your computer's IP when sharing the project
const char* serverIP = "192.168.56.1";  // Your computer's IP address
const int serverPort = 3002;
String serverURL = "http://" + String(serverIP) + ":" + String(serverPort) + "/api/sensor-data";

// Sensor objects
Adafruit_MPU6050 mpu;
MAX30105 particleSensor;
TinyGPSPlus gps;
HardwareSerial SerialGPS(2); // UART2 for GPS (RX=16, TX=17)

// Chennai coordinates as fallback
const double CHENNAI_LAT = 13.0827;
const double CHENNAI_LNG = 80.2707;

// Heart rate and SpO2 calculation variables
const int RATE_SIZE = 8;
int rates[RATE_SIZE];
int rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute;
int beatAvg;
float irValue;
float redValue;
float spo2;

// Improved heart rate detection variables
const int BUFFER_SIZE = 100;
float irBuffer[BUFFER_SIZE];
float redBuffer[BUFFER_SIZE];
int bufferIndex = 0;
bool bufferFull = false;

// SpO2 calculation variables
float redAC, irAC;
float redDC, irDC;
float ratio;

// Data sending interval
unsigned long lastDataSent = 0;
const unsigned long dataInterval = 2000; // Send data every 2 seconds

// Connection status
bool serverConnected = false;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  delay(2000);
  
  Serial.println("=== IoT Sensor System Starting ===");

  // Initialize MPU6050
  if (!mpu.begin(0x68)) {
    Serial.println("MPU6050 NOT FOUND!");
  } else {
    Serial.println("MPU6050 ready");
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  }

  // Initialize MAX30105
  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX30102 NOT FOUND!");
  } else {
    Serial.println("MAX30102 ready");
    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0x0A);
    particleSensor.setPulseAmplitudeIR(0x0A);
  }

  // Initialize GPS
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("GPS started, waiting for fix...");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Test server connection
  Serial.println("Testing connection to IoT Dashboard server...");
  testServerConnection();
}

void loop() {
  // Read sensor data
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  // Read heart rate and SpO2
  readHeartRateAndSpO2();
  
  // Read GPS data
  readGPSData();
  
  // Send data to server every 2 seconds
  if (millis() - lastDataSent >= dataInterval) {
    sendDataToServer(a, g);
    lastDataSent = millis();
  }
  
  delay(50); // Faster sampling for better heart rate detection
}

void readHeartRateAndSpO2() {
  irValue = particleSensor.getIR();
  redValue = particleSensor.getRed();
  
  // Store values in circular buffer
  irBuffer[bufferIndex] = irValue;
  redBuffer[bufferIndex] = redValue;
  bufferIndex++;
  if (bufferIndex >= BUFFER_SIZE) {
    bufferIndex = 0;
    bufferFull = true;
  }
  
  // Only calculate if we have enough data
  if (bufferFull) {
    calculateHeartRate();
    calculateSpO2();
  }
}

void calculateHeartRate() {
  // Find peaks in IR signal for heart rate detection
  static float lastIrValue = 0;
  static unsigned long lastPeakTime = 0;
  static bool rising = false;
  
  float currentIr = irBuffer[(bufferIndex - 1 + BUFFER_SIZE) % BUFFER_SIZE];
  
  // Detect rising edge
  if (currentIr > lastIrValue && !rising) {
    rising = true;
  }
  // Detect falling edge (peak)
  else if (currentIr < lastIrValue && rising) {
    rising = false;
    unsigned long currentTime = millis();
    
    if (lastPeakTime > 0) {
      long delta = currentTime - lastPeakTime;
      
      // Calculate BPM (beats per minute)
      if (delta > 300 && delta < 2000) { // Valid heart rate range: 30-200 BPM
        beatsPerMinute = 60000.0 / delta;
        
        if (beatsPerMinute >= 30 && beatsPerMinute <= 200) {
          rates[rateSpot++] = (int)beatsPerMinute;
          rateSpot %= RATE_SIZE;
          
          // Calculate average
          beatAvg = 0;
          int validReadings = 0;
          for (int x = 0; x < RATE_SIZE; x++) {
            if (rates[x] > 0) {
              beatAvg += rates[x];
              validReadings++;
            }
          }
          if (validReadings > 0) {
            beatAvg /= validReadings;
          }
        }
      }
    }
    lastPeakTime = currentTime;
  }
  
  lastIrValue = currentIr;
}

void calculateSpO2() {
  // Calculate DC and AC components for SpO2
  float irSum = 0, redSum = 0;
  float irMin = irBuffer[0], irMax = irBuffer[0];
  float redMin = redBuffer[0], redMax = redBuffer[0];
  
  // Find min/max and calculate DC
  for (int i = 0; i < BUFFER_SIZE; i++) {
    irSum += irBuffer[i];
    redSum += redBuffer[i];
    
    if (irBuffer[i] < irMin) irMin = irBuffer[i];
    if (irBuffer[i] > irMax) irMax = irBuffer[i];
    if (redBuffer[i] < redMin) redMin = redBuffer[i];
    if (redBuffer[i] > redMax) redMax = redBuffer[i];
  }
  
  // DC components (average)
  irDC = irSum / BUFFER_SIZE;
  redDC = redSum / BUFFER_SIZE;
  
  // AC components (peak-to-peak)
  irAC = irMax - irMin;
  redAC = redMax - redMin;
  
  // Calculate ratio for SpO2
  if (irDC > 0 && redDC > 0 && irAC > 0) {
    ratio = (redAC / redDC) / (irAC / irDC);
    
    // SpO2 calculation using empirical formula
    spo2 = 110 - (25 * ratio);
    
    // Clamp values to realistic range
    if (spo2 < 70) spo2 = 70;
    if (spo2 > 100) spo2 = 100;
  } else {
    spo2 = 98; // Default value when calculation is not possible
  }
}


void readGPSData() {
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
  }
}

void testServerConnection() {
  Serial.print("Connecting to server at: ");
  Serial.println(serverURL);
  
  HTTPClient http;
  http.begin(serverURL.c_str());
  http.setTimeout(5000); // 5 second timeout
  
  int httpResponseCode = http.GET();
  
  if (httpResponseCode > 0) {
    Serial.println("✅ Server connection successful!");
    Serial.print("Response code: ");
    Serial.println(httpResponseCode);
    serverConnected = true;
  } else {
    Serial.println("❌ Server connection failed!");
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    serverConnected = false;
  }
  
  http.end();
}

void sendDataToServer(sensors_event_t a, sensors_event_t g) {
  if (WiFi.status() == WL_CONNECTED && serverConnected) {
    HTTPClient http;
    http.begin(serverURL.c_str());
    http.addHeader("Content-Type", "application/json");
    
    // Create JSON payload
    DynamicJsonDocument doc(1024);
    
    // MPU6050 data
    doc["accelerometer"]["x"] = a.acceleration.x;
    doc["accelerometer"]["y"] = a.acceleration.y;
    doc["accelerometer"]["z"] = a.acceleration.z;
    doc["gyroscope"]["x"] = g.gyro.x;
    doc["gyroscope"]["y"] = g.gyro.y;
    doc["gyroscope"]["z"] = g.gyro.z;
    
    // Heart rate and SpO2 data
    doc["heart_rate"] = beatAvg;
    doc["spo2"] = spo2;
    doc["ir_value"] = irValue;
    doc["red_value"] = redValue;
    doc["ir_dc"] = irDC;
    doc["red_dc"] = redDC;
    doc["ir_ac"] = irAC;
    doc["red_ac"] = redAC;
    doc["ratio"] = ratio;
    
    // GPS data
    if (gps.location.isValid()) {
      doc["gps"]["latitude"] = gps.location.lat();
      doc["gps"]["longitude"] = gps.location.lng();
      doc["gps"]["satellites"] = gps.satellites.value();
      doc["gps"]["status"] = "valid";
    } else {
      doc["gps"]["latitude"] = CHENNAI_LAT;
      doc["gps"]["longitude"] = CHENNAI_LNG;
      doc["gps"]["satellites"] = 0;
      doc["gps"]["status"] = "fallback_chennai";
    }
    
    doc["timestamp"] = millis();
    doc["device_id"] = "ESP32_001";
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    int httpResponseCode = http.POST(jsonString);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("✅ Data sent successfully! Response: " + response);
    } else {
      Serial.println("❌ Error sending data. HTTP Code: " + String(httpResponseCode));
      serverConnected = false; // Mark as disconnected for retry
    }
    
    http.end();
  } else {
    Serial.println("WiFi not connected!");
  }
}
