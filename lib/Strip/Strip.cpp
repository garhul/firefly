#include <NeoPixelBus.h>
#include <Strip.h>

//I tried but have no clue on how to properly extend this
NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart800KbpsMethod> bus(STRIP_SIZE, D10);

Strip::Strip(){
  bus.Begin();
  rel_unit_size = 1.0 / STRIP_SIZE;
  rel_unit_byte =  1.0 / 255;
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

void Strip::_eff_1() {
  byte n = 0;
  for (n = 0; n < STRIP_SIZE -1; n += 2) {
    if (oldh_list[n] > 10) {
      l_list[n]--;
    } else {
      l_list[n]++;
    }
    bus.SetPixelColor(n, RgbColor(HslColor( h_list[n], 1, rel_unit_byte * l_list[n])));
  }
}

void Strip::_randomize() {

};
