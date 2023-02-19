//************************************************************
// this is a simple example that uses the painlessMesh library to 
// connect to a node on another network. Please see the WIKI on gitlab
// for more details
// https://gitlab.com/painlessMesh/painlessMesh/wikis/bridge-between-mesh-and-another-network
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "CORDYCEPS"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555

#define   STATION_SSID     "MINH MUP_2.4G"
#define   STATION_PASSWORD "28051989"
#define   STATION_PORT     5555
uint8_t   station_ip[4] =  {0,0,0,0}; // IP of the server

// prototypes
void receivedCallback( uint32_t from, String &msg );

painlessMesh  mesh;

void setup() {
  Serial.begin(115200);
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION | COMMUNICATION | DEBUG);  // set before init() so that you can see startup messages

  // Channel set to 6. Make sure to use the same channel for your mesh and for you other
  // network (STATION_SSID)
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, 6 );
  // Setup over the air update support
  mesh.initOTAReceive("BridgeNode");

  mesh.stationManual(STATION_SSID, STATION_PASSWORD, STATION_PORT, station_ip);
  // Bridge node, should (in most cases) be a root node. See [the wiki](https://gitlab.com/painlessMesh/painlessMesh/wikis/Possible-challenges-in-mesh-formation) for some background
  mesh.setRoot(true);
  // This node and all other nodes should ideally know the mesh contains a root, so call this on all nodes
  mesh.setContainsRoot(true);

  mesh.onReceive(&receivedCallback);
}

void loop() {
  mesh.update();
}

void receivedCallback( uint32_t from, String &msg ) {}
