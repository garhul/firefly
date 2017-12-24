'use strict'
(function () {
  var ws = null;
  function _id(id) {
    return document.getElementById(id);
  }

  function startWs(){
    ws = new WebSocket('ws://192.168.0.64:1984');
    ws.binaryType = 'arraybuffer';

    ws.addEventListener('message', function (event) {
        console.log('Message from server', event.data);
    });
  }

  function bindButtons() {
    for (var i = 0;i < _id('controls').children.length; i++) {
      var btn = _id('controls').children[i];
      btn.addEventListener('mouseup',(ev) => {
        console.log(ev.target.getAttribute('data-cmd'));
        send(parseInt(ev.target.getAttribute('data-cmd')))
      });
    }

    for(var i = 0; i < _id('tv').children.length ;i++){
      _id('tv').children[i].addEventListener('mouseup',(ev) =>{
        // if ()
        console.log(ev.target.getAttribute('class'));
        ev.target.setAttribute('data-selected' ,true);
        ev.target.setAttribute('class','group'+ i + ' group-selected');
      })
    }
  }

  function send(data) {
    var payload = new Uint8Array(3);
    payload[0] = 0x08; //fill command
    payload[1] = parseInt(data.h);
    payload[2] = parseInt(data.l);
    if (ws.readyState === ws.OPEN) {
      ws.send(payload);
    } else {
      console.error('unable to communicate with socket');
    }
  }

  function render() {
  //   for (var i =0; i< 32; i++){
  //     var led = document.createElement('div');
  //     led.setAttribute('class','led');
  //     led.addEventListener('mousedown',(ev) => {
  //       console.log(ev);
  //     });
  //
  //     if (i < 10) {
  //       _id('tv').children[0].append(led);
  //     } else if (i < 24) {
  //       led.setAttribute('class','vled');
  //       _id('tv').children[1].append(led);
  //     } else {
  //       _id('tv').children[2].append(led);
  //     }
  // }
}

  function updateState(state) {
    console.log("sending new state");
    console.log(state);
    send(state);
  }

  function showTab() {


  }

  function start() {
    startWs();
    const cw = require('./details')(send);
    cw.render();
  };

  start();

})();
