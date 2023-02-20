#include "painlessMesh.h"
#include <TM1637Display.h>

#define   MESH_PREFIX     "WSN_2022"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555 

// #define LED D5
const int CLK = D6;
const int DIO = D5;
// int Bstate;
// float h, t;
float temperature;

TM1637Display display(CLK, DIO);
Scheduler userScheduler; 
painlessMesh  mesh;

void receivedCallback( uint32_t from, String &msg ) {

  Serial.println("Message from Node2");
  Serial.println("Message =");
  Serial.println(msg);
  String json = msg.c_str();
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, json);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
  }
  // Bstate = doc["Button"]; 
  temperature = doc["TEMP"];
  // h = doc["HUM"];

  // if (t > 27 || h > 50)  
  // digitalWrite(LED,HIGH);
  // else(digitalWrite(LED,LOW));
  display.showNumberDec(round(temperature*10)); // Display Temperature in TM1637 Displayss
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}
void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}
void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void setup() {
  Serial.begin(115200);
  // pinMode(LED, OUTPUT);
  display.setBrightness(4);  
  mesh.setDebugMsgTypes( ERROR | STARTUP );  
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
}

void loop() {
  mesh.update();
}