'use strict';

const WebSocket = require('ws');
const wss = new WebSocket.Server({ port: 8080 });

wss.on('connection', function connection(ws) {
  ws.on('message', function incoming(message) {
    console.log('received:' + Array.apply([], message).join(","));
    // console.log('received: %h', message);
  });

  ws.send('ok');
});
