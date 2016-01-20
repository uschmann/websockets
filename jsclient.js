var WebSocket = require('ws');
var ws = new WebSocket('ws://localhost:8080');
 
ws.on('message', function incoming(message) {
 	console.log('received: %s', message);
});

ws.on('open', function open() {
  ws.send('something');
});