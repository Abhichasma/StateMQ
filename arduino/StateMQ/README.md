# StateMQ (Arduino-ESP32)

StateMQ is a small, deterministic, state-based MQTT framework for ESP32.
This Arduino package wraps the same StateMQ core used by the ESP-IDF version.

## Requirements

- ESP32 board
- Arduino-ESP32 core (v2.x or later)

## Installation

1. Clone the repository
2. Copy:
  ```bash
   arduino/StateMQ → ~/Arduino/libraries/StateMQ
   ```
3. Restart the Arduino IDE

## Minimal Example

```cpp
#include <Arduino.h>
#include <StateMQ_ESP32.h>

const char* WIFI_SSID   = "your_ssid";
const char* WIFI_PASS   = "your_pass";
const char* MQTT_BROKER = "mqtt://192.168.1.10:1883";
const char* STATE_TOPIC = "hello/topic";

StateMQ node;
StateMQEsp32 esp(node);

StateId HELLO_ID;
StateId BYE_ID;

void printTask() {
  auto st = node.stateId();

  if (st == HELLO_ID) Serial.println("Hello world");
  else if (st == BYE_ID) Serial.println("Bye world");
}

void setup() {
  Serial.begin(115200);
  //Publish hi to hello/topic
  HELLO_ID = node.map(STATE_TOPIC, "hi", "HELLO");
  BYE_ID   = node.map(STATE_TOPIC, "bye",   "BYE");

  node.taskEvery("print", 1000, small, printTask, true);

  esp.begin(WIFI_SSID, WIFI_PASS, MQTT_BROKER);
}

void loop() {}
```
