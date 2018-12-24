
let h_center = 80;
let brn1 = 14;
let brn2 = 15;

function getNextFrame(frame_index) {
  let br = 30;
  let n = 0 ;
  let pixels = [];

  if (frame_index % 4 == 0) {
    brn1 = (brn1 == 0) ? 14 : brn1-1;
    brn2 = (brn2 == 30) ? 15 : brn2+1;
    //h_center++;
  }

  for (n = 0; n < 30; n++ ) {
    if (n < 15) {
      pixels[n] = {
        br: (n == brn1)? br + 30: br - ((15 - n) * 2),
        hue: ++h_center,
        sat: 255
      };
    } else {
      pixels[n] = {
        br : (n == brn2)? br + 30: br - ((n - 15) * 2),
        hue : --h_center,
        sat: 255,
      }
    }
  }

  let row = "";
  pixels.forEach((i) =>{
      row += ` ${i.br} `;
  });

  console.log(row);
}


for (let i = 0; i < 300; i++ ) {
  getNextFrame(i);
}
