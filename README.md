<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/hungdaqq/Smarthome-IoT">
    <img src="images/smarthome.jpg" alt="Logo" width="800" height="420">
  </a>

<h3 align="center">Smart Home IoT</h3>

  <p align="center">
    My final project of IoT Network and Protocol course (semester 2022.1)
    <br />
    <a href="https://docs.google.com/document/d/1b8eUY19hVWinYOA9YIkjcR_toliwRPs9/edit?usp=sharing&ouid=113352961761938394358&rtpof=true&sd=true"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/hungdaqq/Smarthome-IoT">View Demo</a>
    ·
    <a href="https://github.com/hungdaqq/Smarthome-IoT/issues">Report Bug</a>
    ·
    <a href="https://github.com/hungdaqq/Smarthome-IoT/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

<p align="justify">
Temperature monitoring is a crucial aspect in various industries such as agriculture, food processing, and storage. Traditional temperature monitoring systems are often wired and can be costly to install, particularly in large facilities or areas with numerous temperature-sensitive zones. This project aims to demonstrate the feasibility of temperature monitoring using the ESP8266 microcontroller and the PainlessMesh library to create a wireless sensor network. The system is designed to provide real-time temperature data from remote sensors to a central control unit for easy monitoring and analysis. Below are the project requirements:

- Measuring range: 25°C to 125°C, Accuracy: ±1°C, Display resolution: 0.1°C.
- Powrer source: rechargeable battery with a live time of 4 hours (8 hours with advanced battery)
- Dimensions: 70x50x100 mm, Weight: <150g.
- Time to measure a sample: <20s. (advanced <5s)
- Computer connection: RF Transmission distance of up to 20 meters from measuring devices to the RF receiving station with power and network connection.
- Ability to manage a minimum of 10 measuring devices (expandable to 100 meters with advanced setup).
- Computer software: Collects measured values, manages data, exports reports in excel format, and has a user-friendly interface.
- User's manipulation: Start button, temperature threshold LEDs (3 LEDs), and the ability to update temperature thresholds from a computer.
- OTA update.

<!-- GETTING STARTED -->
## Getting Started
<br />
<div align="center">
  <a href="https://github.com/hungdaqq/Smarthome-IoT">
    <img src="images/diagram.png" alt="Logo" width="800" height="630">
  </a>
</div>

### Prerequisites
Before getting started with the project, you will need the following:
| Hardware | Software |
|----------|----------|
|A Raspberry Pi 3 or PC or any other IoT device that runs on Linux.|Raspbian Buster installed on your Raspberry Pi 3 or Debian (Ubuntu) installed on your PC.|
|NodeMCU ESP8266 for WiFi communication, DS18B20 temperature sensor,LED 7seg 4 digit display and button.|Basic understanding of programming micro controller in C/C++, IoT network protocols and devices wiring.|
|Internet connection for Raspberry Pi via Ethernet.|

### Installation

1. Clone the repo:
   ```sh
   git clone https://github.com/hungdaqq/Smarthome-IoT.git
   ```
2. Create a Thingsboard account at https://demo.thingsboard.io/ and login to use Thingsboard Live Demo server. Then go to [Thingsboard](https://github.com/hungdaqq/Smarthome-IoT/tree/main/Thingsboard) for a quick overview of this open-source IoT platform.
3. Follow the instructions to install [Thingsboard Edge CE](https://thingsboard.io/docs/user-guide/install/edge/installation-options/) v3.4.3 and provision your Edge to the Server.
4. Get and install the ThingsBoardLive on [App Store](https://apps.apple.com/us/app/thingsboard-live/id1594355695) or [Google Play](https://play.google.com/store/apps/details?id=org.thingsboard.demo.app&hl=vi&gl=US).
5. Please refer to [ESP8266](https://github.com/hungdaqq/Smarthome-IoT/tree/main/ESP8266) for setting up micro controllers programming evironment and [Features](https://github.com/hungdaqq/Smarthome-IoT/tree/main/Features) for ThingsBoard configuration as well as connecting the electronics components in accordance with the hardware schema.
6. (Optional) Follow the instructions to install [Thingsboard IoT Gateway](https://thingsboard.io/docs/iot-gateway/installation/) v2.9 on your Raspberry Pi or PC if you want integrate devices that are connected to legacy and third-party systems with ThingsBoard IoT platform. For example: external MQTT brokers, OPC-UA servers, Sigfox Backend, Modbus slaves or CAN nodes.

<!-- USAGE EXAMPLES -->
## Usage

1. Start Edge service by running the script:
   ```sh
   sudo service tb-edge start
   ```
2. Access the user interface by opening a web browser and navigating to https://demo.thingsboard.io/home or using ThingsBoardLive mobile application.
3. Use the interface to control the devices and other features in your home.


<!-- ROADMAP -->
## Roadmap and Future work

- [x] Indoor temperature monitoring
- [x] Outdoor temperature, humidity monitoring (with OpenWeather API)
- [x] Light and Household applicances control
- [x] Create alarms, send emails and notifications.
- [ ] Power consumption and charging monitoring
- [ ] Devices claming (QR code)
- [ ] Data analytics with Trendz

See the [open issues](https://github.com/hungdaqq/Smarthome-IoT/issues) for a full list of proposed features (and known issues).

<!-- CONTRIBUTING -->
## Contributing

Contributions to this project are welcome! If you have any ideas or suggestions, please feel free to open an issue or submit a pull request.

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.


<!-- CONTACT -->
## Contact

Quang Hung Dang - [@hungdaqq](https://www.linkedin.com/in/hungdaqq/) - hungdaqq@gmail.com

Project Link: [https://github.com/hungdaqq/Smarthome-IoT/](https://github.com/hungdaqq/Smarthome-IoT/)


<!-- ACKNOWLEDGMENTS -->
## Acknowledgments
<p align="justify">
This project would not have been possible without the support and resources provided by the open-source community. We would like to extend our gratitude to all the developers and organizations who have contributed to the development of the libraries and tools used in this project.
</p>
