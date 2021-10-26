# MQTT Current Sensor

The idea of this sensor was to monitor the current level (in Amps) of the main electric line of my house and report its values at 5 minute intervals.

The purpose was to monitor the electricity consumption throughout the day and estimate the amount of solar panels I would need to produce that amount of energy.

I soon discovered that if I integrated 3 sensors, I could also use it to monitor electrical consumption in different circuits of my main panel. This allowed my to check that the loads were propperly balanced (they were not) and within their saftey range.

With it I've discovered other interesting things, like the massive spikes that microwave ovens produce, or that most of my electric consumption comes from my refrigerator.

## What it does

The Arduino code uses a SCT-013 non invasive current sensor, connected to an ESP32 and an electrical outlet to read and send the data through Wi-Fi and MQTT. The ESP32 is needed for the 3 sensor version, but for a 1 sensor version, an ESP8266 will also work.

The microcontroller will take measurements every 5 minutes and transform the sensor value in mA to its estimate in Amps.

Measurements will be sent through WIFI to an MQTT broker. Although the code for the MQTT in not provided, here is a brief description of what can be achieved:

With an MQTT broker, Node-Red and Grafana, you can receive the values, store them in a database and then display them in a graphical manner.

Both Grafana and Node-Red can be integrated with Telegram. This allows message alarms to be sent in case the electricity level rises above a threshold value.

--(Check explanation) --
To calculate power consumption in Watts, we must multiply the Current value (Amps) x the Voltage value. In Mexico Voltage is 110 and is assumed as a constant.

Electric power consumption in Mexico is estimated in Kwh (Kilo Watt/hour)

To transform our estimate into this notation, we would have to do the following: (explanation here)
--(Check explanation) --

## How to Install

To use this script, you must have the following:

Hardware:

- 1 ESP32
- 3 SCT-013 current sensor
- 3 10uF Capacitors
- 9 10k Resistors
- 3 20k Resistors
- An Electrical connection to power microcontroller and sensor
- A breadbord, protoboard or PCB
- Optional: [3D Printed Sensor Case](https://www.prusaprinters.org/prints/36310-esp32-energy-monitor-case)

Software:

- ESP8266 Boards installed from the Boards Manager
- Arduino Library: [EspMQTTClient.h](https://www.arduino.cc/reference/en/libraries/espmqttclient/)
- Emonlib Library: [EmonLib.h](https://www.arduino.cc/reference/en/libraries/emonlib/)

The sensor requires 3V input to work and so can be powered directly from the microcontroller.

Connection Diagram:
![Connection Diagram](https://bite-size.mx/CurrentSensorDiagram.png)
![PCB Diagram](https://bite-size.mx/CurrentSensorPCB.png)

Some Photos of the setup:
![Current Sensor Setup](https://bite-size.mx/CurrentSensorSetup.png)

## How to Use

1. Open the 3CurrentSensorMQTT.ino file:
    - Add your Wi-Fi username & password
    - Define your MQTT port (1883 is conventionally used)
    - Add your MQTT broker Username & Password
    - Define a name to identify your sensor
    - Define the MQTT topic the sensor will publish the data into

2. Verify and upload your code

3. Check with the serial monitor that the values are being reported

4. Check with your MQTT broker that values are being reported correctly through Wi-Fi.

Output should be something similar to the following:

---(Serial Monitor GIF here)---

----(Over MQTT Broker GIF here)--

Here is an image of how the information looks when written into influxdb and then displayed in Grafana. 
The graph shows a 24 hour period. 
You can see how the first line (medidor 1) uses more electricity than the second or the third line. 
You can also see periodic spikes of about 5 amps. This is the refrigerator motor turning on. 
Finally, you can see some massive spikes at 8am, 4pm and 8.30pm. This is the microwave turning on during breakfast, lunch and dinner.

![Grafana Output](http://bite-size.mx/GrafanaWaterLevelSensor.png)
