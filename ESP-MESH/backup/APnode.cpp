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
// setting temperature threshold
const int red = 60;
const int yellow = 40;
const int green = 20;

const int oneWireBus = D4; 
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
float temperature;

bool state = false;
bool oldState = false;


Scheduler userScheduler;
painlessMesh  mesh;

bool calc_delay = true;
SimpleList<uint32_t> nodes;
bool connect = false;
int taskInterval;
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
  if(connect) {
    // get sensor temperature
    sensors.requestTemperatures();
    temperature = sensors.getTempCByIndex(0);
    // Led 7seg display
    display.setNumber(round(temperature*10));
    // thresholding leds
    if (temperature <= green)
      setLight('g');
    else if (temperature <= yellow && temperature > green)
      setLight('y');
    else
      setLight('r');
    // broadcast msg
    String msg = String(temperature,4);
    mesh.sendBroadcast(msg);
    // delay time calculation
    if (calc_delay) {
      SimpleList<uint32_t>::iterator node = nodes.begin();
      while (node != nodes.end()) {
        mesh.startDelayMeas(*node);
        node++;
      }
    }
    // set task interval
    taskSendMessage.setInterval(TASK_SECOND * taskInterval);
  }
}

void receivedCallback( uint32_t from, String &msg ) {}
void nodeTimeAdjustedCallback(int32_t offset) {}
void delayReceivedCallback(uint32_t from, int32_t delay) {}
void droppedConnectionCallback(uint32_t nodeId) {
  nodes = mesh.getNodeList();
  if (nodes.size() == 0)
    connect = false;
}
void newConnectionCallback(uint32_t nodeId) {
  connect = true;
}
void changedConnectionCallback() {}
void setup() {
  Serial.begin(115200);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(BUTTON, INPUT);

  display.setDecimalPoint(2); 
  mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | COMMUNICATION );
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onDroppedConnection(&droppedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.onNodeDelayReceived(&delayReceivedCallback);

  mesh.initOTAReceive("APnode");
  taskInterval = 60;
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}
void loop() {
  // Read button state
  state = digitalRead(BUTTON); 
  // Check button state change
  if (state != oldState) {                
    oldState = state;
    if (state)
      // send message
      sendMessage();
  }
  // Update mesh information, the mesh won't work without it
  mesh.update();
  // blink red light while the mesh is forming
  if(!connect) {
    display.sleep();
    delay(250);
    setLight('r');
    Serial.printf(".");
    delay(250);
    setLight('n');
  }
  else{
    // loop display the LED 7seg
    display.loopShow();
  }
}