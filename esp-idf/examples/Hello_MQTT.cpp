// main/app_main.cpp
//
// StateMQ ESP-IDF example: simple HELLO / BYE state control.
//
// MQTT interface (what to publish / subscribe):
// - Publish to:   hello/state
//   Messages:     "hi", "bye"
//   Effect:       changes internal StateMQ and led state 
//
//
// Notes:
// - Demonstrates basic message-to-state mapping.
// - Periodic tasks react to the current state.
//

#include <cstring>

#include "sdkconfig.h"
#include "StateMQ_ESP.h"

#include "driver/gpio.h"
#include "esp_timer.h"

using namespace statemq;

// ---------------- pins ----------------
static constexpr gpio_num_t LED_PIN = GPIO_NUM_21;

// ---------------- topics ----------------
// Incoming control messages that select the device "State".
static constexpr const char* STATE_TOPIC = "hello/state";

// ---------------- node ----------------
// Create the core state machine
static StateMQ node;

// Configure the ESP32 MQTT node
static StateMQEsp esp(node);

using StateId = StateMQ::StateId;
static StateId HELLO_ID = StateMQ::CONNECTED_ID;
static StateId BYE_ID   = StateMQ::CONNECTED_ID;

// ---------------- helpers ----------------
static inline uint32_t nowMs() {
  return (uint32_t)(esp_timer_get_time() / 1000ULL);
}

static inline void ledWrite(bool on) {
  gpio_set_level(LED_PIN, on ? 1 : 0);
}

// ---------------- tasks ----------------

// Task 1: print message based on state
static void printTask() {
  StateId s = node.stateId();

  if (s == HELLO_ID) {
    printf("Hello world\n");
  }
  else if (s == BYE_ID) {
    printf("Bye world\n");
  }
  else if (s == StateMQ::OFFLINE_ID) {
    printf("Offline\n");
  }
  else if (s == StateMQ::CONNECTED_ID) {
    printf("Connected\n");
  }
}

// Task 2: control LED based on state
static void ledTask() {
  StateId s = node.stateId();

  if (s == HELLO_ID) {
    ledWrite(true);
  }
  else if (s == BYE_ID) {
    ledWrite(false);
  }
  else if (s == StateMQ::OFFLINE_ID) {
    // blink when offline
    ledWrite(((nowMs() / 500) % 2) != 0);
  }
}

extern "C" void app_main(void) {
  // GPIO setup
  gpio_config_t io{};
  io.mode = GPIO_MODE_OUTPUT;
  io.pin_bit_mask = (1ULL << LED_PIN);
  gpio_config(&io);
  ledWrite(false);

  // MQTT (topic,payload) -> StateId
  // Publish to hello/state with payload:
  //   "hi"  -> HELLO
  //   "bye" -> BYE
  HELLO_ID = node.map(STATE_TOPIC, "hi",  "HELLO");
  BYE_ID   = node.map(STATE_TOPIC, "bye", "BYE");

  // tasks
  node.taskEvery("print", 500, Stack::Small, printTask, true);
  node.taskEvery("led",   100, Stack::Small, ledTask, true);

  // MQTT wrapper configuration
  esp.setSubscribeQos(STATE_TOPIC, /*qos=*/1);

  // menuconfig credentials
  const char* ssid   = CONFIG_STATEMQ_WIFI_SSID;
  const char* pass   = CONFIG_STATEMQ_WIFI_PASS;
  const char* broker = CONFIG_STATEMQ_BROKER_URI;

  // Start Wi-Fi and MQTT
  esp.begin(ssid, pass, broker, nullptr);
}
