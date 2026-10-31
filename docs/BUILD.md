# Reproducible build

Validated on Windows on 2026-10-31 with Arduino CLI 1.5.1, ESP8266 core 3.1.2,
Adafruit NeoPixel 1.15.5 and board `esp8266:esp8266:nodemcuv2`.
LittleFS, ESP8266WiFi and ESP8266WebServer ship with the board core.

Install [Arduino CLI](https://docs.arduino.cc/arduino-cli/getting-started/), then run from the repository root:

```sh
arduino-cli core update-index --additional-urls https://arduino.esp8266.com/stable/package_esp8266com_index.json
arduino-cli core install esp8266:esp8266@3.1.2 --additional-urls https://arduino.esp8266.com/stable/package_esp8266com_index.json
arduino-cli lib install "Adafruit NeoPixel@1.15.5"
arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 --warnings all Lithophane_Light
node --test tests/web-ui.test.cjs
```

The script tests require Node.js 20 or later and no npm packages. They execute the
actual embedded JavaScript in a mocked DOM/network environment. They do not test
browser rendering, real HTTP parsing, firmware execution or electrical behavior.

The GitHub Actions workflow repeats these checks. Action references are pinned
to commit hashes and the workflow has read-only repository permissions.

## Upload

In Arduino IDE choose NodeMCU 1.0 (ESP-12E Module), a filesystem-enabled flash
layout suitable for your board, and its serial port. Compile and upload the sketch.
The HTML is embedded; no filesystem image upload is necessary. Settings require
a working LittleFS partition. Uploading firmware does not necessarily erase saved
settings. Do not format a device that contains settings you need to preserve.

No upload was performed as part of this review. Follow `TESTING.md` on the actual
lamp before relying on the firmware.
