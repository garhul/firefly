#include <NeoPixelBus.h>

#define STRIP_SIZE 129 // 2 leds per slot -> 64 slots + first led as levelshift
#define TEST_DELAY 15

#define EFFECT_RAINBOW 0x00
#define EFFECT_FADE 0x01
#define EFFECT_TEST 0x02
#define EFFECT_WAVE 0x03

class Strip {
  public:
    Strip();
    void test();
    void setRGBRange(byte r, byte g, byte b, int start, int end);
    void setHSLRange(byte h, byte s, byte l, int start, int end);
    void clear();

    void fillRGB(uint8_t r, uint8_t g, uint8_t b);
    void fillHSL(uint8_t h, uint8_t s, uint8_t l);
    void setMaxBrightness(int b);
    void resetFrameCount();
    void nextFrame(char index); //shows next frame if in animation mode


  private:
    int _max_bright;
    int frame_index;
    float rel_unit_size;
    float rel_unit_byte;

    byte h_list[STRIP_SIZE];
    byte s_list[STRIP_SIZE];
    byte l_list[STRIP_SIZE];
    byte oldh_list[STRIP_SIZE];

    void _randomize();
    void _eff_1();
    void _eff_2();
    void _eff_4();
    void _eff_3();
    void _eff_5();
    void _eff_6();
    void _eff_7();
};
