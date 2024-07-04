#include <Wire.h>

uint8_t receivedNumber = 0;

void setup() {
 
  Wire.begin(8); // Inisialisasi I2C komunikasi dan tentukan alamat slave (alamat 8 pada contoh ini)
  Wire.onReceive(receiveEvent); // Atur fungsi yang akan dipanggil saat menerima data
  Serial.begin(115200);
}

void loop() {
  // Tidak ada yang perlu dilakukan di loop slave
}

// Fungsi ini akan dipanggil saat master mengirimkan data
void receiveEvent(int numBytes) {
if (numBytes >= sizeof(int)) {
    byte sourceID = Wire.read(); // Baca byte pertama untuk mengetahui sumber data (Master 1 atau Master 2)
    Wire.readBytes((uint8_t*)&receivedNumber, sizeof(receivedNumber)); // Baca byte array dan ubah menjadi angka integer
    
    if (sourceID == 1) {
      Serial.print("Data Soil Value: ");
    } else if (sourceID == 2) {
      Serial.print("Data Water Level: ");
    } else if (sourceID == 3) {
      Serial.print("Data Rain Value: ");
    } else {
      Serial.print("Unknown source data: ");
    }
    
    Serial.println(receivedNumber);
  }
}
