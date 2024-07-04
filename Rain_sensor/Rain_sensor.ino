/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-rain-sensor
 */

#define POWER_PIN 32  // ESP32's pin GPIO32 that provides the power to the rain sensor
#define AO_PIN 36     // ESP32's pin GPIO36 connected to AO pin of the rain sensor

void setup() {
  // initialize serial communication
  Serial.begin(9600);
  pinMode(POWER_PIN, OUTPUT);  // configure the power pin pin as an OUTPUT
}

void loop() {
  digitalWrite(POWER_PIN, HIGH);  // turn the rain sensor's power  ON
  delay(10);                      // wait 10 milliseconds

  int rain_value = analogRead(AO_PIN);
  digitalWrite(POWER_PIN, LOW);  // turn the rain sensor's power OFF

  Serial.println(rain_value);  // print out the analog value
  delay(1000);                 // pause for 1 sec to avoid reading sensors frequently to prolong the sensor lifetime
}
