# MQTT Current Sensor

The idea of this sensor is to monitor the current level (in Amps) of an electric line and report its values at 5 minute intervals.

The purpose of the project was to monitor the electricity consumption of my home throughout the day and estimate the amount of solar panels I would need to produce that amount of energy.

I chose to use 3 sensors to monitor the electrical consumption in different circuits of the main panel. This allowed me to check if the loads were propperly balanced (they were not) and if electricity consumption was within a saftey range.

Afterwards I integrated the sensor with a [Raspberry Pi](https://www.raspberrypi.org/). Using 
[Mosquitto MQTT](https://mosquitto.org/),
[Node-RED](https://nodered.org/),
[InfluxDB](https://www.influxdata.com/) and
[Grafana](https://grafana.com/),
I was able to store and visuallize the information, as well as adding some [Telegram](https://core.telegram.org/bots), alerts for when power consumption is very high. 

With the sensor and this setup I could vidualize several interesting things, such as the massive spikes that microwave ovens produce when they are turned on, or that most of my electric consumption comes from my refrigerator.

## What it does

The setup uses a SCT-013 non invasive current sensor, connected to an ESP32 and an electrical outlet to read and send the data through Wi-Fi and MQTT. The ESP32 is needed for the 3 sensor version, but for a 1 sensor version, an ESP8266 will also work. (See the image of the first prototype below).

The microcontroller takes measurements every 5 minutes and transforms the sensor value in mA to its estimate in Amps. Voltage is hardcoded as 120V but can be susbstituted by a voltage sensor if more precise calculations if needed.

Measurements are then sent through WIFI to an MQTT broker. With a Raspberry Pi and using Mosquitto MQTT, Node-Red, influxDB and Grafana a home monitoring system can be set up.

Mosquitto MQTT is an MQTT broker, which allows Node-Red to read the MQTT messages from the sensor and store them in a database (influxDB). Grafana is a data visualization tool that can extract the information from InfluxDB and display it in a visually apealing and interactive way. Node Red can also be integrated with Telegram, which allows message alarms to be sent in case the electricity level rises above a threshold value.

### One way to estimate the power consumption of your home, would be as follows:

(Electric power consumption in Mexico is estimated in Kwh (Kilo Watt/hour), If your country does it differently, this may not apply).

- We asume Voltage = 120V (or 220V depending whey you live)
- We take our average Amperage reding from the sensor. Say 10 Amps (as an example)
- Our Power value, in Watts, would be: P = 120V x 10A = 1,200W
- Our Energy value would be: E = P x T, where T = time (expressed in hours).
- Our Energy value would thus be: E = 1,200W x 1 = 1,200 Watt/hour
- Finally, we divide by 1,000 to get KiloWatt/hour values = 1.2 Kwh

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

## Output Example
Output should be something similar to the following:

#### When your code uploads you should get a confirmation in the serial monitor:
![SerialMonitor](https://bite-size.mx/Current_Serial_Monitor.png)

#### Using Mosquitto MQTT you can also see the values as they are reported in real time.
In this example, we are suscribing to a single sensor topic named "Cordilleras/medidor_corriente1", not all three.
![SerialMonitor](https://bite-size.mx/Current_Mosquitto.png)

#### Using Node-RED you can save the MQTT information into InfluxDB
The purple nodes read the MQTT information.

The orange function nodes save the information of each sensor as a variable.

Green Nodes are for debuging and testing.

The blue node executes automatically every 5 minutes.

The Orange node saves all three variables as a single payload.

Finally, the brown node saves the information into InfluxDB. 

![NodeRED](https://bite-size.mx/Current_NodeRed.png)

#### This is how the information looks inside the InfluxDB Database
![NodeRED](https://bite-size.mx/Current_Influxdb.png)

#### Visualize the data using Grafana
Below is an image of how the information looks when written into influxdb and then displayed in Grafana.
The graph shows a 24 hour period.

You can see how the first electric line (medidor 1) uses more electricity than the second or the third line.

You can also see periodic spikes of about 5 amps. This is the refrigerator motor turning on.
Finally, you can see some massive spikes at 8am, 4pm and 8.30pm. This is the microwave turning on during breakfast, lunch and dinner.

If you compare the electricity consumption of line 1, 2 and 3, it is clear that loads are unbalanced, since most of the electric consumption is going on in line 1. 

![Grafana Output](http://bite-size.mx/GrafanaCurrentSensor.png)

### Below ara some pictures of the Hardware Setup:

### Connection Diagram:

![Connection Diagram](https://bite-size.mx/CurrentSensorDiagram.png)

### PCB Diagram:

![PCB Diagram](https://bite-size.mx/CurrentSensorPCB.png)

### Prototype using a 3D printed diagram and a single sensor:

![PCB Diagram](https://bite-size.mx/Current_Prototype.jpg)

### Prototype using a breadboard and three sensors:

![PCB Diagram](https://bite-size.mx/Current_Breadboard.jpg)

### CNC Milling of the PCB using a Genmitsu 3018-PROVer:

![PCB Diagram](https://bite-size.mx/Current_MillingPCB.jpg)

### Completed PCB Test (cardboard vs copper):

![PCB Diagram](https://bite-size.mx/Current_PCBTest.jpg)

### Completed PCB:

![PCB Diagram](https://bite-size.mx/Current_PCBFinal.jpg)

### Completed PCB (Backside):

![PCB Diagram](https://bite-size.mx/Current_PCBBackside.jpg)

### Completed PCB (Backside2):

![PCB Diagram](https://bite-size.mx/Current_PCBConnected.jpg)

### Labeled PCB (Backside):

![PCB Diagram](https://bite-size.mx/Current_PCBLabeled.jpg)

### Soldered PCB:

![PCB Diagram](https://bite-size.mx/Current_PCBSoldered.jpg)

### Completed Sensor:

![Overview of the Sensor Case](https://bite-size.mx/Current_PCBDone.jpg)

### Sensor Inside the case, glued with silicone:

![Overview of the Sensor Case](https://bite-size.mx/Current_Case.jpg)

### Sensor Inside the case (open):

![Overview of the Sensor Case](https://bite-size.mx/Current_Final.jpg)

### Completed Sensor case (closed):

![Overview of the Sensor Case](https://bite-size.mx/CurrentSensorComplete.jpg)

### The sensor mounted in a wall:

![Current Sensor Case](https://bite-size.mx/ElectricSensorCase.png)

### Sensors connected to the line, taking measurements:

![Current Measuring Sensors](https://bite-size.mx/CurrentSensors.png)

### Sensors calibration:

Once the hardware is setup, I used the following procedure to calibrate the sensors:

- Split an extention cord so that the line and neutral wires are separated.
- Hook up the extention cord to the main power.
- Connect a heater, blow drier or any device that uses a significant amount of electricity.
- Hook up the sensors to one of the extension cord lines.
- Hook up a professional current sensor to the same extension cord line (borrow or rent one if you dont have one)
- Turn the heater on.
- Compare the values in your sensors to the current sensor.
- Adjust the calibration value in the code.
- Upload the code and try again until readings match.

Since all three sensors are hooked up to the same line, you can also check if readings are consistent.

The line of code where the sensors can be calibrated is:

`energyMonitor1.current(ADC_INPUT_1, 0.145);`

Here is a gif of the procedure:

![Current Measuring Sensors](https://bite-size.mx/CurrentSensorCalibration.gif)