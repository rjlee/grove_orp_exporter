# Build Instructions

The sensor is designed to be as easy to build as possible, you'll only need a craft knife and a screwdriver to complete the housing.

## Bill of Materials

*Board and sensor*
* <a href="https://www.amazon.co.uk/gp/product/B07Q4QDM11">Seeeduino Lotus Cortex-M0+</a> - £14.57
* <a href="https://cpc.farnell.com/seeed-studio/113030008/wifi-shield-v2-for-arduino/dp/SC14531">Seeed Wifi Shield V2.0</a> - £38.69
* <a href="https://coolcomponents.co.uk/products/grove-oxidation-reduction-potential-orp-sensor-kit-501z">Grove - Oxidation Reduction Potential (ORP) Sensor Kit</a> - £45.99

*Housing*
* <a href="https://www.amazon.co.uk/gp/product/B07NVRDG1V">Junction Box with Grommets Waterproof IP56</a> - £7.55
* <a href="https://www.amazon.co.uk/gp/product/B001EHAZ12">American Marine Pinpoint ORP / REDOX Calibration Fluid</a> - £7.99
* <a href="https://www.amazon.co.uk/gp/product/B08FW5BQT3">M3 A2 Stainless Steel Pozi Countersunk Machine Pozi CSK Screws Bolts</a> - £5.44
* <a href="https://thepihut.com/products/usb-a-jack-to-micro-usb-b-jack-round-panel-mount-adapter">USB A Jack to Micro USB B Jack Round Panel Mount Adapter</a> - £5.50
* <a href="https://www.ebay.co.uk/itm/BNC-Coupler-Adapter-Insulated-Female-To-Female-Panel-Mount/124401372551">BNC Coupler Adapter Insulated Female To Female Panel Mount</a> - £3.99
* <a href="https://www.ebay.co.uk/itm/Micro-USB-Type-B-Male-To-Micro-B-Male-5Pin-OTG-Adapter-Converter-Lead-Data-Cable/133524877040">Micro USB Type B Male To Micro B Male 5Pin OTG Adapter Converter Lead Data Cable</a> - £2.99
* <a href="https://www.ebay.co.uk/itm/BNC-male-to-BNC-male-pigtail-coax-cable-RG316-30cm-089/233586599610">BNC male to BNC male pigtail coax cable RG316 30cm 089</a> - £4.45
* <a href="https://cpc.farnell.com/pro-signal/psg91431/lead-usb2-0-a-male-a-male-grey/dp/CS30626">USB 2.0 A Male to A Male Cable, 5m Grey</a> - £2.50
* <a href="https://www.amazon.co.uk/SLTX-Charger-Adapter-1000mAh-Universal/dp/B07CZGLHZ6">3 Pin Plug USB Mains Charger Adapter, 1AMP</a> - £7.79

Cost of sensor & housing = £145.

The lowest cost equivalent I could find was <a href="https://atlas-scientific.com/kits/wi-fi-pool-kit/">Atlas Scientific Pool Kit</a> at $280 USD, but that is IP64 rated and has a PH sensor as well.

## Assembly

| | | |
| - | - | - |
| <img src="BOARD1.png"> | <img src="BOARD2.png"> | <img src="SENSOR.png"> |

Assemble the sensor:
* Attach the ORP sensor to the Seeduino using a Grove cable (into A1)
* Push the shield into the haders onto the Seeduino, taking care to feed the grove cable for the ORP sensor between the sheild and board
* Use a craft knife to cut holes into the rubber grommits for the USB panel adaptor and BNC adaptor and thread in the adaptors into the holes
* Connect the USB panel adaptor and BNC adaptor using the micro usb cable and the BNC cable to the respective boards
* Screw the arduino to the case

Job done.

## Build Notes

* At time of purchase, I would rather have bought a Mirco-USB panel adaptor, but they were sold out.  Get one of these if they are in stock and then you can use a sealed Micro-usb power supply to the unit.  <a href="https://thepihut.com/products/micro-usb-b-jack-to-usb-a-plug-round-panel-mount-adapter">Micro USB panel adaptor</a>.
* The ORP sensor board has a tiny screww hole, so it's not fastened in the box as I couldn't find a screw to accomodate it.
