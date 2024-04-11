# IoT monitoring with sensors - project with ESP32
This project is basis for IoT wifi application with ESP32, using it as a node in the local network with a fixed IP, including a basic front interface for the user to check the current data measured by the sensors, that should be added further, according to the demand of the project (temperature, pressure, etc).

ESP32 supports different wireless protocols, but for this project server-Sent events were used via HTTP connection and WS (websocket) protocol. These are going to be used for data should be sent without the need for a request. Libraries AsyncWebSocket, ESPAsyncWebServer, WiFi and AsyncTCP were used.

This is a project than can be useful to develop more complete applications depending on your project's needs. So, The best way to use this is to clone it into your PlatformIO workspace folder on your computer, in projects, and import the project in your IDE (I used Visual Code). The used framework used was arduino, for ESP32 DEV KIT V1, and can be used as a prototype before trying it with the official framework from espressif systems (ESP-IDF).

Check if all the C/C++ and ESP32 libraries are all installed. Before you compile the code into your ESP32 board, make your board matches the one used for this project.

In case the MCU doesn't, match, the codes can still be used, so you can use the header and source files to only get the codes you want to use, and change as you'd like.