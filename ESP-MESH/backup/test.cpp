//************************************************************
// this is a simple example that uses the easyMesh library
//
// 1. blinks led once for every node on the mesh
// 2. blink cycle repeats every BLINK_PERIOD
// 3. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 4. prints anything it receives to Serial.print
//
//
//************************************************************
#include "painlessMesh.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Led4digit74HC595.h>

#define   MESH_SSID       "XAMLOZMESH"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555
#define   MESH_CHAN       1

Led4digit74HC595 display(D1,D2,D3);
const int oneWireBus = D4;   
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

float temperature;
// Prototypes
void startMesh();
void stopMesh();
void receivedCallback(uint32_t from, String & msg);

painlessMesh  mesh;

boolean meshUp = false;
uint32_t baseline = 5*60*1000;
uint32_t stop_timer = baseline;
uint32_t delayUp = 0;
uint32_t ct1 = 0;
uint32_t tnow;

void setup() {
  Serial.begin(115200);
  display.setDecimalPoint(2); 
  mesh.setDebugMsgTypes(ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE);

  startMesh();
//   mesh.onReceive(&receivedCallback);
//   mesh.setContainsRoot(true);
//   randomSeed(analogRead(A0));
}
void sendMessage(){
  Serial.println();
  Serial.println("Start Sending ....");
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  // Led 7seg display
  display.setNumber(round(temperature*10));
  String msg = String(temperature,4);
  mesh.sendBroadcast(msg);
  Serial.printf("Sending message: %s\n", msg.c_str());
}

void loop() 
{
  mesh.update();
  tnow = millis();
  if (!meshUp)
  {
    if (tnow > delayUp)
    {
      startMesh();
    }
  }
  else
  {
    if (tnow > stop_timer && tnow > baseline)
    {
      stopMesh();
    }
  }
}

void startMesh()
{
  Serial.println("Starting mesh");
  mesh.init(MESH_SSID,MESH_PASSWORD,MESH_PORT,WIFI_STA);
  meshUp = true;
  stop_timer = millis() + 30000; // keep up for 30s
}

void stopMesh()
{
  Serial.println("Stopping mesh");
  mesh.stop();
  meshUp = false;
  delayUp = millis() + 30000;    // keep stopped for 30s
}