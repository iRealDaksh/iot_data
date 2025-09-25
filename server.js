const express = require('express');
const cors = require('cors');
const http = require('http');
const socketIo = require('socket.io');
const path = require('path');

const app = express();
const server = http.createServer(app);
const io = socketIo(server, {
  cors: {
    origin: "*",
    methods: ["GET", "POST"]
  }
});

const PORT = process.env.PORT || 3002;

// Middleware
app.use(cors());
app.use(express.json());
app.use(express.static('public'));

// Store latest sensor data
let latestSensorData = {
  accelerometer: { x: 0, y: 0, z: 0 },
  gyroscope: { x: 0, y: 0, z: 0 },
  temperature: 0,
  heart_rate: 0,
  spo2: 0,
  ir_value: 0,
  red_value: 0,
  gps: {
    latitude: 13.0827,
    longitude: 80.2707,
    satellites: 0,
    status: "fallback_chennai"
  },
  timestamp: 0,
  device_id: "ESP32_001"
};

// Store historical data (last 100 readings)
let historicalData = [];

// API endpoint to receive sensor data from ESP32
app.post('/api/sensor-data', (req, res) => {
  try {
    const sensorData = req.body;
    
    // Validate required fields
    if (!sensorData.accelerometer || !sensorData.gyroscope || !sensorData.gps) {
      return res.status(400).json({ error: 'Missing required sensor data fields' });
    }
    
    // Update latest data
    latestSensorData = {
      ...sensorData,
      timestamp: Date.now()
    };
    
    // Add to historical data
    historicalData.push(latestSensorData);
    
    // Keep only last 100 readings
    if (historicalData.length > 100) {
      historicalData = historicalData.slice(-100);
    }
    
    // Emit real-time data to connected clients
    io.emit('sensorData', latestSensorData);
    
    console.log('Received sensor data:', {
      timestamp: new Date(latestSensorData.timestamp).toISOString(),
      heart_rate: latestSensorData.heart_rate,
      spo2: latestSensorData.spo2,
      gps_status: latestSensorData.gps.status
    });
    
    res.json({ 
      success: true, 
      message: 'Data received successfully',
      timestamp: latestSensorData.timestamp
    });
    
  } catch (error) {
    console.error('Error processing sensor data:', error);
    res.status(500).json({ error: 'Internal server error' });
  }
});

// API endpoint to get latest sensor data
app.get('/api/sensor-data', (req, res) => {
  res.json(latestSensorData);
});

// API endpoint to get historical data
app.get('/api/historical-data', (req, res) => {
  const limit = parseInt(req.query.limit) || 50;
  const limitedData = historicalData.slice(-limit);
  res.json(limitedData);
});

// Serve the main dashboard
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Socket.IO connection handling
io.on('connection', (socket) => {
  console.log('Client connected:', socket.id);
  
  // Send latest data immediately when client connects
  socket.emit('sensorData', latestSensorData);
  
  socket.on('disconnect', () => {
    console.log('Client disconnected:', socket.id);
  });
});

// Start server
server.listen(PORT, '0.0.0.0', () => {
  console.log(`🚀 IoT Sensor Dashboard Server running on port ${PORT}`);
  console.log(`📊 Dashboard available at: http://localhost:${PORT}`);
  console.log(`📡 API endpoint: http://localhost:${PORT}/api/sensor-data`);
  console.log(`🌐 Server listening on all network interfaces`);
  console.log(`📝 ESP32 should connect to: http://192.168.56.1:${PORT}/api/sensor-data`);
});

// Graceful shutdown
process.on('SIGINT', () => {
  console.log('\n🛑 Shutting down server...');
  server.close(() => {
    console.log('✅ Server closed');
    process.exit(0);
  });
});
