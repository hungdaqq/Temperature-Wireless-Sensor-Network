//************************************************************
// this is an example that uses the painlessmesh library to
// upload firmware to another node on the network. 
// This will upload to an ESP device running the otaReceiver.ino
//
// The naming convetions should be as follows for bin files
// firmware_<hardware>_<role>.bin
// To create your own binary files, export them and rename
// them to follow this format, where hardware will be
// "ESP8266" or "ESP32" (capitalized.)
// If sending to the otacreceiver sketch, role should be
// "otareceiver" (lowercase)
//
//************************************************************
#include "IPAddress.h"
#include "painlessMesh.h"

#ifdef ESP8266
#include "Hash.h"
#include <ESPAsyncTCP.h>
#else
#include <AsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>


#define   STATION_SSID     "4H"
#define   STATION_PASSWORD "88998899"
#define HOSTNAME "HTTP_BRIDGE"

#define MESH_PREFIX "ESP WIFI MESH"
#define MESH_PASSWORD "12345678"
#define MESH_PORT 5555

#define OTA_PART_SIZE 1024 //How many bytes to send per OTA data packet

// Prototype
void receivedCallback( const uint32_t &from, const String &msg );
IPAddress getlocalIP();

painlessMesh  mesh;
AsyncWebServer server(80);
IPAddress myIP(0,0,0,0);
IPAddress myAPIP(0,0,0,0);

//-----------------------------------------------------------------------------
void setup(){
  Serial.begin(115200);
  delay(1000);
  mesh.setDebugMsgTypes(
      ERROR | STARTUP | CONNECTION |
      DEBUG);  // set before init() so that you can see startup messages

  // Channel set to 6. Make sure to use the same channel for your mesh and for you other
  // network (STATION_SSID)
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, 6 );
  mesh.onReceive(&receivedCallback);

  mesh.stationManual(STATION_SSID, STATION_PASSWORD);
  mesh.setHostname(HOSTNAME);

  // Bridge node, should (in most cases) be a root node. See [the wiki](https://gitlab.com/painlessMesh/painlessMesh/wikis/Possible-challenges-in-mesh-formation) for some background
  mesh.setRoot(true);
  // This node and all other nodes should ideally know the mesh contains a root, so call this on all nodes
  mesh.setContainsRoot(true);

  myAPIP = IPAddress(mesh.getAPIP());
  Serial.println("My AP IP is " + myAPIP.toString());

  //Async webserver
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", 
    "<form method='POST' action='/runupdate' enctype='multipart/form-data' id='upload_form'>Select file"
    "<input type='file' name='update'>"
    "<input type='submit' value='Upload'></form>");
    
  });

  server.on("/runupdate", HTTP_POST, [](AsyncWebServerRequest *request){
    // here the file uploaded from the webpage is passed
    // to the function OTAmesh() 
    //OTAmesh();
    request->send(200, "text/html". "Update in progress");
  });
  
  server.begin();
} //end of setup

//-------------------------------------------------

void loop(){
  mesh.update();
  if(myIP != getlocalIP()){
    myIP = getlocalIP();
    Serial.println("My IP is " + myIP.toString());
  }
 }
//--------------------------------------------------

void OTAmesh(File entry){ //over the mesh firmware update
  //This block of code parses the file name to make sure it is valid.
    //It will also get the role and hardware the firmware is targeted at.
      TSTRING name = entry.name();
      if (name.length() > 1 && name.indexOf('_') != -1 &&
          name.indexOf('_') != name.lastIndexOf('_') &&
          name.indexOf('.') != -1) {
        TSTRING firmware = name.substring(1, name.indexOf('_'));
        TSTRING hardware =
            name.substring(name.indexOf('_') + 1, name.lastIndexOf('_'));
        TSTRING role =
            name.substring(name.lastIndexOf('_') + 1, name.indexOf('.'));
        TSTRING extension =
            name.substring(name.indexOf('.') + 1, name.length());
        if (firmware.equals("firmware") &&
            (hardware.equals("ESP8266") || hardware.equals("ESP32")) &&
            extension.equals("bin")) {

          Serial.println("OTA FIRMWARE FOUND, NOW BROADCASTING");

          //This is the important bit for OTA, up to now was just getting the file. 
          //If you are using some other way to upload firmware, possibly from 
          //mqtt or something, this is what needs to be changed.
          //This function could also be changed to support OTA of multiple files
          //at the same time, potentially through using the pkg.md5 as a key in
          //a map to determine which to send
          mesh.initOTASend(
              [&entry](painlessmesh::plugin::ota::DataRequest pkg,
                       char* buffer) {
                
                //fill the buffer with the requested data packet from the node.
                entry.seek(OTA_PART_SIZE * pkg.partNo);
                entry.readBytes(buffer, OTA_PART_SIZE);
                
                //The buffer can hold OTA_PART_SIZE bytes, but the last packet may
                //not be that long. Return the actual size of the packet.
                return min((unsigned)OTA_PART_SIZE,
                           entry.size() - (OTA_PART_SIZE * pkg.partNo));
              },
              OTA_PART_SIZE);

          //Calculate the MD5 hash of the firmware we are trying to send. This will be used
          //to validate the firmware as well as tell if a node needs this firmware.
          MD5Builder md5;
          md5.begin();
          md5.addStream(entry, entry.size());
          md5.calculate(); 

          //Make it known to the network that there is OTA firmware available.
          //This will send a message every minute for an hour letting nodes know
          //that firmware is available.
          //This returns a task that allows you to do things on disable or more,
          //like closing your files or whatever.
          mesh.offerOTA(role, hardware, md5.toString(),
                        ceil(((float)entry.size()) / OTA_PART_SIZE), false);

        
      }
    }
}//end of OTAmesh
//------------------------------------------------


void receivedCallback( const uint32_t &from, const String &msg ) {
  Serial.printf("bridge: Received from %u msg=%s\n", from, msg.c_str());
}
//---------------------------------------------------
IPAddress getlocalIP() {
  return IPAddress(mesh.getStationIP());
}

//-----------------------------------------------------
void rebootEspWithReason(String reason) {
  Serial.println(reason);
  delay(1000);
  ESP.restart();
}

//-----------------------------------------------------