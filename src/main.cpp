#include <Arduino.h>
#include <defaults.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <EEPROM.h>
#include <NeoPixelBus.h>
#include <Table.h>
#include <WiFiUdp.h>

TableController Table;

MDNSResponder mdns;
ESP8266WebServer server(80);
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

    EEPROM.commit();
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
  server.on("/setup", HTTP_POST, setupAP);
  server.on("/reboot", reboot);

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

  String pwd = "";
  String ssid = "";

  //try to load eeprom data for SSID and pwd
  for (int i = 0; i < 32; ++i) {
    ssid += char(EEPROM.read(i));
  }

  for (int i = 32; i < 96; ++i) {
    pwd += char(EEPROM.read(i));
  }

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

  if (!beginST()) {
    Serial.println("Station startup failed, going into AP mode");
    beginAP();
  } else {
    //initialize device controller
    Table.begin();
  }
}

void loop ( void ) {
  mdns.update();
  server.handleClient();
  Table.service();
}
