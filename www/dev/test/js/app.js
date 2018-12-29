function _id(id) {
  return document.getElementById(id);
}
const ledCount = 28;
const max_bright = 60;
const framecount = _id('framecount');
let leds = [];
let pixels = [];

const init = () => {
  const bar = _id('bar');
  for (let i =0; i < ledCount; i++) {
    let l = document.createElement('div',{id: `led_${i}`});
    bar.appendChild(l);
    pixels.push(l);
    leds.push({h:0,s:0,l:0});
  }
};


const update = (index, values) => {
  const rel = 360 / 255;
  pixels[index].style.backgroundColor = `hsl(${values.h * rel}, ${values.s}%, ${values.l}%)`;
}

let h_center = 0;
let br_center = 0;
let br_center_dir = -1;

const playAnimation = (frame) => {
  if (frame == 0 ) {
    //set the base for the effect
    for (n = 0; n < ledCount; n ++) {
      leds[n].l = 50;
      leds[n].s = 100,
      leds[n].h = (n < 15) ? h_center + (15 - n) * 2: h_center - (n - 15) * 2;
    }
  }

  for (let i =0; i < ledCount; i++) {
    let l = Math.ceil((br_center + i) * (ledCount / max_bright));

    leds[i] = {
      h: h_center,
      s: 100,
      l: l
    };
    update(i, leds[i]);
}

  if (frame % 8 == 0) {
      h_center = ++h_center % 360 ;
  }
  if (frame % 2 == 0) {
    // brn1 = (brn1 == 0) ? 14 : brn1-1;
    // brn2 = (brn2 == 29) ? 15 : brn2+1;

    if (br_center == ledCount || br_center == 0) {
      br_center_dir = br_center_dir * -1
    }
    br_center = (br_center + br_center_dir);
  }
}

init();
let frame = 0;
const timer = setInterval(() => {
  playAnimation(frame);
  framecount.innerHTML = frame;
  frame++;
} , 30)
