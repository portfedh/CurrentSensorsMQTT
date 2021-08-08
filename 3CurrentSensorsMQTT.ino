// Codigo para medir corriente usando tres SCT-013 y un ESP32
// Board seleccionado: NODEMCU-32S
// 20 Marzo 2021

// Incluir Emon Library
#include "EmonLib.h"

// Mills Function 
const unsigned long event_interval = 300000; // 5 minute interval, 300 second
unsigned long previous_time = 0; 

// Incluir Conexion a MQTT 
#include "EspMQTTClient.h" 

#define ADC_INPUT_1 34                                                   // Sensor 1
#define ADC_INPUT_2 32                                                   // Sensor 2
#define ADC_INPUT_3 35                                                   // Sensor 3

#define WIFI_SSID        "<WiFi_username_here>"                          // WiFi Username
#define WIFI_PASS        "<WiFi_password_here>"                          // Wifi Password

#define BROKER_IP        "<local_ip_address>"                            // IP adress of MQTT broker
#define BROKER_USERNAME  "<broker_username>"                             // Broker username
#define BROKER_PASSWORD  "<broker_password>"                             // Broker password
#define CLIENT_NAME      "<MQTT_name_to_identify_device>"                // Mqtt client name to identify the device
#define BROKER_PORT      1883                                            // MQTT Port, 1883  is default
#define lastwill_topic   "<last_will_topic>"                             // MQTT topic to report lastwill and testament.
#define lastwill_text    "Current sensor has gone offline unexpextedly." // MQTT message to report lastwill and testament.

String  client_name                = CLIENT_NAME;                        // MQTT Topic to report initial value
String  startup_topic              = "<startup_topic>";                  // MQTT Topic to report startup
String  medidor_corriente1_topic   = "<topic_to_report_sensor_1>";       // MQTT topic to report values
String  medidor_corriente2_topic   = "<topic_to_report_sensor_2>";       // MQTT topic to report values
String  medidor_corriente3_topic   = "<topic_to_report_sensor_3>";       // MQTT topic to report values

// Voltaje de red eléctrica
float voltajeRed = 120.0;

// Crear tres instancias de EnergyMonitor
EnergyMonitor energyMonitor1;
EnergyMonitor energyMonitor2;
EnergyMonitor energyMonitor3;

// Function to connect to MQTT 
EspMQTTClient client(
                  WIFI_SSID,
                  WIFI_PASS,
                  BROKER_IP,        // MQTT Broker server IP
                  BROKER_USERNAME,  // Can be omitted if not needed
                  BROKER_PASSWORD,  // Can be omitted if not needed
                  CLIENT_NAME,      // Client name that uniquely identify your device
                  BROKER_PORT       // The MQTT port, default to 1883. this line can be omitted
);


void setup()
{

  Serial.begin(115200);
  
  // Enable debugging messages sent to serial output
  client.enableDebuggingMessages(); 

  // Enable the web updater.
  client.enableHTTPWebUpdater();     
  
  // MQTT Last Will & Testament
  client.enableLastWillMessage( lastwill_topic , lastwill_text);  

  // Número de pin: donde tenemos conectado el SCT-013
  // Valor de calibración: valor obtenido de la calibración teórica
  energyMonitor1.current(ADC_INPUT_1, 0.145);
  energyMonitor2.current(ADC_INPUT_2, 0.145);
  energyMonitor3.current(ADC_INPUT_3, 0.145);

}
  //MQTT Innitial Connection
  void onConnectionEstablished()
  {
    client.publish(startup_topic, String(client_name + " is now online."));
    }

void loop()
{
  // MQTT Loop: Must be called once per loop.
  client.loop(); 

  // Mills Loop
  unsigned long current_time = millis();

  // Mills If Statment
  if(current_time - previous_time >= event_interval) {

      // Pasamos el número de muestras que queremos tomar
      double Irms1 = energyMonitor1.calcIrms(1484);
      // Calculamos la potencia aparente
      double potencia1 =  Irms1 * voltajeRed;

      // Pasamos el número de muestras que queremos tomar
      double Irms2 = energyMonitor2.calcIrms(1484);
      // Calculamos la potencia aparente
      double potencia2 =  Irms2 * voltajeRed;

      // Pasamos el número de muestras que queremos tomar
      double Irms3 = energyMonitor3.calcIrms(1484);
      // Calculamos la potencia aparente
      double potencia3 =  Irms3 * voltajeRed;

      // Mostramos la información por el monitor serie
      Serial.print("Potencia 1 = ");
      Serial.print(potencia1);
      Serial.print("    Irms 1 = ");
      Serial.println(Irms1);

      // Mostramos la información por el monitor serie
      Serial.print("Potencia 2 = ");
      Serial.print(potencia2);
      Serial.print("    Irms 2 = ");
      Serial.println(Irms2);

     // Mostramos la información por el monitor serie
      Serial.print("Potencia 3 = ");
      Serial.print(potencia3);
      Serial.print("    Irms 3 = ");
      Serial.println(Irms3);
      Serial.print('\n');
      Serial.print('\n');

      // MQTT Client Publisher
      client.publish(medidor_corriente1_topic, potencia1);
      client.publish(medidor_corriente2_topic, potencia2); //float(potencia2, 1)
      client.publish(medidor_corriente3_topic, potencia3); //float(potencia3, 1)
      Serial.print('\n');

      // Update timing for next time
      previous_time = current_time;

  }
}
