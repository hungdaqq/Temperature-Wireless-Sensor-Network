#include "painlessMesh.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Led4digit74HC595.h>

#define   MESH_PREFIX     "XAMLOZMESH"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555

Led4digit74HC595 display(D1,D2,D3);

#define BUTTON D5
#define RED D6
#define YELLOW D7
#define GREEN D8

const int red = 60;
const int yellow = 40;
const int green = 20;

const int oneWireBus = D4;   
bool state = false;
bool oldState = false;

float temperature;

// unsigned long timeSend = 0;

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// Scheduler userScheduler; 
painlessMesh  mesh;

bool calc_delay = true;
SimpleList<uint32_t> nodes;

// void sendMessage();
// Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );
bool connect = false;

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
  // Serial.println();
  // Serial.println("Start Sending ....");
  // DynamicJsonDocument doc(1024);
  // get sensor temperature
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  
  // Led 7seg display
  display.setNumber(round(temperature*10));

  // if (temperature <= green)
  //   setLight('g');
  // else if (temperature <= yellow && temperature > green)
  //   setLight('y');
  // else
  //   setLight('r');
  // json
  // doc["NAME"] = mesh.getNodeId();
  // doc["TEMP"] = temperature;
  String msg = String(temperature,4);
  // serializeJson(doc, msg);
  // timeSend = micros();
  mesh.sendBroadcast(msg);
  // Serial.println("");
  // Serial.printf("Broadcasting time: %ld us\n", micros() - timeSend);

  // if (calc_delay) {
  //   SimpleList<uint32_t>::iterator node = nodes.begin();
  //   while (node != nodes.end()) {
  //     mesh.startDelayMeas(*node);
  //     node++;
  //   }
  // }
  // Serial.printf("Sending message: %s\n", msg.c_str());
  // mesh.sendBroadcast( msg );
  // Serial.println("Sent Message: ");
  // Serial.printf("Alive time: %ld ms\n", millis() - timeSend);
  // taskSendMessage.setInterval(TASK_SECOND * 30);
}

// void receivedCallback( uint32_t from, String &msg ) {
//   Serial.printf("Received from: %u, message: %s\n", from, msg.c_str());
//   // String json;
//   // DynamicJsonDocument doc(1024);
//   // json = msg.c_str();
//   // DeserializationError error = deserializeJson(doc, json);
//   // if (error) {
//   //   Serial.print("deserializeJson() failed: ");
//   //   Serial.println(error.c_str());
//   // }
// }
void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> Start: New Connection, nodeId = %u\n", nodeId);
  connect = true;
}

// void droppedConnectionCallback(uint32_t nodeId) {
//   Serial.printf("Dropped Connection, nodeId = %u\n", nodeId);
// }

// void nodeTimeAdjustedCallback(int32_t offset) {
//   Serial.printf("Adjusted node time: %u. Offset = %d us\n", mesh.getNodeTime(), offset);
// }
// void delayReceivedCallback(uint32_t from, int32_t delay) {
//   Serial.printf("Delay received from %u: %u us\n", from, delay);
// }

unsigned long timer;
unsigned long sleepTimer = 10e3;

void setup() {
  Serial.begin(115200);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(BUTTON, INPUT);
  display.setDecimalPoint(2); 
  mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | COMMUNICATION);
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_STA);
  // mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  // mesh.onChangedConnections(&changedConnectionCallback);
  // mesh.onDroppedConnection(&droppedConnectionCallback);
  // mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  // mesh.onNodeDelayReceived(&delayReceivedCallback);
  mesh.initOTAReceive("STAnode");
  // userScheduler.addTask( taskSendMessage );
  // taskSendMessage.enable()
  // wifi_set_sleep_type(MODEM_SLEEP_T);
}
bool initWiFi = true;

void loop() {
  // state = digitalRead(BUTTON); 
  // if (state != oldState) {                
  //   oldState = state;
  //   delay(200);
  //   if (state) {
  //     sendMessage();
  //   }
  // }           
  display.loopShow();
  timer = millis();
  if (!initWiFi) {
    mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_STA);
    initWiFi = true;
  }
  mesh.update();

  if(connect) {
    sendMessage();
    connect = false;
    initWiFi = false;
    for ( int i = 0; i < 100; i++) {
      mesh.update();
    }
    Serial.printf("Time Alive: %ld", millis() - timer);
    mesh.stop();
    ESP.deepSleep(20e6);
  }
  // sleepTimer = millis();
    // WiFi.forceSleepWake();
    // WiFi.disconnect();
    // WiFi.forceSleepBegin();
    // delay(1);
  // else {
  //   Serial.printf("%ld\n", millis());
  // }
  // if(connect){
  //   sendMessage();
  //   mesh.stop();
  //   WiFi.disconnect();
  //   WiFi.forceSleepBegin();
  //   delay(1);
  //   connect = false;
  // }
}