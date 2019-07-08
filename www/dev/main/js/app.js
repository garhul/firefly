function _id(id) {
  return document.getElementById(id);
}

const COLOR_WHEEL = (function() {
  var service = () => null;
  var state = {h:0, l:25, s:255};

  function onlChange(ev) {
    state.l = parseInt(ev.target.value);
    service(state);
  }

  function onHchange(ev) {

    state.h = parseInt(ev.target.getAttribute('h'));
    console.log(state);
    service(state);
  }

  function onSchange(ev) {
    state.s = parseInt(ev.target.value);
    service(state);
  }

  function render() {
    const relation = 1/1.411;
    for (var i=0; i < 90; i++) {
  			var color = document.createElement("span");
  			color.setAttribute("id", "d" + i );
  			color.style.backgroundColor = "hsl(" + i * 4 + ", 100%, 50%)";
  			color.style.transform = "rotate(" + i * 4 + "deg)";
        color.setAttribute("h", i * 4 * relation);

        color.addEventListener('touchmove',onHchange);
        color.addEventListener('mousemove',onHchange);
        _id('l').value = state.l;
        _id('s').value = state.s;
        _id('l').addEventListener('change',onlChange);
        _id('s').addEventListener('change',onSchange);
        _id('cw').appendChild(color)
  	};
  }

  function init(sv) {
    service = sv;
  }

  return {
    init,
    getState: () => this.state,
    render: render,
  }
})();

(function () {
  var ws = null;
  var tabs = 3;

  function startWs() {
    ws = new WebSocket('ws://192.168.0.36:1984');
    ws.binaryType = 'arraybuffer';

    _id('overlay').style.visibility = 'visible';
    _id('conn_err').style.visibility = 'visible';

    ws.addEventListener('message', function (event) {
        console.log('Message from server', event.data);
    });

    ws.addEventListener('error', function (event) {
        console.log('error', event.data);
        _id('overlay').style.visibility = 'visible';
        _id('conn_err').style.visibility = 'visible';
    });

    ws.addEventListener('open', function (event) {
      _id('overlay').style.visibility = 'hidden';
      _id('conn_err').style.visibility = 'hidden';
    });

    ws.addEventListener('close', function (event) {
      _id('overlay').style.visibility = 'visible';
      _id('conn_err').style.visibility = 'visible';
    });
  }

  function bindButtons() {
    for (var i = 0;i < _id('btns').children.length; i++) {
      var btn = _id('btns').children[i];
      btn.addEventListener('mouseup',(ev) => {
        console.log(ev.target.getAttribute('data-cmd'));
        var payload = new Uint8Array(3);
        payload[0] = 0x07;
        payload[1] = parseInt(ev.target.getAttribute('data-cmd'));
        payload[2] = 0x00;
        send(payload);
      });
    }

    //tabs bindButtons
    for (var i = 0; i < tabs; i++) {
      _id('tab' + i).addEventListener('click', function(ev) {
        showTab(ev.currentTarget.id + '_view');
      });
    }
    // uv button
    _id('uv').addEventListener('click', function(ev) {
      var payload = new Uint8Array(3);

      payload[0] = 0x08;
      payload[1] = 0x00;
      payload[2] = 0x00;

      send(payload);
    });
    // uv button
    _id('uv2').addEventListener('click', function(ev) {
      var payload = new Uint8Array(3);

      payload[0] = 0x09;
      payload[1] = 0x00;
      payload[2] = 0x00;

      send(payload);
    });

    // off button
    _id('off').addEventListener('click', function(ev) {
      var payload = new Uint8Array(3);

      payload[0] = 0x03;
      payload[1] = 0x00;
      payload[2] = 0x00;

      send(payload);
    });

    _id('br').addEventListener('change',function(ev) {
      var val = parseInt(ev.target.value);
      var payload = new Uint8Array(3);

      payload[0] = 0x05;
      payload[1] = val;
      payload[2] = 0x00;

      send(payload);
    });
  }

  var in_use = false;
  function send(payload) {
    if (in_use) {
      return;
    }

    in_use = true;
    if (ws.readyState === ws.OPEN) {
      console.log(payload);
      ws.send(payload);
    } else {
      console.error('unable to communicate with socket');
    }

    setTimeout(function () {
      in_use = false;
    }, 100);
  }

  function showTab(tab) {
    //hide other tabs
    for (var i=0; i < tabs; i++){
      _id('tab' + i + '_view').style.visibility = 'hidden';
    }
    //show tab
    _id(tab).style.visibility = 'visible';
  }

  function start() {
    startWs();
    COLOR_WHEEL.init(function(data) {
      var payload = new Uint8Array(4);
      payload[0] = 0x08;
      payload[1] = parseInt(data.h);
      payload[2] = parseInt(data.s);
      payload[3] = parseInt(data.l);
      send(payload);
    });

    COLOR_WHEEL.render();
    showTab('tab0_view');
  };

  bindButtons();
  start();

})();
