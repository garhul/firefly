#include "Rainbow.h"

Rainbow::Rainbow() {
  Strip Panel();
  playing = false;
}

void Rainbow::begin() {
  UDP.begin(PORT);
}

//TODO:: implement response for each command
// Service endpoint,
void Rainbow::service() {
  int dataLength = UDP.parsePacket();

  if (dataLength) {
    playing = false;
    UDP.read(data, dataLength); // read the packet into the buffer

    //debug received packet
    #ifdef DEBUG
    for (int i = 0; i < dataLength; i++) {
      Serial.print(data[i],HEX);
      Serial.print(" ");
    }
    Serial.println(" ");
    #endif

    if (data[0] == CMD_OFF) {
      Panel.clear();
    } else if (data[0] == CMD_TEST) {
      Panel.test();
    } else if (data[0] == CMD_FILL) {
      Panel.fillRGB(data[1],data[2],data[3]);
    } else if (data[0] == CMD_SET_BRIGHTNESS) {
      Panel.setMaxBrightness(data[1]);
    } else if (data[0] == CMD_SET_RANGE) {
      Panel.setRGBRange(data[3], data[4], data[5], data[1], data[2]);
    } else if (data[0] == CMD_RUN_EFFECT) {
      playing = true;
    }

    // send a response
    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
    UDP.write("ok");
    // Udp.print(system_get_chip_id());
    // Udp.write("#IP of ESP8266#");
    // Udp.println(WiFi.localIP());
    UDP.endPacket();

  } else if (playing) {
    Panel.nextFrame(data[1]);
    delay(data[2]);
  }
}
