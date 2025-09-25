# IoT Sensor Dashboard Setup Guide

## 🎉 NEW: Automatic Server Discovery!

**Great news! The project now uses automatic server discovery (mDNS), so you don't need to change IP addresses when sharing the project!**

The ESP32 will automatically find the server on the network. If automatic discovery fails, it will fall back to a configured IP address.

## Quick Setup Steps

### 1. Install Dependencies
```bash
npm install
```

### 2. Start the Server
```bash
npm start
```
The server will automatically advertise itself on the network using mDNS.

### 3. Upload ESP32 Code
1. Open `esp32_iot_sensor/esp32_iot_sensor.ino` in Arduino IDE
2. **No need to change IP addresses!** The ESP32 will automatically find the server
3. Upload the code to your ESP32

### 4. Access the Dashboard
Open your browser and go to: `http://localhost:3002`

**That's it! The ESP32 will automatically connect to your server.**

## 🔧 Troubleshooting

### Dashboard Opens But No Data Received

1. **Check ESP32 Serial Monitor**: Look for these messages:
   - `✅ Server discovered via mDNS!` - Automatic discovery worked
   - `⚠️ mDNS discovery failed, using fallback IP` - Using backup method
   - `✅ Data sent successfully via mDNS/fallback IP` - Connection successful

2. **If mDNS fails**: The ESP32 will automatically try the fallback IP address
3. **Check Network**: Ensure all devices are on the same WiFi network
4. **Firewall**: Make sure Windows Firewall allows Node.js on port 3002

### ESP32 Can't Connect to WiFi
1. Update WiFi credentials in the ESP32 code:
   ```cpp
   const char* ssid = "YOUR_WIFI_NAME";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```

### Server Won't Start
1. Make sure port 3002 is not being used by another application
2. Try a different port by changing `PORT` in `server.js`

## 📱 For Multiple Users

**Now it's super easy to share!**

### Option 1: Individual Servers (Recommended)
1. **Each person just needs to**:
   - Run `npm install` to install dependencies
   - Start their own server with `npm start`
   - Upload the ESP32 code (no IP changes needed!)
   - Access dashboard at `http://localhost:3002`

2. **ESP32 will automatically connect to whichever server is running**

### Option 2: Shared Server
1. **One person runs the server**: `npm start`
2. **Others access the dashboard**: `http://SERVER_IP:3002`
3. **ESP32 connects to the running server automatically**

## 🔄 Network Setup

### Option 1: Individual Servers (Recommended for Development)
- Each person runs their own server
- Each person updates ESP32 to connect to their own computer
- Good for testing and development

### Option 2: Shared Server (For Production)
- One person runs the server
- Everyone else accesses the dashboard via the server's IP
- ESP32 connects only to the server computer
- Others access: `http://SERVER_COMPUTER_IP:3002`

## 📋 File Structure
```
project/
├── esp32_iot_sensor/
│   └── esp32_iot_sensor.ino    # ESP32 code (UPDATE IP HERE!)
├── public/
│   └── index.html              # Dashboard UI
├── server.js                   # Node.js server
├── package.json                # Dependencies
├── get_ip.js                   # IP detection helper
└── SETUP_GUIDE.md             # This file
```

## 🆘 Still Having Issues?

1. **Check ESP32 Serial Monitor** for error messages
2. **Verify all devices are on the same WiFi network**
3. **Test server connectivity**: Try accessing `http://YOUR_IP:3002` from another device
4. **Check firewall settings** on your computer
5. **Restart everything**: ESP32, server, and browser

## 📞 Quick Commands

```bash
# Get your IP address
node get_ip.js

# Install dependencies
npm install

# Start server
npm start

# Start with auto-restart (development)
npm run dev
```
