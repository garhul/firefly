#include "Strip.h"

//TODO:: make this a class property
NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang400KbpsMethod> Strip (STRIP_LENGTH, DATA_PIN);

StripController::StripController() {
  max_brightness = 0;
}

void StripController::begin() {
  Strip.Begin();
  test();
  //initialize UDP listener
  UDP.begin(PORT);
}

void StripController::setColorRange(RgbColor color, int start, int end) {
  if (start < 0 || end > STRIP_LENGTH)
    return;
  int i = 0;
  for (i = start; i < end; i++ ) {
    Strip.SetPixelColor(i , color );
  }

  Strip.Show();
}

/**
 Run a test seting on and then off each pixel color for all the strip length
 **/
void StripController::test() {
  // clear strip first
  Strip.ClearTo(RgbColor(0,0,0));
  Strip.Show();

  for (int i = 0; i < STRIP_LENGTH; i++ ) {
    //red goes first
    Strip.SetPixelColor(i, RgbColor(255,0,0));
    Strip.Show();
    delay(TEST_DELAY);
    Strip.SetPixelColor(i, RgbColor(0,255,0));
    Strip.Show();
    delay(TEST_DELAY);
    Strip.SetPixelColor(i, RgbColor(0,0,255));
    Strip.Show();
    delay(TEST_DELAY);
    Strip.SetPixelColor(i, RgbColor(0,0,0));
    Strip.Show();
    delay(TEST_DELAY);
  }

  //clear strip again
  Strip.ClearTo(RgbColor(0,0,0));
  Strip.Show();
}


//TODO:: implement response for each command
// Service endpoint,
void StripController::service() {
  int dataLength = UDP.parsePacket();
  byte data[BUFFER_SIZE];

  if (dataLength) {
    UDP.read(data, dataLength); // read the packet into the buffer

    //debug received packet
    Serial.println("-------");
    for (int i = 0; i < dataLength; i++) {
      Serial.print(data[i],HEX);
      Serial.print(" ");
    }

    if (data[0] == CMD_OFF) {
      Strip.ClearTo(RgbColor(0,0,0));
      Strip.Show();
    } else if (data[0] == CMD_TEST) {
      test();
    } else if (data[0] == CMD_SET_STRIP) {
      Strip.ClearTo(RgbColor(data[1],data[2],data[3]));
      Strip.Show();
    } else if (data[0] == CMD_SET_BRIGHTNESS) {
      max_brightness = data[1];
    } else if (data[0] == CMD_SET_RANGE) {
      setColorRange(RgbColor(data[3], data[4], data[5]), data[1], data[2]);
    }


    // Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    // Udp.write("Answer from ESP8266 ChipID#");
    // Udp.print(system_get_chip_id());
    // Udp.write("#IP of ESP8266#");
    // Udp.println(WiFi.localIP());
    // Udp.endPacket();
  }
}
