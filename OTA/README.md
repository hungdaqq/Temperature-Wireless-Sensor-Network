## OTA Protocol
The protocol consists of three message types: version, request and data. The first message is generally send by the node that distributes the firmware and announces the current version of firmware available for each hardware and nodeType. Firmware version is determined by its MD5 signature.  

Once a node receives a version message it will check it against its own type and hardware to discover if it is aimed at this node. If that checks out and the MD5 is different than its own MD5 it will send a data request back to the firmware distribution node. This request also includes a partNo, to determine which part of the data it needs (starting from zero).  

When the distribution node receives a data request, it sends the data back to the node (with a data message). The node will then write this data and request the next part of the data, until the node has all the data, written it and reboots into the new firmware. For an example see the otaReceiver in the examples folder.

## Steps
1. Replace `painlessmesh_ESP8266/src` with the `.cpp` file `ota.cpp`, then upload the code to the NodeMCU.
2. Connect the parts in line with the following hardware schemas:  
![Schema1](https://github.com/hungdaqq/Temperature-Wireless-Sensor-Network/blob/main/OTA/microsd.png)  
3. Build a File system image, result in a `spinff.bin`. You have to rename the file according to the device and role. For example: `firmware_ESP8266_apnode.bin`. Move this file to a microSD card. 
4. Insert the SD card to the module.
