const WebSocket = require('ws');
const fs = require('fs');
const path = require('path');

const server = new WebSocket.Server({ port: 8765 });

server.on('connection', (socket) => {
  console.log('Client connected');

  socket.on('message', (message) => {
    const data = JSON.parse(message);
    if (data.type === 'send_image') {
      const imageData = Buffer.from(data.image_data, 'base64');
      const filePath = path.join(__dirname, 'images', `${Date.now()}.jpeg`);
      fs.writeFile(filePath, imageData, (error) => {
        if (error) {
          console.error(`Error writing file: ${error}`);
        } else {
          console.log(`Image saved as: ${filePath}`);
        }
      });
      const response = {
        type: 'image_received'
      };
      socket.send(JSON.stringify(response));
    }
  });

  socket.on('close', () => {
    console.log('Client disconnected');
  });
});

console.log('WebSocket server listening on port 8765');