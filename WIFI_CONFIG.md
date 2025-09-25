# WiFi Configuration Guide

## 🚨 CRITICAL: Update WiFi Credentials Before Uploading!

When you receive this project or want to use it on a different network, you **MUST** update the WiFi credentials in the ESP32 code.

## 📝 How to Update WiFi Credentials

### Step 1: Open ESP32 Code
1. Open `esp32_iot_sensor/esp32_iot_sensor.ino` in Arduino IDE

### Step 2: Find WiFi Settings
Look for these lines (around line 13-14):
```cpp
const char* ssid = "OnePlus 11R 5G";        // Change this
const char* password = "1223334444";        // Change this
```

### Step 3: Update Credentials
Replace with your WiFi details:
```cpp
const char* ssid = "YOUR_WIFI_NAME";        // Your WiFi network name
const char* password = "YOUR_WIFI_PASSWORD"; // Your WiFi password
```

### Step 4: Upload Code
Upload the updated code to your ESP32.

## 🔍 Common WiFi Issues

### Issue: "WiFi not connected"
**Solution**: Check that:
1. WiFi name and password are correct
2. WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)
3. WiFi network allows new devices to connect

### Issue: "Connection timeout"
**Solution**: 
1. Move ESP32 closer to WiFi router
2. Check if WiFi network has device limits
3. Try a different WiFi network

### Issue: "IP address not found"
**Solution**:
1. Make sure computer and ESP32 are on the same WiFi network
2. Update the server IP address in ESP32 code (line 18)

## 📋 Network Requirements

- **WiFi Type**: 2.4GHz (not 5GHz)
- **Security**: WPA/WPA2 (most common)
- **Range**: ESP32 should be within WiFi range
- **Same Network**: All devices must be on the same WiFi network

## 🆘 Still Having Issues?

1. **Check Serial Monitor** in Arduino IDE for error messages
2. **Try a mobile hotspot** as a test network
3. **Verify WiFi credentials** are exactly correct (case-sensitive)
4. **Restart ESP32** after uploading new code

## 📱 Quick Test

To test if WiFi is working:
1. Upload code with correct WiFi credentials
2. Open Serial Monitor (115200 baud)
3. Look for: `WiFi connected! IP address: xxx.xxx.xxx.xxx`
4. If you see this, WiFi is working correctly!

