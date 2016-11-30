#include <NeoPixelBus.h>
#include <WiFiUdp.h>

#define PORT 5000
#define BUFFER_SIZE 512
#define STRIP_LENGTH 16
#define DATA_PIN 0
#define TEST_DELAY 80

//*==== define commands =====**//
#define CMD_SET_RANGE 0x01
#define CMD_SET_STRIP 0x02
#define CMD_OFF       0x03
#define CMD_TEST      0x04
#define CMD_SET_BRIGHTNESS 0x05
#define CMD_LOAD_ANIMATION 0x06
#define CMD_SET_ANIMATION 0x07

class StripController {
  public:
    StripController();
    void begin();
    void test(); //turn on each led to test connection
    void setColorRange(RgbColor color, int start, int end);
    void off(); //turn off all strip
    void setColor(RgbColor color); //sets single color for all strip;
    void setMaxBrightness(int brightness); //sets maximum brightness
    void service(); //runs service inside the loop method
    ///Effects
    void effect_rainbow();

    void effect_tricolor();

    void effect_bicolor();

    void effect_strobe();

    void effect_fade();

  private:
    int max_brightness;
    // NeoPixelBus<NeoPixelBus , NeoEsp8266BitBang400KbpsMethod> Strip (uint16_t, uint8_t);
    WiFiUDP UDP;
};
