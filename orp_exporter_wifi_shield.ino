#include <SoftwareSerial.h>
#include "WiFly.h"

// Wifi connection settings
#define SSID      "YOUR_AP"
#define KEY       "YOUR_PASSWORD"
#define AUTH      WIFLY_AUTH_WPA2_PSK
#define RETRIES   "30"

// ORP sensor settings for Grove ORP sensor
#define ORPPIN 1        //orp meter output,connect to Arduino controller ADC pin (A1)
#define VOLTAGE 5.00    //system voltage
#define OFFSET -42      //zero drift voltage

// ORP sensor value
double orpValue;
int crashes = 0;
int drops = 0;
boolean enabled = true;

// https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__
int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

// Initialise uptime variables
// https://hackaday.io/project/7008-fly-wars-a-hackers-solution-to-world-hunger/log/25043-updated-uptime-counter
long TotalSeconds = 0;
long Day = 0;
int Hour = 0;
int Minute = 0;
int Second = 0;
int HighMillis = 0;
int Rollover = 0;
// Uptime monitor
void uptime() {
  //** Making Note of an expected rollover *****//
  if (millis() >= 3000000000) {
    HighMillis = 1;
  }
  //** Making note of actual rollover **//
  if (millis() <= 100000 && HighMillis == 1) {
    Rollover++;
    HighMillis = 0;
  }
  long secsUp = millis() / 1000;
  TotalSeconds = (Rollover * 50) + (secsUp);
  Second = secsUp % 60;
  Minute = (secsUp / 60) % 60;
  Hour = (secsUp / (60 * 60)) % 24;
  Day = (Rollover * 50) + (secsUp / (60 * 60 * 24)); //First portion takes care of a rollover [around 50 days]
};

String returnUptime() {
  char output[11];
  sprintf(output, "%02d:%02d:%02d:%02d", Day, Hour, Minute, Second);
  return output;
};

SoftwareSerial wiflyUart(2, 3); // create a WiFi shield serial object
WiFly wifly(&wiflyUart); // pass the wifi siheld serial object to the WiFly class

void connectToWifi()
{
  Serial.println("------Configuring Wifi shield------");
  wifly.reset(); // reset the shield
  delay(1000);

  //set parameters to make wifi shield safer against outside attacks.
  wifly.sendCommand("set ip tcp-mode 0x10\r"); // disable remote configuration via TCP connections
  delay(100);
  wifly.sendCommand("set wlan hide 1\r"); //when the module shows the wlan settings, the passphrase is shown as ******
  delay(100);

  //set WiFly params
  wifly.sendCommand("set ip local 80\r"); // set the local comm port to 80
  delay(100);
  wifly.sendCommand("set comm remote 0\r"); // do not send a default string when a connection opens
  delay(100);
  wifly.sendCommand("set comm open *OPEN*\r"); // set the string that the wifi shield will output when a connection is opened
  delay(100);

  // https://forum.arduino.cc/index.php?topic=538388.0
  //set parameters so the module automatically connects to the access point and attempts to reconnect after loss of connection
  wifly.sendCommand("set wlan auth WIFLY_AUTH_WPA2_PSK\r"); //set authentication mode to WPA2 with personal key
  delay(100);
  wifly.sendCommand("set wlan passphrase ssid " SSID "\r"); //set the SSID
  delay(100);
  wifly.sendCommand("set wlan passphrase " KEY "\r"); //set the passphrase (WPA/WPA2 personal key)
  delay(500); //longer delay needed, otherwise linkmon causes failure of connection; no documentation found on this topic; just trial and error :-(
  //set parameters so the module automatically connects to the access point and attempts to reconnect after loss of connection
  delay(500); //longer delay needed, otherwise linkmon causes failure of connection; no documentation found on this topic; just trial and error :-(
  wifly.sendCommand("set wlan linkmon " RETRIES "\r"); //set the link monitor to RETRIES attempts of reconnecting when detecting the loss of connection to the access point
  delay(500); //longer delay needed, otherwise linkmon causes failure of connection; no documentation found on this topic; just trial and error :-(
  wifly.sendCommand("set wlan join 1\r"); //set the mode (value 1) so the module will auto-connect and re-connect to the access point
  delay(100);

  Serial.println("Join " SSID );
  // check if WiFly is associated with AP(SSID)
  if (!wifly.isAssociated(SSID)) {
    while (!wifly.join(SSID, KEY, AUTH)) {
      Serial.println("Failed to join " SSID);
      Serial.println("Wait 0.5 secs and try again...");
      delay(500);
    }
    wifly.save();    // save configuration,
  }
  Serial.println("Success joining " SSID);
  // Print the IP configuration
  delay(5000);
  wifly.sendCommand("get ip\r");
  char c;
  while (wifly.receive((uint8_t *)&c, 1, 300) > 0) { // print the response from the get ip command
    Serial.print((char)c);
  }
  Serial.println("------Web server ready------");
}

void htmlHeader(String code, String output) {
  String contentLength = "Content-Length: ";
  contentLength += output.length();

  wiflyUart.print("HTTP/1.1 ");
  wiflyUart.println(code);
  wiflyUart.println("Content-Type: text/html; charset=UTF-8");
  wiflyUart.println(contentLength);
  wiflyUart.println("Connection: close");
  wiflyUart.println();
  wiflyUart.print(output);
}

void setup()
{
  wiflyUart.begin(9600); // start wifi shield uart port
  Serial.begin(9600); // start the arduino serial port
  // wait for initilization of wifly
  delay(1000);
  connectToWifi();
}

void loop()
{
  // Update uptime counters
  uptime();

  if (wifly.available())
  { // the wifi shield has data available

    // Recieve and print the request
    char c;
    String response;
    while (wifly.receive((uint8_t *)&c, 1, 1000) > 0) {
      response += (char)c;
    }
    Serial.println(response);

    if (response.indexOf("OPEN*")) {

      delay(1000); // delay enough time for the browser to complete sending its HTTP request string
      
      String output = "";
      if (response.indexOf("GET /metrics") > 0) {

        // Only return a 200 OK if sensor is enabled
        if (enabled == true) {
          // Sample three times to calibrate
          for (int sample = 0; sample <= 3; sample++) {
            orpValue = ((30 * (double)VOLTAGE * 1000) - (75 * analogRead(ORPPIN) * VOLTAGE * 1000 / 1024)) / 75 - OFFSET; //convert the analog value to orp according the circuit
            delay(200);
          }
          // Output in prometheus compatible format, including the uptime as a comment
          output += "# HELP orp_sensor_value_mv Returns the sensor value from the ORP sensor in mV\n";
          output += "# TYPE orp_sensor_value_mv gauge\n";
          output += "orp_sensor_value_mv ";
          output += orpValue;
          output += "\n";
          output += "# HELP orp_sensor_free_memory Returns the board free memory in Kb\n";
          output += "# TYPE orp_sensor_free_memory gauge\n";
          output += "orp_sensor_free_memory ";
          output += freeMemory();
          output += "\n";
          output += "# HELP orp_sensor_wifi_associations Returns the number of times the wifi has reassociated\n";
          output += "# TYPE orp_sensor_wifi_associations gauge\n";
          output += "orp_sensor_wifi_associations ";          
          output += drops;
          output += "\n";
          output += "# HELP orp_sensor_crashes Returns the number of times the board has crashed\n";
          output += "# TYPE orp_sensor_crashes gauge\n";
          output += "orp_sensor_crashes ";
          output += crashes;
          output += "\n";
          output += "# HELP orp_sensor_uptime Returns the board uptime in seconds\n";
          output += "# TYPE orp_sensor_uptime gauge\n";
          output += "orp_sensor_uptime ";
          output += TotalSeconds;
          output += "\n";
          output += "# UPTIME (DD:HH:MM:SS) ";
          output += returnUptime();
          output += "\n";
          htmlHeader("200 OK", output);
        } else {
          output = "<html><body>Sensor Disabled</body></html>";
          htmlHeader("404 Not Found", output);
        }
      } else if (response.indexOf("GET /enable") > 0) {
        enabled = true;
        output = "<html><body>Sensor Enabled</body></html>";
        htmlHeader("200 OK", output);
      } else if (response.indexOf("GET /disable") > 0) {
        enabled = false;
        output = "<html><body>Sensor Disabled</body></html>";
        htmlHeader("200 OK", output);
      } else {
        output = "<html><body><ul><li><a href='/metrics'>/metrics</a></li></ul></body></html>";
        htmlHeader("404 Not Found", output);
      }
      Serial.println(output);
    } else if (response.indexOf("CLOS*") > 0) {
      // Ack connection close
    } else if (response.indexOf("isconn") > 0 || response.indexOf("AUTH-ERR") > 0) {
      // AP association failure - Disconn / AUTH-ERR
      drops += 1;
      Serial.println("Lost Wifi AP association, reconnecting ...");
    } else if (response.indexOf("CRASH*") > 0) {
      // Total board crash, cross your fingers
      crashes += 1;
      Serial.println("Wifi board has crashed ...");
      delay(1000);
      connectToWifi();
    } else {
      Serial.println("UNKNOWN MESSAGE");
    }
  }
}
