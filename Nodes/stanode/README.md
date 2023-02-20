# STA Nodes setup 
## List of hardware
- 01 NodeMCU ESP8266 
- 01 Push button 9 (4 pin)
- 01 Temperature Sensor DS18B20
- microUSB cable
- LiPo battery

## Installation steps
1. Replace `painlessmesh_ESP8266/src` with the `.cpp` file in `Nodes/stanode`, then upload the code to the NodeMCU.
2. Connect the parts in line with the following hardware schemas:  
![Schema1](https://github.com/hungdaqq/Smarthome-IoT/blob/main/Features/Kitchen/schema.png)  
3. Placed in the proper position.

## Feature
- Power-saving (sleep in 99% life-time).
