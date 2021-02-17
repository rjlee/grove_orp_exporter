# grove_orp_exporter

Arduino based grove ORP prometheus exporter, that can be easily integrated into home assistant.  It allows you to remotely measure the ORP value of your pool or hot tub.

## Introduction

This project provides a simple to assemble ORP (Oxidation Reduction Potential) monitoring system.  It has four main design principles:

1. Be easy to assemble - no soldering, no drilling.
2. Be 'not too expensive' - be cheaper than many of the commercial smart ORP monitoring systems.
3. Expose reading as a HTTP interface - so it can be easily interfaced with different platforms (e.g. home assistant, grafana)
4. Be 'safe' next to the hot tub - In my case it'll be in a rattan storage unit next to the hot tub, so the case I've used is IP56, which isn't waterproof but is sealed.  Depending on your specific setup, you might want to pick a different enclosure that is waterproof (IP68).

At it's heart it uses a <a href="https://wiki.seeedstudio.com/Grove-ORP-Sensor-kit/">Grove ORP sensor kit</a>.

## Construction

Be easy to assemble - no soldering, no drilling.

You'll only need a craft knife to assemble the housing, the Gromits on the listed case are rubber, so you can cut into them easily to add the ingress for the respective cables.

*Bill of Materials*

* <a href="https://www.amazon.co.uk/gp/product/B07Q4QDM11">Seeeduino Lotus Cortex-M0+</a> - £14.57
* <a href="https://cpc.farnell.com/seeed-studio/113030008/wifi-shield-v2-for-arduino/dp/SC14531">Seeed Wifi Shield V2.0</a> - £38.69
* <a href="https://coolcomponents.co.uk/products/grove-oxidation-reduction-potential-orp-sensor-kit-501z">Grove - Oxidation Reduction Potential (ORP) Sensor Kit</a> - £45.99
* <a href="https://www.amazon.co.uk/gp/product/B07NVRDG1V">Junction Box with Grommets Waterproof IP56</a> - £7.55
* <a href="https://www.amazon.co.uk/gp/product/B001EHAZ12">American Marine Pinpoint ORP / REDOX Calibration Fluid</a> - £7.99
* <a href="https://www.amazon.co.uk/gp/product/B08FW5BQT3">M3 A2 Stainless Steel Pozi Countersunk Machine Pozi CSK Screws Bolts</a> - £5.44
* <a href="https://thepihut.com/products/usb-a-jack-to-micro-usb-b-jack-round-panel-mount-adapter">USB A Jack to Micro USB B Jack Round Panel Mount Adapter</a> - £5.50
* <a href="https://www.ebay.co.uk/itm/BNC-Coupler-Adapter-Insulated-Female-To-Female-Panel-Mount/124401372551">BNC Coupler Adapter Insulated Female To Female Panel Mount</a> - £3.99
* <a href="https://www.ebay.co.uk/itm/Micro-USB-Type-B-Male-To-Micro-B-Male-5Pin-OTG-Adapter-Converter-Lead-Data-Cable/133524877040">Micro USB Type B Male To Micro B Male 5Pin OTG Adapter Converter Lead Data Cable</a> - £2.99
* <a href="https://www.ebay.co.uk/itm/BNC-male-to-BNC-male-pigtail-coax-cable-RG316-30cm-089/233586599610">BNC male to BNC male pigtail coax cable RG316 30cm 089</a> - £4.45

Total Cost = £135.

The lowest cost equivalent I could find was <a href="https://atlas-scientific.com/kits/wi-fi-pool-kit/">Atlas Scientific Pool Kit</a> at $280 USD, but that is IP64 rated.

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
