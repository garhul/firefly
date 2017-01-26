#include <NeoPixelBus.h>
#include <WiFiUdp.h>

#define PORT 5000
#define BUFFER_SIZE 512 // in order to receive frames we need to specify such frames
#define TABLE_SIZE 129 // 2 leds per slot -> 64 slots + first led as levelshift
#define DATA_PIN 0
#define TEST_DELAY 15

//*==== define commands =====**//
#define CMD_SET_RANGE 0x01
#define CMD_SET_STRIP 0x02
#define CMD_OFF       0x03
#define CMD_TEST      0x04
#define CMD_SET_BRIGHTNESS 0x05
#define CMD_LOAD_ANIMATION 0x06
#define CMD_SET_ANIMATION 0x07
#define CMD_PLAY_ANIMATION 0x08
#define CMD_RUN_EFFECT 0x07

class TableController {
  public:
    TableController();
    void begin();
    void test(); //turn on each led to test connection
    void setColorRange(RgbColor color, int start, int end);
    void off(); //turn off all strip
    void setColor(RgbColor color); //sets single color for all strip;
    void setMaxBrightness(int brightness); //sets maximum brightness
    void service(); //runs service inside the loop method


  private:
    int max_brightness;
    bool play_effect;
    byte data[BUFFER_SIZE];
    WiFiUDP UDP;
};
