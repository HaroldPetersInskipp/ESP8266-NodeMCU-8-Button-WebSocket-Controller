#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

const char* esp8266unitNumber = "ESP8266controller_1";  // Optionally set a esp8266unitNumber to identify each ESP8266 NodeMCU (example "ESP8266controller_1")

// Wi-Fi credentials
const char* ssid = "your_ssid_here";
const char* password = "your_wifi_password_here";

// WebSocket server details
const char* webSocketServer = "your_Node-RED_server_IP_here";
const int webSocketPort = 1880; // Default Node-RED port
const char* webSocketEndpoint = "/controller_1"; //Websocket endpoint can be changed here

// Structure to define buttons
struct Button {
  const char* name;
  int pin;
};

// Array of buttons with names and corresponding GPIO pins
Button buttons[] = {
  {"left", D4},
  {"down", D5},
  {"up", D6},
  {"right", D7},
  {"circle", D3},
  {"triangle", D2},
  {"cross", D1},
  {"square", A0}
};

const int numButtons = sizeof(buttons) / sizeof(buttons[0]);

// Create a Wi-Fi client and a WebSocket client
WiFiClient client;
WebSocketsClient webSocket;

// Function prototypes
void connectWiFi(); // Function to establish Wi-Fi connection
void setupButtons(); // Function to configure button GPIO pins
void readButtonStates(StaticJsonDocument<128>& jsonDoc); // Function to read button states into a JSON document
void sendButtonStates(const StaticJsonDocument<128>& jsonDoc); // Function to send button states to the WebSocket server

void setup() {
  Serial.begin(115200);
  delay(100);

  // Print boot message
  Serial.println("\n\nStarting the boot process...");

  // Print device identifier
  Serial.print("Device identifier: ");
  Serial.println(esp8266unitNumber);

  // Print firmware version
  Serial.print("Firmware version: ");
  Serial.println(ESP.getSdkVersion());

  // Print details about the device's firmware
  Serial.print("Boot version: ");
  Serial.println(ESP.getBootVersion());
  Serial.print("Boot mode: ");
  Serial.println(ESP.getBootMode());
  Serial.print("CPU frequency: ");
  Serial.println(ESP.getCpuFreqMHz());
  Serial.print("Flash size: ");
  Serial.println(ESP.getFlashChipSize());

  // Print memory usage
  Serial.print("Free heap: ");
  Serial.println(ESP.getFreeHeap());
  Serial.print("Heap fragmentation: ");
  Serial.println(ESP.getHeapFragmentation());

  // Print chip information
  Serial.print("Chip ID: ");
  Serial.println(ESP.getChipId());
  Serial.print("Flash chip ID: ");
  Serial.println(ESP.getFlashChipId());

  Serial.println("\nConnecting to Wi-Fi");
  delay(250);
  connectWiFi(); // Connect to Wi-Fi

    // Print STATION mode specific details about the device's WiFi capabilities
  Serial.print("WiFi SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("WiFi local IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("WiFi subnet mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("WiFi gateway IP address: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("WiFi DNS server IP address: ");
  Serial.println(WiFi.dnsIP());

  // Print the remaining details about the device's WiFi capabilities
  Serial.print("WiFi mode: ");
  Serial.println(WiFi.getPhyMode());
  Serial.print("WiFi channel: ");
  Serial.println(WiFi.channel());
  Serial.print("WiFi MAC address: ");
  Serial.println(WiFi.macAddress());

  // Print WiFi connection status
  Serial.print("WiFi status: ");
  Serial.println(WiFi.status());

  // Print WiFi signal strength
  Serial.print("WiFi RSSI: ");
  Serial.println(WiFi.RSSI());

  setupButtons(); // Set up button GPIO pins

  // End the boot process and print client ready message
  Serial.println("Ending the boot process...\n");
  Serial.print(esp8266unitNumber);
  Serial.println(" should now be operational.\n");

  // Fancy graphics
  Serial.println(" ★BUTTON LAYOUT★");
  Serial.println(" ___ ___ ___ ___ ");
  Serial.println("| 0 | 1 | 2 | 3 |");
  Serial.println("|---|---|---|---|");
  Serial.println("| 4 | 5 | 6 | 7 |");
  Serial.println(" --- --- --- --- \n");

  // Initialize WebSocket client and set up event handling
  webSocket.begin(webSocketServer, webSocketPort, webSocketEndpoint);
  Serial.println("Connecting to WebSocket");
  webSocket.onEvent([](WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
      case WStype_DISCONNECTED:
        Serial.println("WebSocket disconnected");
        break;
      case WStype_CONNECTED:
        Serial.println("WebSocket connected");
        Serial.print("WebSocket endpoint: http://");
        Serial.print(webSocketServer);
        Serial.print(":");
        Serial.print(webSocketPort);
        Serial.println(webSocketEndpoint);
        break;
      case WStype_TEXT:
        // Handle incoming text data if needed
        break;
      case WStype_BIN:
        // Handle incoming binary data if needed
        break;
      default:
        break;
    }
  });
}

void loop() {
  webSocket.loop(); // Allow WebSocket client to process incoming messages

  StaticJsonDocument<128> jsonDoc;
  readButtonStates(jsonDoc); // Read button states into the JSON document
  sendButtonStates(jsonDoc); // Send button states to the WebSocket server

  delay(100); // Introduce a small delay to control the loop frequency
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void setupButtons() {
  for (int i = 0; i < numButtons - 1; i++) {
    pinMode(buttons[i].pin, INPUT_PULLUP); // Configure GPIO buttons as INPUT with internal pull-up resistors
  }
  pinMode(A0, INPUT); // Configure analog button as INPUT
}

void readButtonStates(StaticJsonDocument<128>& jsonDoc) {
  for (int i = 0; i < numButtons - 1; i++) {
    // Read the state of each button and update the JSON document
    bool buttonState = digitalRead(buttons[i].pin) == LOW;
    jsonDoc[buttons[i].name] = buttonState ? 1 : 0;

    // Print Serial message if button is pressed
    if (buttonState) {
      Serial.println(buttons[i].name);
    }
  }

  // Read the state of the analog button and update the JSON document
  bool analogButtonState = analogRead(A0) >= 1000;
  jsonDoc[buttons[7].name] = analogButtonState ? 1 : 0;

  // Print Serial message if analog button is pressed
  if (analogButtonState) {
    Serial.println(buttons[7].name);
  }
}

void sendButtonStates(const StaticJsonDocument<128>& jsonDoc) {
  char payload[128];
  serializeJson(jsonDoc, payload); // Serialize the JSON document to a char array
  if (webSocket.isConnected()) {
    webSocket.sendTXT(payload); // Send the JSON payload to the WebSocket server if connected
  }
}