'use strict'

module.exports = function(sv) {
  var service = sv;
  var state = {h:0, l:0}

  function onStateChange(ev) {
    if (ev.type === "change")
      state.l = parseInt(ev.target.value);
    else
      state.h = parseInt(ev.target.getAttribute('h'));

    service(state);
  }

  function render() {
    for (var i=0; i<360; i++) {
  			var color = document.createElement("span");
  			color.setAttribute("id", "d" + i);
  			color.style.backgroundColor = "hsl(" + i + ", 100%, 50%)";
  			color.style.msTransform = "rotate(" + i + "deg)";
  			color.style.webkitTransform = "rotate(" + i + "deg)";
  			color.style.MozTransform = "rotate(" + i + "deg)";
  			color.style.OTransform = "rotate(" + i + "deg)";
  			color.style.transform = "rotate(" + i + "deg)";
        color.setAttribute("h", i);

        color.addEventListener('touchmove',onStateChange);
        color.addEventListener('mousemove',onStateChange);

        document.getElementById('h').addEventListener('change',onStateChange);
        document.getElementById('cw').appendChild(color)
  	};
  }

  return {
    render
  }
}
