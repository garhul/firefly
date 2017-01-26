#include <Animations.h>

// ----------------- EFFECTS --------------------------//
//
//
//



//
// void  TableController::eff_2() {
//   byte n = 0;
//   byte dir = 1;
//   byte r=0;
//
//   for (n = 0; n < TABLE_SIZE; n++){
//     h_list[n]+= rel_unit;
//   }
//
//   for (n = 0; n < TABLE_SIZE - 1; n+=2) {
//     if (br_old[n] > 10){
//       br[n]--;
//     } else {
//       br[n]++;
//     }
//
//     leds[n] = leds[n+1] = CHSV(hues[n], 255 , br[n]);
//   }
//   FastLED.show();
//
// }
//
// void  TableController::randomize() {
//   byte pixel = 0;
//   for (pixel; pixel < TABLE_SIZE ; pixel++) {
//     h_list[pixel] = rel_unit * pixel;
//     l_list[pixel] = rel_unit * random8(0,120);
//     l_old_list[hue] = rel_unit * random8(0,120);
//   }
// }
//
// void  TableController::eff_3() {
//   byte n = 0;
//   byte dir = 1;
//   byte r=0;
//   for (dir = 0; dir <255; dir++) {
//     delay(15);
//     for (n = 0; n < NUM_LEDS - 1; n+=2) {
//       if (br_old[n] < 120) {
//         if (br[n] > 120) {
//           br_old[n] = 255;
//         }
//         br[n]++;
//       } else {
//         if (br[n] < 2) {
//           br_old[n] = 0;
//           hues[n] = random8();
//         }
//
//         br[n]--;
//       }
//       leds[n] = leds[n+1] = CHSV(hues[n], 255 , br[n]);
//     }
//     FastLED.show();
//   }
//
//   for (n = 0; n < NUM_LEDS -1; n++){
//     hues[n]+=16;
//   }
// }
//
// void  TableController::eff_4() {
//   byte n = 0;
//   byte dir = 1;
//   byte r=0;
//   for (dir = 0; dir <255; dir++) {
//     delay(15);
//     for (n = 0; n < NUM_LEDS - 1; n+=2) {
//       if (br_old[n] < 120) {
//         if (br[n] > 120) {
//           br_old[n] = 255;
//         }
//         br[n]++;
//       } else {
//         if (br[n] < 2) {
//           br_old[n] = 0;
//           hues[n] = random8();
//         }
//
//         br[n]--;
//       }
//       leds[n] = leds[n+1] = CHSV(hues[n], 255 , br[n]);
//     }
//     FastLED.show();
//   }
//
//   for (n = 0; n < NUM_LEDS -1; n++){
//     hues[n]+=16;
//     br[n]+=16;
//   }
// }
//
// void TableController::eff_5() {
//   byte n = 0;
//   byte dir = 1;
//   for (dir = 0; dir <255; dir++) {
//     delay(15);
//     for (n = 0; n < NUM_LEDS - 1; n+=2) {
//       leds[n] = leds[n+1] = CHSV(hues[n], 255 , 150);
//     }
//     FastLED.show();
//
//     for (n = 0; n < NUM_LEDS -1; n++){
//       hues[n]+=1;
//     }
//   }
// }
//
// void TableController::eff_6(int increment) {
//   int n = 0;
//   for (int i = 0; i < TABLE_SIZE; i+= 2 ) {
//     Strip.SetPixelColor(i, RgbColor(HslColor( rainbow_hue[n] , 1 , 0.5)));
//     rainbow_hue[n] += 1.0 / (float)increment;
//     if (rainbow_hue[n] > 1) rainbow_hue[n] = 0;
//     n++;
//   }
//   Strip.Show();
// }
//
// void TableController::effect_zone_rainbow(int inc_a, int inc_b, int inc_c, int inc_d) {
//   int n = 0;
//   for (int i = 0; i < TABLE_SIZE; i+= 2 ) {
//     Strip.SetPixelColor(i, RgbColor(HslColor( rainbow_hue[n] , 1 , 0.5)));
//     if ()
//     rainbow_hue[n] += 1.0 / (float)int_a;
//
//     if (rainbow_hue[n] > 1) rainbow_hue[n] = 0;
//     n++;
//   }
//   Strip.Show();
// }

// ---------------- EO EFFECTS ---------------------//
