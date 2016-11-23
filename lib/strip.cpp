#include <NeoPixelBus.h>

#define PIXEL_COUNT 15

NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang400KbpsMethod> strip(PIXEL_COUNT, 0);


void setColorRange(RgbColor color, int start, int end) { //todo, check for invalid offset
  Serial.println("Setting range to color");
  if (start < 0 || end > PIXEL_COUNT)
    return;
  int i = 0;
  for (i = start; i < end; i++ ) {
    strip.SetPixelColor(i , color );
  }
  strip.Show();
}

void parseCMD ( byte buff[] ) {
  //we asume we've got our complete cmd which should be at least 8 bytes, including padding

  if (buff[0] == 0x00) { //set all stripe to color
    setColorRange(RgbColor(buff[2],buff[3],buff[4]) , 0 , PIXEL_COUNT);
  } else if (buff[0] == 0x01) { //set pixel range to color
    Serial.println("Setting color range from ");
    Serial.print(buff[1],HEX);
    Serial.print(" ");
    Serial.print(buff[2],HEX);
    Serial.println("to color");
    Serial.print(buff[3],HEX);
    Serial.print(buff[4],HEX);
    Serial.print(buff[5],HEX);
    setColorRange(RgbColor(buff[3],buff[4],buff[5]), buff[1], buff[2]);
  } else if (buff[0] == 0x0B) { //set effect

  }
}





void testStrip() {
  int i;
  for (i = 0;  i < 30; i++){
    //apagar el anterior
    if (i > 0) {
      strip.SetPixelColor(i - 1,   RgbColor(0,0,0));
    }
    strip.SetPixelColor(i, RgbColor(i * 6,0,0));
    strip.Show();
    delay(20);
  }
  delay(100);

  for (i = 0;  i < 30; i++){
    //apagar el anterior

    if (i > 0) {
      strip.SetPixelColor(i - 1,   RgbColor(0,0,0));
    }
    strip.SetPixelColor(i, RgbColor(0, i * 6 ,0));
    strip.Show();
    delay(20);
  }
  delay(100);

  for (i = 0;  i < 30; i++){
    //apagar el anterior

    if (i > 0) {
      strip.SetPixelColor(i - 1,   RgbColor(0,0,0));
    }
    strip.SetPixelColor(i, RgbColor(0, 0, i * 6));
    strip.Show();
    delay(20);
  }

  delay(100);
}



///Effects

void effect_rainbow(){
  //alternate hue from 0 to 360 on 30 leds
}

//set three colors and fade from on to the next
void effect_tricolor(){}

//set two colors and fade from one to the other
void effect_bicolor(){}

//strobe light
void effect_strobe(){}

// fade between colors in a cylce
void effect_fade(){}
