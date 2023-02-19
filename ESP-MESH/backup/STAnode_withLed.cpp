#include "painlessMesh.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Led4digit74HC595.h>
#include <TM1637Display.h>

// define MESH prefix, password and port
#define   MESH_PREFIX     "CORDYCEPS"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555
// define MESH
painlessMesh  mesh;
// define LED 7seg
TM1637Display display(D2,D3);
// define temperature sensor
const int oneWireBus = D4;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
float temperature;
// parameters
bool initWiFi = true;   // for mesh init
bool connect = false;   // for AP parent node connection check
// task send message
void sendMessage(){
  // get sensor temperature
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  // display on LE 7seg 4 digit
  display.showNumberDec(round(temperature*10));
  // broadcast message
  String msg = String(temperature,4);
  mesh.sendBroadcast(msg);
}
// check connection callback
void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> Start: New Connection, nodeId = %u\n", nodeId);
  connect = true;
}
// setup
void setup() {
  Serial.begin(115200);
  display.setBrightness(4); 
  mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | COMMUNICATION );
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_STA);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.initOTAReceive("STAnode");
}
// program loop
void loop() {
  // if not init mesh
  if (!initWiFi) {
    mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_STA);
    initWiFi = true;
  }
  // get mesh information until this node is connected to a AP parent node
  mesh.update();
  if(connect) {
    // broadcast msg
    sendMessage();
    // set connection state
    connect = false;
    initWiFi = false;
    // update mesh until the msg arrived
    for ( int i = 0; i < 500; i++) {
      Serial.printf(". ");
      mesh.update();
    }
    // stop mesh: stop sending and receving msg
    mesh.stop();
    Serial.printf("%ld\n", millis());
    // go deep sleep
    ESP.deepSleep(60e6);
  }
}