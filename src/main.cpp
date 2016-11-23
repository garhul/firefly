#include <Arduino.h>
#include <defaults.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <EEPROM.h>

MDNSResponder mdns;
ESP8266WebServer server(80);
// WiFiUDP Udp;
// register events
// WiFiEventHandler onSoftAPModeStationConnected;
// WiFiEventHandler onSoftAPModeStationDisconnected;

/**
  Start the device in access point mode and serve a small webapp for configuration
*/

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

/** Station & Access point service endpoints **/
void homeAP() {
  serveFile("/main.html");
}

void descriptor() {
  serveFile("/descriptor.json");
}

void reboot() {
  ESP.restart();
}

void clearEEPROM() {
  //empty eeprom contents
  for (int i = 0; i < 96; ++i) {
    EEPROM.write(i, 0);
  }

  EEPROM.commit();
}

void announce() {
  //send a broadcast message attempting to reach the server to begin handshake

}

void setupAP() {
  //check post params exist
  if(server.hasArg("pwd") && server.hasArg("ssid")) {

    clearEEPROM();

    Serial.println("writing eeprom ssid:");
    for (int i = 0; i < server.arg("ssid").length(); ++i) {
      EEPROM.write(i, server.arg("ssid")[i]);
      Serial.print("Wrote: ");
      Serial.println(server.arg("ssid")[i]);
    }

    Serial.println("writing eeprom pass:");
    for (int i = 0; i < server.arg("pwd").length(); ++i) {
      EEPROM.write(32+i, server.arg("pwd")[i]);
      Serial.print("Wrote: ");
      Serial.println(server.arg("pwd")[i]);
    }


    EEPROM.commit();
    //return json success
    server.send(200, "application/json", "{message:'settings stored'}");

  } else {
    //return json error
    server.send(400, "application/json", "{message:'Invalid params'}");
  }
}



void beginAP() {
  #if AP_USE_PWD
    WiFi.softAP(AP_SSID, AP_PWD);
  #else
    WiFi.softAP(AP_SSID);
  #endif

  IPAddress myIP = WiFi.softAPIP();

  server.on("/", homeAP);
  server.on("/descriptor", descriptor);
  server.on("/setup", HTTP_POST, setupAP);
  server.on("/reboot", reboot);

  server.begin();
  WiFi.printDiag(Serial);
  Serial.println(WiFi.softAPIP());

  if ( mdns.begin ( AP_DEVICE_NAME, WiFi.softAPIP() )) {
    Serial.println ( "MDNS responder started" );
  }
}


bool beginST(const char* ssid,const char* pwd) {
  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < CONN_RETRIES) { //wait until we connect
    WiFi.begin ( ssid, pwd );
    attempts++;
    delay(ST_CONN_TIMEOUT);
    Serial.println("Station connection attempt failed, retrying");
  }

  //are we connected yet ?
  if (WiFi.status() != WL_CONNECTED)
    return false;

  return true;
}

void setup ( void ) {
  Serial.begin(115200);
  SPIFFS.begin();
  delay(5000);

  String pwd = "";
  String ssid = "";

  //try to load eeprom data for SSID and pwd
  for (int i = 0; i < 32; ++i) {
    ssid += char(EEPROM.read(i));
  }

  for (int i = 32; i < 96; ++i) {
    pwd += char(EEPROM.read(i));
  }

  if (!beginST(ssid.c_str(), pwd.c_str())) {
    Serial.println("Station startup failed, going into AP mode");
    beginAP();
  } else {
    Serial.println("Station startup successful");
    WiFi.printDiag(Serial);

    if (mdns.begin (ST_DEVICE_NAME, WiFi.localIP() ) ) {
      Serial.println ( "MDNS responder started" );
    }

    //setup basic endpoints
    server.on("/clear", clearEEPROM); //endpoint for clearing ssid / pwd
    server.on("/descriptor" , descriptor); //endpoint for serving the descriptor
    server.on("/announce" , announce);
    server.on("/reboot", reboot);
    server.on("/update", setup);
    //setup module

  }
}


void loop ( void ) {
  mdns.update();
  server.handleClient();
}
