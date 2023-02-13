#include <Arduino.h>
#include "painlessMesh.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Led4digit74HC595.h>

#define   MESH_PREFIX     "MESH"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555

Led4digit74HC595 display(D1,D2,D3);

const int oneWireBus = D4;   

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

Scheduler userScheduler; 
painlessMesh  mesh;

void sendMessage();
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage(){
  Serial.println();
  Serial.println("Start Sending....");
  
  DynamicJsonDocument doc(1024);

  // get sensor temperature
  sensors.requestTemperatures();
  int temperature = sensors.getTempCByIndex(0)*10;
  
  // led display
  display.setNumber(temperature);

  // json
  doc["NAME"] = mesh.getNodeId();
  doc["TEMP"] = temperature;

  String msg ;
  serializeJson(doc, msg);

  // broadcast
  mesh.sendBroadcast( msg );
  Serial.println("Sent Message: ");
  Serial.println(msg);
  taskSendMessage.setInterval(TASK_SECOND * 100);
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.println();
  Serial.print("Reveiced Message: ");
  Serial.println(msg);
  String json;
  DynamicJsonDocument doc(1024);
  json = msg.c_str();
  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
  }
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> Start: New Connection, nodeId = %u\n", nodeId);
  Serial.println();
  Serial.printf("Structure: %s\n", mesh.subConnectionJson(true).c_str());
}
void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
  Serial.printf("Structure: , %s\n", mesh.subConnectionJson(true).c_str());
}
void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void setup() {
  Serial.begin(115200);
  display.setDecimalPoint(2); 
  mesh.setDebugMsgTypes( ERROR | STARTUP );  
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.initOTAReceive("default");
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}
void loop() {
  display.loopShow();
  mesh.update();
  // Serial.println("Sleep for 10 sec");
  // mesh.stop();
  // ESP.deepSleep(10e6); 
}