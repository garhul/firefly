function _id(id) {
  return document.getElementById(id);
}

const COLOR_WHEEL = function(sv) {
  var service = sv;
  var state = {h:0, l:50, s:100};

  function onlChange(ev) {
    state.l = parseInt(ev.target.value);
    service(state);
  }

  function onHchange(ev) {
    state.h = parseInt(ev.target.getAttribute('h'));
    service(state);
  }

  function onSchange(ev) {
    state.s = parseInt(ev.target.value);
    service(state);
  }

  function render() {
    for (var i=0; i<360; i++) {
  			var color = document.createElement("span");
  			color.setAttribute("id", "d" + i);
  			color.style.backgroundColor = "hsl(" + i + ", 100%, 50%)";
  			//color.style.msTransform = "rotate(" + i + "deg)";
  			//color.style.webkitTransform = "rotate(" + i + "deg)";
  			//color.style.MozTransform = "rotate(" + i + "deg)";
  			//color.style.OTransform = "rotate(" + i + "deg)";
  			color.style.transform = "rotate(-" + i + "deg)";
        color.setAttribute("h", i);

        color.addEventListener('touchmove',onHchange);
        color.addEventListener('mousemove',onHchange);

        _id('l').addEventListener('change',onlChange);
        _id('s').addEventListener('change',onSchange);
        _id('cw').appendChild(color)
  	};
  }

  return {
    render
  }
};

(function () {
  var ws = null;
  var tabs = 3;

  function startWs() {
    ws = new WebSocket('ws://localhost:8080');
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
    const cw = COLOR_WHEEL(function(data) {
      var payload = new Uint8Array(4);
      payload[0] = 0x08;
      payload[1] = parseInt(data.h);
      payload[2] = parseInt(data.s);
      payload[3] = parseInt(data.l);
      send(payload);
    });
    cw.render();
    showTab('tab0_view');
  };

  bindButtons();
  start();

})();
