#include <Arduino.h>
#include <defaults.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <EEPROM.h>
#include <NeoPixelBus.h>
#include <Rainbow.h>
#include <WiFiUdp.h>
#include <Hash.h>
#include <WebSocketsServer.h>

Rainbow rb;

MDNSResponder mdns;
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1984);

#define USE_SERIAL Serial
WiFiUDP UDP;

void serveFile(const char *filepath, const char *doctype = "text/html") {
  if (! SPIFFS.exists(filepath)) {
    server.send(404,"text/plain", "File not found");
    Serial.println("file not found");
    Serial.println(filepath);
    return ;
  }

  File f = SPIFFS.open(filepath, "r");
  if (!f) {
    server.send(500, "text/plain", "Error opening file");
    return;
  }

  //read the file in chunks (not that much ram)
  server.streamFile(f, doctype);
  f.close();
}

/**websockets test**/


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    Serial.println("escuche algo");
    switch(type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

				// send message to client
				webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
            USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);

            // send message to client
            // webSocket.sendTXT(num, "message here");

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;
        case WStype_BIN:
            USE_SERIAL.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);

            rb.run(payload,length);

            // send message to client
            // webSocket.sendBIN(num, payload, length);
            break;
    }

}



/** Station & Access point service endpoints **/
void homeAP() {
  serveFile("/main.html");
}

void descriptor() {
  serveFile("/descriptor.json");
}

void controls() {
  serveFile("/controls.html");
}

void reboot() {
  ESP.restart();
}

void clearCredentials() {
  for (int i = 0; i < 96; ++i) {
    EEPROM.write(i, 1);
  }

  if (EEPROM.commit()) {
    server.send(200,"application/json","{message:'Device eeprom cleared'}");
  } else {
    server.send(500,"text/plain","{message:'Error clearing eeprom'}");
  }
}

void announce() {
  //send a broadcast message attempting to reach the server to begin handshake
  UDP.beginPacket(BROADCAST_IP, BROADCAST_PORT);
  UDP.print(system_get_chip_id());
  int rs = UDP.endPacket();

  Serial.println(system_get_chip_id());
  Serial.println(rs,DEC);
}

void cmd() {
  if (server.hasArg("cmd")) {
    String c = server.arg("cmd");
    byte b[c.length()];
    byte n[3] = {0x07,0x02,0x00};
    c.getBytes(b, c.length() + 1);
    rb.run(n, 3);
    server.send(200, "application/json", "{message:'ok'}");
  } else {
    server.send(404,"application/json", "{message:'no cmd provided'}");
  }
}

void setupAP() {
  //check post params exist
  if(server.hasArg("pwd") && server.hasArg("ssid")) {
    //first we clear everything
    for (int i = 0; i < 96; ++i) {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();

    for (int i = 0; i < server.arg("ssid").length(); ++i) {
      EEPROM.write(i, server.arg("ssid")[i]);
    }

    for (int i = 0; i < server.arg("pwd").length(); ++i) {
      EEPROM.write(32+i, server.arg("pwd")[i]);
    }

    if (EEPROM.commit()) {
        Serial.println("Guardado");
    }
    //return json success
    server.send(200, "application/json", "{message:'settings stored'}");
  } else {
    //return json error
    server.send(400, "application/json", "{message:'Invalid params'}");
  }
}

void beginAP() {
  WiFi.mode(WIFI_AP);

  #if AP_USE_PWD
    WiFi.softAP(AP_SSID, AP_PWD);
  #else
    WiFi.softAP(AP_SSID);
  #endif

  IPAddress myIP = WiFi.softAPIP();

  server.on("/", homeAP);
  server.on("/descriptor", descriptor);
  server.on("/controls", controls);
  server.on("/setup", HTTP_POST, setupAP);
  server.on("/reboot", reboot);
  server.on("/cmd", HTTP_POST, cmd);


  server.begin();
  WiFi.printDiag(Serial);
  Serial.println(WiFi.softAPIP());

  if ( mdns.begin ( AP_DEVICE_NAME, WiFi.softAPIP() )) {
    Serial.println ( "MDNS responder started" );
  }
}

bool beginST() {
  Serial.println("starting station mode");
  WiFi.mode(WIFI_STA);
  int attempts = 0;

  String pwd = "B41l4C0m0UnT3rr3m0t0"; //Elsochoridedorapa
  String ssid = "Elsochoridedorapa"; //B41l4C0m0UnT3rr3m0t0

  //try to load eeprom data for SSID and pwd
  // for (int i = 0; i < 32; ++i) {
  //   ssid += char(EEPROM.read(i));
  // }
  //
  // for (int i = 32; i < 96; ++i) {
  //   pwd += char(EEPROM.read(i));
  // }

  //if we are already connected:
  if (WiFi.status() == WL_CONNECTED) {
    //dsconnect and wait
    WiFi.disconnect(false);
    delay(5000);
  }

  while (WiFi.status() != WL_CONNECTED && attempts < CONN_RETRIES) { //wait until we connect
    Serial.println("creds...");
    Serial.println(ssid.c_str());
    Serial.println(pwd.c_str());

    WiFi.begin ( ssid.c_str(), pwd.c_str() );
    attempts++;
    delay(ST_CONN_TIMEOUT);
    Serial.println("Station connection attempt failed, retrying");
  }

  //are we connected yet ?
  if (WiFi.status() != WL_CONNECTED)
    return false;

  Serial.println("Station startup successful");
  WiFi.printDiag(Serial);

  if (mdns.begin (ST_DEVICE_NAME, WiFi.localIP() ) ) {
    Serial.print ( "MDNS responder started" );
    Serial.println (WiFi.localIP());
  }

  //setup basic endpoints
  server.on("/clear", clearCredentials); //endpoint for clearing ssid / pwd
  server.on("/descriptor" , descriptor); //endpoint for serving the descriptor
  server.on("/announce" , announce); //request device to announce itself
  server.on("/reboot", reboot); //reboot the device
  server.on("/update", setup); //update descriptor info
  server.on("/cmd", HTTP_POST, cmd);


  server.begin();

  //announce this device
  announce();

  return true;
}

void setup ( void ) {
  Serial.begin(115200);

  EEPROM.begin(EEPROM_SIZE);

  SPIFFS.begin();
  WiFi.persistent(false);

  delay(5000);

  clearCredentials();

   if (!beginST()) {
    Serial.println("Station startup failed, going into AP mode");
    beginAP();
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
   } else {
    //initialize device controller
    rb.begin();
    cmd();
  }
}

void loop ( void ) {
  mdns.update();
  server.handleClient();
  rb.service();
  webSocket.loop();
}
