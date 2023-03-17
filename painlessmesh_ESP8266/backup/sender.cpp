#include "painlessMesh.h"
#include <TM1637Display.h>

#define   MESH_PREFIX     "CORDYCEPS"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555 

#define   SENDBUTTON D5
#define   SETBUTTON D6

TM1637Display display(D2,D3);
// state parameters
bool stateSend = false;
bool oldStateSend = false;
bool stateSet = false;
bool oldStateSet = false;
painlessMesh  mesh;

int green = 20;
int yellow = 30;
int red = 60;

void sendMessage() {
  DynamicJsonDocument doc(1024);
  // json
  doc["GREEN"]  = green;
  doc["YELLOW"] = yellow;
  doc["RED"]    = red;
  String msg ;
  serializeJson(doc, msg);
  // send 
  mesh.sendBroadcast( msg );
}

void receivedCallback( uint32_t from, String &msg ) {}
void newConnectionCallback(uint32_t nodeId) {}
void changedConnectionCallback() {}
void nodeTimeAdjustedCallback(int32_t offset) {}

void setup() {
  Serial.begin(115200);
  pinMode(SENDBUTTON, INPUT);
  pinMode(SETBUTTON, INPUT);
  display.setBrightness(4);
  mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | COMMUNICATION );  
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_STA, 6);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
}

unsigned long onTime;
int config = 0;

void loop() {
  stateSend = digitalRead(SENDBUTTON); 
  // Check button state change
  if (stateSend != oldStateSend) {                
    oldStateSend = stateSend;
    if (stateSend) {
      // send message
      sendMessage();
      onTime = millis();
    }
    else {
      if ((millis() - onTime) > 1000) {
        if (config == 3)
          config = 1;
        else
          config += 1;
      }
    }
  }
  stateSet = digitalRead(SETBUTTON); 
  if (config == 1) {
    display.showNumberDec(green);
    if (stateSet != oldStateSet) { 
      oldStateSet = stateSet;
      if (stateSet) {
        green ++;
        display.showNumberDec(green);
      }
    }
  }
  else if (config == 2) {
    display.showNumberDec(yellow);
    if (stateSet != oldStateSet) { 
      oldStateSet = stateSet;
      if (stateSet) {
        yellow ++;
        display.showNumberDec(yellow);
      }
    }
  }
  else if (config == 3) {
    display.showNumberDec(red);
    if (stateSet != oldStateSet) { 
      oldStateSet = stateSet;
      if (stateSet) {
        red ++;
        display.showNumberDec(red);
      }
    }
  }
  mesh.update();
}