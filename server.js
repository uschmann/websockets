var WebSocketServer = require('ws').Server
  , wss = new WebSocketServer({ port: 8080 });

wss.on('connection', function connection(ws) {
	console.log('connected!');
	console.log(ws);
  ws.on('message', function incoming(message) {
    console.log('received: %s', message);

    ws.send(message.toUpperCase());
  });

  ws.on('close', function close() {
  	console.log('disconnected');
	});

});