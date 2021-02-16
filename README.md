# grove_orp_exporter

Arduino based grove ORP prometheus exporter, that can be easily integrated into home assistant.  It allows you to remotely measure the ORP value of your pool or hot tub.

## Introduction

This project provides a simple to assemble ORP (Oxidation Reduction Potential) monitoring system.  It has three main design principles:

1. Be easy to assemble - no soldering, no drilling.
2. Be 'not too expensive' - be cheaper than many of the commercial smart ORP monitoring systems.
3. Expose reading as a HTTP interface - so it can be easily interfaced with different platforms (e.g. home assistant, grafana)

At it's heart it uses a <a href="https://wiki.seeedstudio.com/Grove-ORP-Sensor-kit/">Grove ORP sensor kit</a>.

## Installation

The board used is a <a href="https://wiki.seeedstudio.com/Seeeduino_Lotus_Cortex-M0-/">Seeeduino Lotus Cortex-M0+</a>.  Follow the board setup instructions on the page to configure the board in the Arduino environment.

Upload the sketch to the board, enssuring you've amended:

```
#define SSID      "YOUR_AP"
#define KEY       "YOUR_PASSWORD"
```
At this point, it should successully associate with the Wifi and you should be able to see the board output on the 'Serial Monitor'.

## Configuration


## Usage


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
