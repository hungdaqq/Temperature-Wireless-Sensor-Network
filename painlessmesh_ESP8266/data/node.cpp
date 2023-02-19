#include "painlessMesh.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Led4digit74HC595.h>
#include <string.h>

#define   MESH_PREFIX     "CORDYCEPS"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555

Led4digit74HC595 display(D1,D2,D3);

#define BUTTON D5
#define RED D6
#define YELLOW D7
#define GREEN D8

const int red = 60;
const int yellow = 40;
const int green = 25;

const int oneWireBus = D4;   
bool state = false;
bool oldState = false;

float temperature;

unsigned long timeSend = 0;

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

Scheduler userScheduler; 
painlessMesh  mesh;

bool calc_delay = true;
SimpleList<uint32_t> nodes;

void sendMessage();
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void setLight(char STATE){
  if (STATE == 'g') {
    digitalWrite(GREEN, HIGH);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);
  }
  else if (STATE == 'y') {
    digitalWrite(YELLOW, HIGH);
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
  }
  else if (STATE == 'r') {
    digitalWrite(RED, HIGH);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, LOW);    
  }
  else if (STATE == 'n') {
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, LOW);       
  }
  else if (STATE == 'a') {
    digitalWrite(RED, HIGH);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(GREEN, HIGH);       
  }
}

void sendMessage(){
  // timeSend = millis();
  Serial.println();
  Serial.println("Start Sending ....");
  
  // DynamicJsonDocument doc(1024);

  // get sensor temperature
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  
  // Led 7seg display
  display.setNumber(round(temperature*10));

  if (temperature <= green)
    setLight('g');
  else if (temperature <= yellow && temperature > green)
    setLight('y');
  else
    setLight('r');

  // json
  // doc["NAME"] = mesh.getNodeId();
  // doc["TEMP"] = temperature;

  String msg = String(temperature,4);
  // serializeJson(doc, msg);

  mesh.sendBroadcast(msg);

  if (calc_delay) {
    SimpleList<uint32_t>::iterator node = nodes.begin();
    while (node != nodes.end()) {
      mesh.startDelayMeas(*node);
      node++;
    }
  }
  // Serial.printf("Sending message: %s\n", msg.c_str());
  taskSendMessage.setInterval(TASK_SECOND * 30);
}

void receivedCallback( uint32_t from, String &msg ) {
  // Serial.printf("Received from: %u, message: %s\n", from, msg.c_str());
  // String json;
  // DynamicJsonDocument doc(1024);
  // json = msg.c_str();
  // DeserializationError error = deserializeJson(doc, json);
  // if (error) {
  //   Serial.print("deserializeJson() failed: ");
  //   Serial.println(error.c_str());
  // }
}

void newConnectionCallback(uint32_t nodeId) {
  // Serial.printf("--> Start: New Connection, nodeId = %u\n", nodeId);
  // Serial.println();
  // Serial.printf("Structure: %s\n", mesh.subConnectionJson(true).c_str());
}

void droppedConnectionCallback(uint32_t nodeId) {
  // Serial.printf("Dropped Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  // Serial.printf("Changed connections\n");
  // Serial.printf("Structure: , %s\n", mesh.subConnectionJson(true).c_str());
  // nodes = mesh.getNodeList();
  // Serial.printf("Num nodes: %d\n", nodes.size());
  // Serial.printf("Connection list:");
  // SimpleList<uint32_t>::iterator node = nodes.begin();
  // while (node != nodes.end()) {
  //   Serial.printf(" %u", *node);
  //   node++;
  // }
  // Serial.println();
}

void nodeTimeAdjustedCallback(int32_t offset) {
  // Serial.printf("Adjusted node time: %u. Offset = %d us\n", mesh.getNodeTime(), offset);
}
void delayReceivedCallback(uint32_t from, int32_t delay) {
  // Serial.printf("Delay received from %u: %u us\n", from, delay);
}

void setup() {
  Serial.begin(115200);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(BUTTON, INPUT);

  display.setDecimalPoint(2); 
  mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | COMMUNICATION );
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler,MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onDroppedConnection(&droppedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.onNodeDelayReceived(&delayReceivedCallback);

  mesh.initOTAReceive("node");
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}
void loop() {
  state = digitalRead(BUTTON); 
  if (state != oldState) {                
    oldState = state;
    delay(100);
    if (state) {
      sendMessage();
    }
  }           
  display.loopShow();
  mesh.update();
}