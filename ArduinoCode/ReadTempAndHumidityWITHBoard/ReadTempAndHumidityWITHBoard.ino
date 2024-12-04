/**
 * DHT11 Sensor Reader
 * This sketch reads temperature and humidity data from the DHT11 sensor and prints the values to the serial port.
 * It also handles potential error states that might occur during reading.
 *
 * Author: Dhruba Saha
 * Version: 2.1.0
 * License: MIT
 */

// Include the DHT11 library for interfacing with the sensor.
#include <DHT11.h>

// Create an instance of the DHT11 class.
// - For Arduino: Connect the sensor to Digital I/O Pin 2.
// - For ESP32: Connect the sensor to pin GPIO2 or P2.
// - For ESP8266: Connect the sensor to GPIO2 or D4.
#define LED1 22
#define LED2 19
#define LED3 17

DHT11 dht11(4);

void setup() {
  // Initialize serial communication to allow debugging and data readout.
  // Using a baud rate of 9600 bps.
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  // Uncomment the line below to set a custom delay between sensor readings (in milliseconds).
  // dht11.setDelay(500); // Set this to the desired delay. Default is 500ms.
}

void loop() {
  int temperature = 0;
  int humidity = 0;

  // Attempt to read the temperature and humidity values from the DHT11 sensor.
  int result = dht11.readTemperatureHumidity(temperature, humidity);

  // Check the results of the readings.
  // If the reading is successful, print the temperature and humidity values.
  // If there are errors, print the appropriate error messages.
  if (result == 0) {
    digitalWrite(LED1, HIGH);
    Serial.print(temperature);
    Serial.print(',');
    Serial.println(humidity);
    if (temperature > 25){
      digitalWrite(LED2, HIGH);
    }else{
      digitalWrite(LED2, LOW);
    }

    if (humidity > 50){
      digitalWrite(LED3, HIGH);
    }else{
      digitalWrite(LED3, LOW);
    }
    // digitalWrite(LED1, HIGH);
    // digitalWrite(LED2, HIGH);
    // digitalWrite(LED3, HIGH);
  } else {
    // Print error message based on the error code.
    // Serial.println(DHT11::getErrorString(result));
    digitalWrite(LED1, LOW);
    // digitalWrite(LED2, LOW);
    // digitalWrite(LED3, LOW);
  }
}
