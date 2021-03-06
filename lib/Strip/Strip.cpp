#include <NeoPixelBus.h>
#include <Strip.h>

//I tried but have no clue on how to properly extend this
//btw this method only works for d4 pin
NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUartWs2813Method> bus(STRIP_SIZE);

Strip::Strip(){
  bus.Begin();
  _max_bright = 60;
  frame_index = 0;
}

void Strip::fillRGB(uint8_t r, uint8_t g, uint8_t b){
  bus.ClearTo(RgbColor(r, g, b));
  bus.Show();
}

void Strip::fillHSL(uint8_t h, uint8_t s, uint8_t l){
  bus.ClearTo(RgbColor(HslColor((float) REL_UNIT_BYTE *  h, (float) REL_UNIT_BYTE * s, (float) REL_UNIT_BYTE * l)));
  bus.Show();
}

void Strip::setMaxBrightness(byte b){
  _max_bright = b;
}

//cycle each led to test connections
void Strip::test(){
  bus.ClearTo(RgbColor(0,0,0));
  bus.Show();
  for (int i = 0; i < STRIP_SIZE; i++ ) {
    bus.SetPixelColor(i, RgbColor(255,0,0));
    bus.Show();
    delay(TEST_DELAY);
    bus.SetPixelColor(i, RgbColor(0,255,0));
    bus.Show();
    delay(TEST_DELAY);
    bus.SetPixelColor(i, RgbColor(0,0,255));
    bus.Show();
    delay(TEST_DELAY);
    bus.SetPixelColor(i, RgbColor(0,0,0));
    bus.Show();
    delay(TEST_DELAY);
  }
  //clear strip again
  bus.ClearTo(RgbColor(0,0,0));
  bus.Show();
};

void Strip::setRGBRange(byte r, byte g, byte b, int start, int end) {
  if (start < 0 || end > STRIP_SIZE)
    return;

  for (int i = start; i < end; i++ ) {
    bus.SetPixelColor(i , RgbColor(r,g,b));
  }

  bus.Show();
}

void Strip::setHSLRange(byte h, byte s, byte l, int start, int end) {
  if (start < 0 || end > STRIP_SIZE)
    return;

  HslColor color = HslColor((float) REL_UNIT_BYTE *  h, (float) REL_UNIT_BYTE * s, (float) REL_UNIT_BYTE * l);
  for (int i = start; i < end; i++ ) {
    bus.SetPixelColor(i , RgbColor(color));
  }

  bus.Show();
}

void Strip::clear() {
  bus.ClearTo(RgbColor(0,0,0));
  bus.Show();
}

void  Strip::_randomize() {
  byte px = 0;
  for (px = 0; px < STRIP_SIZE ; px++) {
    pixels[px].hue = byte(random(100));
    pixels[px].br = byte(random(_max_bright));
    pixels[px].sat = 255;
  }
}

void Strip::_eff_0() {
  byte n = 0;
  byte hue = 0;
  static byte hue_inc = 0;
  hue_inc++;
  for (n = 0; n < STRIP_SIZE ; n++ ) {
    if (n % 16  == 0){
      hue += 16;
    }

    pixels[n].hue = hue + hue_inc;
    pixels[n].sat = 255;
    pixels[n].br = _max_bright;
  }
}


void Strip::_eff_1() {
  static byte hue_inc = 0;

  if (frame_index % 2 == 0) {
    hue_inc++;
  }

  byte n = 0;
  for (n = 0; n < STRIP_SIZE ; n++ ) {
    pixels[n].hue = (n * 2)  + hue_inc;
    pixels[n].br = _max_bright;
    pixels[n].sat = 255;
  }
}

void Strip::_eff_2() {
  byte n = 0;
  static byte hue = random(255);

  if (frame_index % 10 == 0) {
    hue++;
  }

  for (n = 0; n < STRIP_SIZE ; n++ ) {
    if (n % 32 == 0) {
      hue += 64;
    }
    pixels[n].hue == hue;
    pixels[n].sat = 255;
    pixels[n].br = _max_bright;
  }
}

void Strip::_eff_3() {
  static byte hue = 0;

  if (frame_index % 4 == 0) {
    hue++;
  }

  byte n = 0;
  byte br = 0;

    for (n = 0; n < STRIP_SIZE; n++ ) {
    if (n < STRIP_SIZE / 2) {
      if (n % 16 == 0) {
        br +=32;
      }
      pixels[n].hue = hue + 128;
      pixels[n].br = br;
      pixels[n].sat = 255;
    } else {
      if (n % 16 == 0 && n % 64 != 0) {
        br -= 32;
      }
      pixels[n].hue = hue;
      pixels[n].br = br;
      pixels[n].sat = 255;
    }

  }
}

void Strip::_eff_4() {
  byte n = 0;
  static byte h = 0;

  for (n = 0; n < STRIP_SIZE ; n++) {
    if( n < (STRIP_SIZE / 2)) {
      pixels[n].hue = n * (255 / STRIP_SIZE) + h;
    } else {
      pixels[n].hue = (STRIP_SIZE - n -1) * (255 / STRIP_SIZE) +  h;
    }

    pixels[n].br = _max_bright;
    pixels[n].sat = 255;
  }

  if (frame_index % 5 == 0) {
    h++;
  }

}

void Strip::_eff_5() {
  byte n = 0;
  static byte h,b = 0;
  static int inc = -1;


  for (n = 0; n < STRIP_SIZE ; n++) {
    if( n < (STRIP_SIZE / 2)) {
      pixels[n].br = n * (b / STRIP_SIZE  ) ;
    } else {
      pixels[n].br = (STRIP_SIZE - n -1) * (b / STRIP_SIZE) ;
    }

    pixels[n].hue = n * (255 / STRIP_SIZE) + h;
    pixels[n].sat = 255;
  }

  if (frame_index % 2 == 0) {
    // h++;
    b+=inc;

    if (b == 0 || b == _max_bright)
        inc = inc * -1;
  }

}

void Strip::_eff_6() {
  static byte first_run = 1;
  static byte hue_inc = 0;
  static int dirs[STRIP_SIZE];

  if (first_run == 1) {
    for (int i = 0; i < STRIP_SIZE; i++) {
      pixels[i].br = i % 2 * _max_bright;
    }
    first_run = 0;
  }

  if (frame_index % 4 == 0) {
    hue_inc++;
  }

  for (int n = 0; n < STRIP_SIZE; n++ ) {
    //if (frame_index % 1 == 0) {
    if (pixels[n].br == _max_bright) { //we should start reducing
      dirs[n] = -1;
    } else if (pixels[n].br <= 20) {
      dirs[n] = 1;
    }
    pixels[n].br += dirs[n];
  //}

    pixels[n].hue = (n * 2)  + hue_inc;
    pixels[n].sat = 255;
  }
 }

void Strip::resetFrameCount() {
  frame_index = 0;
}

void Strip::nextFrame(char eff_index) {
  byte n = 0;
  if (eff_index == 0x00) {
    _eff_0();
  } else if (eff_index == 0x01) {
    _eff_1();
  } else if (eff_index == 0x02) {
    _eff_2();
  } else if (eff_index == 0x03) {
    _eff_3();
  } else if (eff_index == 0x04) {
    _eff_4();
  } else if (eff_index == 0x05) {
    _eff_5();
  } else if (eff_index == 0x06) {
    _eff_6();
  }

  for (n = 0; n < STRIP_SIZE; n++ ) {
    bus.SetPixelColor(n, RgbColor(
      HsbColor( pixels[n].hue * REL_UNIT_BYTE,
                pixels[n].sat * REL_UNIT_BYTE,
                pixels[n].br * REL_UNIT_BYTE)));
  }

  bus.Show();
  frame_index++;
}
