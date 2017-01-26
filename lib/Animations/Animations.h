#include <NeoPixelBus.h>

#define STRIP_SIZE 128
template<typename T_COLOR_FEATURE, typename T_METHOD> class Animations {
  public:
    Animations(const NeoPixelBus<T_COLOR_FEATURE, T_METHOD> &bus):
      Strip (bus),
      rel_unit_byte( 1.0 / 255),
      rel_unit_size(1.0 / STRIP_SIZE)
      {}
      // rel_unit_size = 1.0 / STRIP_SIZE;


    /* first effect, just animates brightness up and down from a random starting point */
    void eff_1() {
      byte n = 0;
      for (n = 0; n < STRIP_SIZE -1; n += 2) {
        if (oldh_list[n] > 10) {
          l_list[n]--;
        } else {
          l_list[n]++;
        }
        Strip.SetPixelColor(n, RgbColor(HslColor( h_list[n], 1, rel_unit_byte * l_list[n])));
      }
    }
    void eff_2();
    void eff_3();
    void eff_4();
    void eff_5();
    void eff_6(int incremnet); //hue rotation change
    void effect_fade();
    void effect_random_fade();

  private:
    NeoPixelBus<T_COLOR_FEATURE, T_METHOD> & Strip ;
    int max_brightness;
    float rel_unit_byte;
    float rel_unit_size;
    byte h_list[STRIP_SIZE];
    byte s_list[STRIP_SIZE];
    byte l_list[STRIP_SIZE];
    byte oldh_list[STRIP_SIZE];
};
