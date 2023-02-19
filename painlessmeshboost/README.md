# PainlessMeshBoost

This is a boost based version of [painlessMesh](gitlab.com/painlessMesh/painlessMesh). For more details on how to set up painlessMesh to log to this listener see the usage instructions below and the [wiki](https://gitlab.com/BlackEdder/painlessMesh/wikis/bridge-between-mesh-and-another-network). `painlessMeshBoost` is a replacement of the old `painlessMeshListener`, which was harder to maintain and difficult to compile on low memory/arm devices such as the raspberrypi. `painlessMeshBoost` shares most of its code with `painlessMesh`, which ensures compatibility between the two.

## Install 

```
git clone --recurse-submodules https://gitlab.com/painlessMesh/painlessmeshboost.git
cd painlessmeshboost
cmake .
make
sudo make install
```

`painlessMeshBoost` depends on a number of boost libraries. On ubuntu (or other debian derivatives) these can be installed with:

```
sudo apt-get install libboost-system-dev libboost-thread-dev libboost-regex-dev libboost-program-options-dev libboost-chrono-dev libboost-date-time-dev libboost-atomic-dev libboost-filesystem-dev
```

## Usage

`painlessMeshBoost` can be run as both a server and/or a client node. By default it runs as a server node. You can log events to the console using: `painlessMeshBoost --log <event>` (if --log is not specified it will log all events). To expand the nodes behaviour further you can change the source code in `src/main.cpp`. The painlessMesh implementation is the same as in the original code and can therefore be changed in a similar way. 

### Server

This is the default mode when you run painlessMeshBoost

```
painlessMeshBoost
```

In this mode it will listen on a port (5555 by default, use `-p <port>` to change this). This mode requires you to include a [bridge](https://gitlab.com/BlackEdder/painlessMesh/blob/master/examples/bridge/bridge.ino) node in your network. 

### Client

Running it as a client requires you to setup a WiFi connection to the mesh network. Then you can run `painlessMeshBoost` to connect to the node you are connected to. You need the IP address of this node (this is typically the IP of the gateway of the WiFi connection to the mesh).

```
painlessMeshBoost --client <ip>
```

### OTA

painlessMeshBoost can be used to distribute firmware to nodes in the mesh. Firmware can be distributed to specific hardware (ESP32 or ESP8266). Different nodes often perform different roles that need specific firmware as well. The role a node fulfills is defined on the node itself. To setup painlessMeshBoost to distribute the firmware use the --ota-dir option:

```
painlessMeshBoost --ota-dir /path/to/firmware/directory
```

This causes painlessMeshBoost to start monitoring that directory. When you copy/move new firmware into that directory it will be distributed to the nodes. New firmware should follow the naming scheme: `firmware_<hardware>_<role>.bin` in order for it to be send to th correct hardware/node type. Supported hardware values are ESP32 or ESP8266.

