# StateMQ – ESP-IDF 

This directory contains native ESP-IDF files and examples using the StateMQ core

## Requirements

- ESP-IDF v5.x
- ESP32 / ESP32-S3 / ESP32-C3 

## Configuration

Configure Wi-Fi and MQTT credentials using menuconfig:

```bash
idf.py menuconfig
```

## Build & Flash

```bash
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

