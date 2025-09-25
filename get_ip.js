const os = require('os');

function getLocalIPAddress() {
  const interfaces = os.networkInterfaces();
  
  for (const name of Object.keys(interfaces)) {
    for (const interface of interfaces[name]) {
      // Skip internal (loopback) and non-IPv4 addresses
      if (interface.family === 'IPv4' && !interface.internal) {
        // Prefer WiFi interfaces, but return any valid IPv4 address
        if (name.toLowerCase().includes('wi-fi') || 
            name.toLowerCase().includes('wlan') || 
            name.toLowerCase().includes('wireless') ||
            name.toLowerCase().includes('ethernet')) {
          return interface.address;
        }
      }
    }
  }
  
  // If no preferred interface found, return the first valid IPv4 address
  for (const name of Object.keys(interfaces)) {
    for (const interface of interfaces[name]) {
      if (interface.family === 'IPv4' && !interface.internal) {
        return interface.address;
      }
    }
  }
  
  return '192.168.1.100'; // Fallback IP
}

const ipAddress = getLocalIPAddress();
console.log('='.repeat(50));
console.log('🌐 IoT Dashboard Setup Helper');
console.log('='.repeat(50));
console.log(`📍 Your computer's IP address: ${ipAddress}`);
console.log(`🔗 Dashboard URL: http://${ipAddress}:3002`);
console.log(`📡 API endpoint: http://${ipAddress}:3002/api/sensor-data`);
console.log('='.repeat(50));
console.log('📝 To update ESP32 code:');
console.log(`   1. Open esp32_iot_sensor/esp32_iot_sensor.ino`);
console.log(`   2. Find line 19: const char* serverIP = "10.60.39.9";`);
console.log(`   3. Replace "10.60.39.9" with "${ipAddress}"`);
console.log('='.repeat(50));
console.log('🚀 Then run: npm start');
console.log('='.repeat(50));