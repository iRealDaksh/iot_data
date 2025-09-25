#include <WiFi.h>

// Simple WiFi test - change these to your network
const char* ssid = "ESP32_Test";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("=== ESP32 WiFi Test ===");
  Serial.print("Trying to connect to: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(1000);
    Serial.print(".");
    attempts++;
    
    if (attempts % 5 == 0) {
      Serial.println();
      Serial.print("Status: ");
      Serial.print(WiFi.status());
      Serial.print(" (Attempt ");
      Serial.print(attempts);
      Serial.println("/30)");
    }
  }
  
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("✅ SUCCESS! WiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal Strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("❌ FAILED! WiFi Connection Failed!");
    Serial.print("Final Status: ");
    Serial.println(WiFi.status());
    Serial.println();
    Serial.println("Status Codes:");
    Serial.println("0 = WL_IDLE_STATUS");
    Serial.println("1 = WL_NO_SSID_AVAIL");
    Serial.println("3 = WL_CONNECTED");
    Serial.println("4 = WL_CONNECT_FAILED");
    Serial.println("6 = WL_DISCONNECTED");
    Serial.println();
    Serial.println("SOLUTIONS:");
    Serial.println("1. Check if network is 2.4GHz (ESP32 doesn't support 5GHz)");
    Serial.println("2. Verify password is correct");
    Serial.println("3. Move closer to WiFi source");
    Serial.println("4. Try a different network");
  } 
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi still connected - IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("WiFi disconnected!");
  }
  delay(5000);
}
