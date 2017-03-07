#include <NeoPixelBus.h>
#include <Strip.h>


//I tried but have no clue on how to properly extend this
NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart800KbpsMethod> bus(STRIP_SIZE, D10);

Strip::Strip(){
  bus.Begin();
  rel_unit_size = 1.0 / STRIP_SIZE;
  rel_unit_byte =  1.0 / 255;
  frame_index = 0;
}

void Strip::fillRGB(uint8_t r, uint8_t g, uint8_t b){
  bus.ClearTo(RgbColor(r, g, b));
  bus.Show();
}

void Strip::fillHSL(uint8_t h, uint8_t s, uint8_t l){
  bus.ClearTo(RgbColor(HslColor((float) rel_unit_byte *  h, (float) rel_unit_byte * s, (float) rel_unit_byte * l)));
  bus.Show();
}

void Strip::setMaxBrightness(int b){
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

  HslColor color = HslColor((float) rel_unit_byte *  h, (float) rel_unit_byte * s, (float) rel_unit_byte * l);
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
  byte pixel = 0;
  for (pixel; pixel < STRIP_SIZE ; pixel++) {
    h_list[pixel] =   random(255);
    s_list[pixel] =   random(120);
    l_list[pixel] = 255;
    oldh_list[pixel] = random(120);
  }
}

void Strip::_eff_1() {

  if (frame_index % 300 == 0) {
    _randomize();
  }

  byte n = 0;
  for (n = 0; n < STRIP_SIZE -1; n += 2) {
    if (oldh_list[n] > 10) {
      l_list[n]--;
    } else {
      l_list[n]++;
    }
    bus.SetPixelColor(n, RgbColor(HslColor(h_list[n] * rel_unit_byte, 255 * rel_unit_byte, l_list[n])));
  }
  bus.Show();
}

void  Strip::_eff_2() {

}

void Strip::_eff_3() {

}


// void Strip::_eff_2() {
//   byte n = 0;
//   //
//   // for (n = 0; n < STRIP_SIZE; n+=2 ) {
//   //   if (n < STRIP_SIZE / 2) {
//   //
//   //   }
//   //
//   //   bus.SetPixelColor(n, RgbColor(HslColor( hue * rel_unit_byte, 1, rel_unit_byte )));
//   // }
// }
//

// void Strip::_eff_3(char hue) {
//   byte n = 0;
//   char mirrored_hue = hue + 128;
//   char l = 50;
//   static int row_a = 0;
//   static int row_b = 7;
//   //other hue should be 180 degrees apart
//   row_a = ++row_a % 4;
//   row_b = --row_b;
//   if (row_b == 3) row_b = 7;
//
//   for (n = 0; n < STRIP_SIZE; n+=2 ) {
//     // if (frame_index % 10 == 0) { //every 10 frames we adjust the light movement
//     //     //first row value
//     //     if (n < 16 * row_a) {
//     //       l = 50 + frame_index %;
//     //     } else if (n < 32) {
//     //       l = 55;
//     //     } else if (n < 48 ) {
//     //       l =
//     //     }
//     // }
//
//     if (n < STRIP_SIZE / 2) {
//       bus.SetPixelColor(n, RgbColor(HslColor( (float) mirrored_hue * rel_unit_byte, 1, rel_unit_byte )));
//     } else {
//       bus.SetPixelColor(n, RgbColor(HslColor( float) hue * rel_unit_byte, 1, rel_unit_byte )));
//     }
//
//   }
// }

void Strip::resetFrameCount() {
  frame_index = 0;
}

void Strip::nextFrame(char eff_index) {
  frame_index++;
  if (eff_index == EFFECT_RAINBOW){
    _eff_1();
  } else if (eff_index == EFFECT_TEST) {
    _eff_2();
  } else if (eff_index == EFFECT_WAVE) {
    _eff_3();
  }
}
