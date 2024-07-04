/* DESAIN PROJECT 
  MUHAMMAD ALFAN FUAD DZIA UL HAQ
  MUHAMMAD RIALDO FARIZKY
  RAIHAN FAKHAR ARIFIN
  ALTHAIRA ANJANI
*/

#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#define SOIL_PIN 15                          // ESP32 pin GPIO36 (ADC0) that connects to AOUT pin of moisture sensor
#define WATLEVEL_PIN 27                      // ESP32 pin GPIO36 (ADC0) connected to sensor's signal pin
#define RAIN_PIN 4
#define RELAY_PIN 14

// calculate debit water
const float literPerDetik = 0.0666666666666;  // Debit air dalam liter per detik
unsigned long startTime;  // Waktu awal penggunaan dalam milidetik
unsigned long endTime;    // Waktu akhir penggunaan dalam milidetik

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "id.pool.ntp.org", 25200, 60000);

const char *ssid     = "realmeC55";
const char *password = "12345678";

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//////////////////////////////////////////////////////////////////////////////////////////////////////

void setup(){
  Serial.begin(115200);

  pinMode(SOIL_PIN, INPUT);
  pinMode(WATLEVEL_PIN, INPUT);
  pinMode(RAIN_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() { 
  time(); 
  soil();
  rain();
  water_level();
  Serial.println("");

  delay(1000);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void soil(){              // Pembacaan Soil Sensor
  int soil_value = analogRead(SOIL_PIN);      
  int relayPin = digitalRead(RELAY_PIN);
  int persen_tanah = map(soil_value, 4095, 0, 0, 100);


  Serial.printf("Moisture value: %d,\tPersen tanah: %d \n",soil_value, persen_tanah);

  if (persen_tanah < 20) {
    digitalWrite(relayPin, HIGH);
  } else if (persen_tanah > 50) {
    digitalWrite(relayPin, LOW);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void rain(){              // Pembacaan rain sensor
  int rain_value = analogRead(RAIN_PIN);
  Serial.printf("Rain sensor value: %d\n", rain_value);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void water_level(){
  int value_water = analogRead(WATLEVEL_PIN);     
  int high_value_water = map(value_water, 0, 2200, 0, 4);

  Serial.printf("Water sensor value: %d,\thigh value: %d cm\n",value_water, high_value_water);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void time(){
  timeClient.update();
/*
  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);
*/

  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int currentSecond = timeClient.getSeconds();
  String weekDay = weekDays[timeClient.getDay()];

  Serial.printf("Day, Hour:Minutes:Seconds  ->  %s,\t%d:%d:%d \n", weekDay, currentHour, currentMinute, currentSecond);
  delay(1000);}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void relay(){
  int relayPin = digitalRead(RELAY_PIN);
  float water_out;
  float banyakAir;
  float banyakAir_new = 0.0;
  
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int currentSecond = timeClient.getSeconds();

  // Baca sensor untuk mendeteksi awal penggunaan
  if (relayPin == HIGH) {
    startTime = millis();  // Catat waktu awal penggunaan
    delay(1000);  // Tunggu satu detik agar tidak terbaca ganda
  }

  // Baca sensor untuk mendeteksi akhir penggunaan
  if (relayPin == LOW && startTime > 0) {
    endTime = millis();  // Catat waktu akhir penggunaan
    unsigned long penggunaanWaktu = endTime - startTime;  // Waktu penggunaan dalam milidetik
    banyakAir = (penggunaanWaktu / 1000.0) * literPerDetik;  // Hitung banyaknya air dalam liter
    water_out = banyakAir + banyakAir_new;
    banyakAir_new = banyakAir;

    // Tampilkan hasil pada monitor serial
    Serial.printf("Banyaknya Air yang Keluar: %.6f Liter", water_out);

    // Reset waktu awal untuk mendeteksi penggunaan selanjutnya
    startTime = 0;    
  }

  while(currentHour == 0 && currentMinute == 0 && currentSecond == 0){
    water_out = 0;
  }



}