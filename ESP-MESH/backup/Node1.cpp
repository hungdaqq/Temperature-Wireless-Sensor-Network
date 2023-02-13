#include "painlessMesh.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Led4digit74HC595.h>

#define   MESH_PREFIX     "WSN2023"
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

void sendMessage() {
  Serial.println();
  Serial.println("Start Sending....");
  
  DynamicJsonDocument doc(1024);

  // get sensor temperature
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  
  // led display
  display.loopShow();
  display.setNumber(temperature);

  // json
  doc["TEMP"] = temperature;
  String msg ;
  serializeJson(doc, msg);
  
  // send 
  mesh.sendBroadcast( msg );
  Serial.println("Message from Node1");
  Serial.println(msg);
  taskSendMessage.setInterval((TASK_SECOND * 1, TASK_SECOND * 10));
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
  display.setDecimalPoint(0); 
  mesh.setDebugMsgTypes( ERROR | STARTUP );  
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  // mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}
void loop() {
  mesh.update();
}