# IoT Sensor Dashboard

A complete IoT monitoring system using ESP32 with MPU6050, MAX30105, and GPS sensors, displaying real-time data on a web dashboard.

## 🚀 Features

- **Real-time sensor monitoring** with live updates
- **MPU6050**: 3-axis accelerometer and gyroscope data
- **MAX30105**: Heart rate and SpO2 monitoring
- **GPS**: Location tracking with Chennai fallback coordinates
- **Beautiful web dashboard** with charts and visualizations
- **WebSocket communication** for real-time updates

## 📋 Hardware Requirements

- ESP32 Dev Module (WiFi/Bluetooth)
- MPU6050 (3-axis accelerometer & gyroscope)
- MAX30105 (Heart rate & SpO2 sensor)
- GPS Module (NEO-6M or similar)
- Jumper wires
- Breadboard

## 🔧 Software Requirements

- Arduino IDE with ESP32 board support
- Node.js (v14 or higher)
- Modern web browser

## 📦 Installation & Setup

### 1. Install Node.js Dependencies

```bash
npm install
```

### 2. Start the Web Server

```bash
npm start
```

The dashboard will be available at: `http://localhost:3000`

### 3. Configure ESP32 Code

1. Open `esp32_iot_sensor.ino` in Arduino IDE
2. Update WiFi credentials:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
3. Update server URL with your computer's IP address:
   ```cpp
   const char* serverURL = "http://YOUR_COMPUTER_IP:3000/api/sensor-data";
   ```

### 4. Install Required Arduino Libraries

Install these libraries through Arduino IDE Library Manager:

- `Adafruit MPU6050` by Adafruit
- `Adafruit Sensor` by Adafruit
- `MAX30105lib` by SparkFun
- `TinyGPSPlus` by Mikal Hart
- `ArduinoJson` by Benoit Blanchon

### 5. Hardware Connections

#### MPU6050 to ESP32:
- VCC → 3.3V
- GND → GND
- SCL → GPIO 22
- SDA → GPIO 21

#### MAX30105 to ESP32:
- VCC → 3.3V
- GND → GND
- SCL → GPIO 22
- SDA → GPIO 21

#### GPS Module to ESP32:
- VCC → 3.3V
- GND → GND
- TX → GPIO 16 (RX2)
- RX → GPIO 17 (TX2)

## 🎯 Usage

1. **Start the web server** on your computer
2. **Upload the ESP32 code** to your device
3. **Open the dashboard** in your browser at `http://localhost:3000`
4. **Monitor real-time data** from all sensors

## 📊 Dashboard Features

### Real-time Monitoring
- **Heart Rate & SpO2**: Live pulse monitoring with visual indicators
- **Accelerometer**: 3-axis motion data with live charts
- **Gyroscope**: 3-axis rotation data with live charts
- **Temperature**: Environmental temperature monitoring
- **GPS Location**: Current position with satellite count
- **Raw Sensor Data**: IR and Red values from MAX30105

### Visual Elements
- **Live Charts**: Real-time data visualization
- **Status Indicators**: Connection and GPS status
- **Responsive Design**: Works on desktop and mobile
- **Beautiful UI**: Modern gradient design with animations

## 🔧 Configuration

### WiFi Setup
Update the WiFi credentials in the ESP32 code:
```cpp
const char* ssid = "YourWiFiName";
const char* password = "YourWiFiPassword";
```

### Server IP Configuration
Find your computer's IP address and update the ESP32 code:
```cpp
const char* serverURL = "http://192.168.1.XXX:3000/api/sensor-data";
```

### GPS Fallback
The system automatically uses Chennai coordinates (13.0827, 80.2707) when GPS signal is not available.

## 🐛 Troubleshooting

### ESP32 Not Connecting to WiFi
- Check WiFi credentials
- Ensure WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)
- Check signal strength

### No Data on Dashboard
- Verify server is running on correct port
- Check ESP32 serial monitor for connection status
- Ensure both devices are on same network

### GPS Not Working
- Check GPS module connections
- Ensure GPS module has clear sky view
- Wait for GPS fix (can take several minutes)

### Sensor Reading Issues
- Verify I2C connections (SDA/SCL)
- Check power supply (3.3V)
- Ensure proper library installation

## 📈 Data Format

The system sends JSON data every 2 seconds:

```json
{
  "accelerometer": {
    "x": 0.12,
    "y": -0.05,
    "z": 9.81
  },
  "gyroscope": {
    "x": 0.01,
    "y": 0.02,
    "z": -0.01
  },
  "temperature": 25.3,
  "heart_rate": 72,
  "spo2": 98,
  "gps": {
    "latitude": 13.0827,
    "longitude": 80.2707,
    "satellites": 8,
    "status": "valid"
  },
  "timestamp": 1234567890,
  "device_id": "ESP32_001"
}
```

## 🔄 API Endpoints

- `GET /` - Main dashboard
- `POST /api/sensor-data` - Receive sensor data from ESP32
- `GET /api/sensor-data` - Get latest sensor data
- `GET /api/historical-data` - Get historical data (last 50 readings)

## 📝 License

MIT License - feel free to use and modify for your projects!

## 🤝 Contributing

Contributions are welcome! Please feel free to submit issues and enhancement requests.

---

**Happy IoT Monitoring! 🚀**

