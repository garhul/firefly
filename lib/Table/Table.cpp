#include "Table.h"
#include "Animations.h"
NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart800KbpsMethod> Strip (TABLE_SIZE, D10);

TableController::TableController() {
  max_brightness = 0; //set max power
  play_effect = false;
}

void TableController::begin() {
  Strip.Begin();
  UDP.begin(PORT);
}

void TableController::setColorRange(RgbColor color, int start, int end) {
  if (start < 0 || end > TABLE_SIZE)
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
void TableController::test() {
  // clear strip first
  Strip.ClearTo(RgbColor(0,0,0));
  Strip.Show();
  Serial.println("testing started");
  for (int i = 0; i < TABLE_SIZE; i++ ) {
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
  Serial.println("test finished");
  //clear strip again
  Strip.ClearTo(RgbColor(0,0,0));
  Strip.Show();
}


//TODO:: implement response for each command
// Service endpoint,
void TableController::service() {
  int dataLength = UDP.parsePacket();

  if (play_effect) {
    if (data[1] == 0) {
    } else if (data[1] == 1) {
      delay(35);
    }
  }

  if (dataLength) {
    play_effect = false;
    UDP.read(data, dataLength); // read the packet into the buffer

    //debug received packet
    for (int i = 0; i < dataLength; i++) {
      Serial.print(data[i],HEX);
      Serial.print(" ");
    }
    Serial.println(" ");

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
    } else if (data[0] == CMD_RUN_EFFECT) {
      play_effect = true;
    }


    // Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    // Udp.write("Answer from ESP8266 ChipID#");
    // Udp.print(system_get_chip_id());
    // Udp.write("#IP of ESP8266#");
    // Udp.println(WiFi.localIP());
    // Udp.endPacket();
  }
}
