# ESP8266-NodeMCU-8-Button-WebSocket-Controller

This repository contains code for an 8-Button ESP8266-based WebSocket controller designed to interface with Node-RED. It allows you to control and monitor button states from your ESP8266 NodeMCU using WebSocket communication.

## Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- [ESP8266 Arduino Core](https://github.com/esp8266/Arduino)

## Getting Started

1. Clone this repository to your local machine.

    ```bash
    git clone https://github.com/HaroldPetersInskipp/ESP8266-NodeMCU-8-Button-WebSocket-Controller.git
    ```

2. Open the `controller.ino` file in the Arduino IDE.

3. Install the required libraries via Arduino Library Manager:

    - ESP8266WiFi
    - WebSockets
    - ArduinoJson

4. Update the following variables in the code:

    - `ssid`: Your Wi-Fi network SSID.
    - `password`: Your Wi-Fi network password.
    - `webSocketServer`: IP address of your Node-RED server.
    - `webSocketPort`: Port of your Node-RED server. (customize if needed).
    - `webSocketEndpoint`: WebSocket endpoint (customize if needed).

5. Optionally, set a unique `esp8266unitNumber` to identify each ESP8266 NodeMCU.

6. Upload the code to your ESP8266 NodeMCU.

## Usage

1. Connect your ESP8266 NodeMCU to the power source.

2. Open the Arduino IDE Serial Monitor to view detailed information about the ESP8266's boot process, Wi-Fi connection, and WebSocket connection.

3. Observe button presses on the Serial Monitor when buttons are pressed. The WebSocket connection status will also be displayed.

4. The WebSocket server will receive button states in real-time.

## Pin Configuration

The ESP8266 NodeMCU is configured with the following GPIO pins for buttons:

- "left" - D4
- "down" - D5
- "up" - D6
- "right" - D7
- "circle" - D3
- "triangle" - D2
- "cross" - D1
- "square" - A0 (analog button)

## WebSocket Server Configuration

Ensure that your Node-RED server is configured to accept WebSocket connections. The WebSocket endpoint should match the `webSocketEndpoint` defined in the code.

## Additional Information

- This code includes detailed boot information, Wi-Fi connection details, and WebSocket connection status in the Serial Monitor.

- The `esp8266unitNumber` provides a unique identifier for each ESP8266 NodeMCU, aiding in device management.

- Button states are sent to the WebSocket server only when buttons are pressed, minimizing unnecessary communication.

## License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/HaroldPetersInskipp/ESP8266-NodeMCU-8-Button-WebSocket-Controller/blob/main/LICENSE) file for details.
