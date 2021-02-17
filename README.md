# grove_orp_exporter

Arduino based grove ORP prometheus exporter, that can be easily integrated into home assistant.  It allows you to remotely measure the ORP value of your pool or hot tub.

## Introduction

This project provides a simple to assemble ORP (Oxidation Reduction Potential) monitoring system.  It has four main design principles:

1. Be easy to assemble - no soldering, no drilling.
2. Be 'not too expensive' - be cheaper than many of the commercial smart ORP monitoring systems.
3. Expose reading as a HTTP interface - so it can be easily interfaced with different platforms (e.g. home assistant, grafana)
4. Be 'safe' next to the hot tub - In my case it'll be in a rattan storage unit next to the hot tub, so the case I've used is IP56, which isn't waterproof but is sealed.  Depending on your specific setup, you might want to pick a different enclosure that is waterproof (IP68).

At it's heart it uses a <a href="https://wiki.seeedstudio.com/Grove-ORP-Sensor-kit/">Grove ORP sensor kit</a>.

-IMAGES-

See <a href="/CONSTRUCTION.md">CONSTRUCTION</a> for assembly instructions.

## Software Installation

The board used is a <a href="https://wiki.seeedstudio.com/Seeeduino_Lotus_Cortex-M0-/">Seeeduino Lotus Cortex-M0+</a>.  Follow the board setup instructions on the page to configure the board in the Arduino environment.

The Wifi shield used is a <a href="https://wiki.seeedstudio.com/Wifi_Shield_V2.0/">Wifi Shield V2.0</a>.  Follow the setup instructions on the page and download the Wifly library to your Arduino library manager.

Upload the sketch to the board, enssuring you've amended:

```
#define SSID      "YOUR_AP"
#define KEY       "YOUR_PASSWORD"
```
At this point, it should successully associate with the Wifi and you should be able to see the board output on the 'Serial Monitor'.

## Probe Callibration

The <a href="https://wiki.seeedstudio.com/Grove-ORP-Sensor-kit/">Grove ORP sensor kit</a> outlines how to calibrate the sensor and calculate the probe offset.  To note, hold and press the black button to get a consistent offset value.  Once you have the probe offset, add this to the configuration in the Arduino Sketch:

`#define OFFSET -42      //add your value here`

## Usage

The software exposes three endpoints:

| Endpoint | Usage |
| -------- | ----- |
| `/metrics` | Exposes the ORP reading in prometheus exporter format |
| `/enable` | Enables readings to be taken (default) |
| `/disable` | Disables readings, causing the `/metrics` endpoint to 404 |

### /metrics

This endpoint returns metrics that can be integrated into Grafana:

```
# TYPE orp_sensor_value_mv gauge
orp_sensor_value_mv 367.20
# HELP orp_sensor_free_memory Returns the board free memory in Kb
# TYPE orp_sensor_free_memory gauge
orp_sensor_free_memory 24351
# HELP orp_sensor_wifi_associations Returns the number of times the wifi has reassociated
# TYPE orp_sensor_wifi_associations gauge
orp_sensor_wifi_associations 0
# HELP orp_sensor_crashes Returns the number of times the board has crashed
# TYPE orp_sensor_crashes gauge
orp_sensor_crashes 0
# HELP orp_sensor_uptime Returns the board uptime in seconds
# TYPE orp_sensor_uptime gauge
orp_sensor_uptime 24
# UPTIME (DD:HH:MM:SS) 00:00:00:24
```
### /enable | /disable

These endpoints can be used to stop the ORP sensor from taking readings and cause the `/metrics` endpoint to return a 404 when set to disabled.  This can be used in an automation to stop readings if the probe is removed from the water (e.g. if the Hot Tub is turned on).

## Integration into Home Assistant

The ORP reading can be easily integrated into some assistant using a REST sensor:

```
sensor:
 - platform: rest
   resource: http://YOURSENSORIPADDRESS/metrics
   name: hottub_orp
   unit_of_measurement: "mV"
   timeout: 5
   value_template: '{{ value | regex_findall_index("orp_sensor_value_mv (\d+.\d+)\n") }}'
```

## Notes

* The Wifi shield is expensive, a better choice is probably <a href="https://wiki.seeedstudio.com/Grove-UART_Wifi_V2/">Seeed UART Wifi V2</a>.
* Similarly a Pi Pico and Grove shield might be a better board choice.
* The wifi shield does crash from time to time, but is recovered in the sketch.
* It should be trivial to add a <a href="https://wiki.seeedstudio.com/Grove-PH-Sensor-kit/">Grove PH sensor</a>.
