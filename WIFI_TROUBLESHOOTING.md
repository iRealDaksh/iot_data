# WiFi Troubleshooting Guide

## 🚨 Common ESP32 WiFi Issues

### Issue 1: "OnePlus 11R 5G" Network Problems

**Problem**: The network name suggests it's a 5GHz network, but ESP32 only supports 2.4GHz.

**Solutions**:
1. **Check if your phone has dual-band WiFi**:
   - Look for "OnePlus 11R 5G" (5GHz) and "OnePlus 11R" (2.4GHz)
   - Use the 2.4GHz version for ESP32

2. **Create a 2.4GHz hotspot**:
   - Go to phone settings → Hotspot & Tethering
   - Look for "Band" or "Frequency" settings
   - Set to 2.4GHz only

3. **Use a different WiFi network**:
   - Connect to a regular WiFi router (usually 2.4GHz)
   - Update ESP32 code with new credentials

### Issue 2: WiFi Status Codes

When you see status numbers in Serial Monitor:
- **Status 0**: WL_IDLE_STATUS - WiFi is initializing
- **Status 1**: WL_NO_SSID_AVAIL - Network not found
- **Status 3**: WL_CONNECTED - Successfully connected
- **Status 4**: WL_CONNECT_FAILED - Connection failed
- **Status 6**: WL_DISCONNECTED - Lost connection

### Issue 3: Connection Timeout

**Symptoms**: ESP32 keeps trying to connect but never succeeds

**Solutions**:
1. **Move ESP32 closer to WiFi source**
2. **Check password is exactly correct** (case-sensitive)
3. **Restart the WiFi hotspot/router**
4. **Try a different network**

## 🔧 Quick Fixes

### Fix 1: Use Mobile Hotspot (Recommended)
1. **On your phone**: Settings → Hotspot & Tethering → Mobile Hotspot
2. **Set to 2.4GHz** if possible
3. **Note the network name and password**
4. **Update ESP32 code** with new credentials
5. **Connect your laptop to the same hotspot**

### Fix 2: Use Home WiFi
1. **Connect laptop to home WiFi**
2. **Update ESP32 code** with home WiFi credentials
3. **Make sure it's 2.4GHz network**

### Fix 3: Test with Simple Network
1. **Create a simple hotspot** with name like "Test123"
2. **Use simple password** like "12345678"
3. **Update ESP32 code** and test

## 📱 Phone Hotspot Settings

### For OnePlus/Android:
1. **Settings** → **Wi-Fi & Internet** → **Hotspot & Tethering**
2. **Mobile Hotspot** → **Configure**
3. **Network name**: Change to something simple like "ESP32_Test"
4. **Password**: Use simple password like "12345678"
5. **Band**: Select 2.4GHz if available

### For iPhone:
1. **Settings** → **Personal Hotspot**
2. **Wi-Fi Password**: Set simple password
3. **Note**: iPhone hotspots are usually 2.4GHz by default

## 🧪 Testing Steps

1. **Upload updated ESP32 code** with debugging
2. **Open Serial Monitor** (115200 baud)
3. **Look for these messages**:
   ```
   === WiFi Connection Debug ===
   Attempting to connect to: [Network Name]
   Password length: [Number]
   Connecting to WiFi.....
   Status: [Number]
   ✅ WiFi connected successfully!
   IP address: [IP Address]
   ```

4. **If connection fails**, you'll see:
   ```
   ❌ WiFi connection failed!
   Final status: [Number]
   Possible issues:
   - Network is 5GHz (ESP32 only supports 2.4GHz)
   - Wrong password
   - Network not in range
   - Network has device limits
   ```

## 🆘 Still Not Working?

1. **Try a different phone/device** for hotspot
2. **Use a friend's WiFi network**
3. **Check ESP32 hardware** (try different ESP32 board)
4. **Update ESP32 libraries** in Arduino IDE
5. **Try different WiFi credentials** in the code

## 📋 Network Requirements Checklist

- [ ] Network is 2.4GHz (not 5GHz)
- [ ] Password is exactly correct (case-sensitive)
- [ ] ESP32 is within WiFi range
- [ ] Network allows new devices
- [ ] No device limit reached
- [ ] Network security is WPA/WPA2 (not WEP)
- [ ] All devices on same network

