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
#include <StateMQ.h>

StateMQ node;
StateMQEsp32 esp(node);

void setup() {
  node.map("node/cmd", "ON",  "ON");
  node.map("node/cmd", "OFF", "OFF");

  node.taskEvery("print", 1000, small, [] {
    Serial.println(node.state());
  });

  esp.begin("SSID", "PASS", "mqtt://broker");
}

void loop() {}
```