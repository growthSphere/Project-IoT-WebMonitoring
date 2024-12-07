#include <WiFi.h>
#include <WiFiClient.h> 
#include <HTTPClient.h>
#include <DHT.h>
#define DHT11_PIN  4 // Pin DHT11
#define POWER_PIN1  17 // Pin 17 esp ke vcc waterlevel sensor
#define SIGNAL_PIN 35 // Pin 35 esp ke sinyal waterlevel sensor
#define POWER_PIN2 19  // Pin 19 esp ke vcc rain sensor

#define AO_PIN 36     // Pin 36 esp ke analog rain sensor
#define SENSOR_MIN 0
#define SENSOR_MAX 521

int value = 0; // nilai waterlevel sensor
int air = 0; // variable untuk menentukan level air
int dataKelembapan;
float dataSuhu;
String dataAir;
String dataCuaca;
DHT dht11(DHT11_PIN, DHT11);


const char* ssid = "ssid";
const char* pass = "pass";
const char* server = "alamat ipv4";

long zero = 0;
long jeda = 5000;

void setup() {
  Serial.begin(115200);
  dht11.begin(); // inisialisasi sensor DHT11
  analogSetAttenuation(ADC_11db);
  pinMode(POWER_PIN1, OUTPUT);   
  digitalWrite(POWER_PIN1, LOW); 
  pinMode(POWER_PIN2, OUTPUT);  
  //WIFI KONEKSI  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
Serial.println("TERHUBUNG");

}

String urlEncode(String str) {
  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (isalnum(c)) {
      encodedString += c;
    } else {
      code0 = (c >> 4) & 0xF;
      code1 = (c & 0xF);
      encodedString += '%';
      encodedString += String("0123456789ABCDEF").charAt(code0);
      encodedString += String("0123456789ABCDEF").charAt(code1);
    }
  }
  return encodedString;
}

void dhtsensor(){
   // baca kelembapan
  dataKelembapan  = dht11.readHumidity();
  // baca suhu
  dataSuhu = dht11.readTemperature();
  

  // cek kondisi sensor
  if ( isnan(dataSuhu) ||  isnan(dataKelembapan)) {
    Serial.println("Gagal membaca sensor!");
  } 
  else {
    Serial.print("Kelembapan: ");
    Serial.print(dataKelembapan);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Suhu: ");
    Serial.print(dataSuhu);
    Serial.println("°C  ~  ");
  }

  // delay 100ms
  delay(100);
}

void waterlv() {
  digitalWrite(POWER_PIN1, HIGH);  
  delay(10);                      
  value = analogRead(SIGNAL_PIN); 
  digitalWrite(POWER_PIN1, LOW);   

  air = map(value, SENSOR_MIN, SENSOR_MAX, 0, 4); // 4 level
  Serial.print("Water level: ");
  Serial.println(air);
  
  if (air > 7){
    dataAir = "Bahaya";
    Serial.println("Bahaya");
  }
  else if (air > 5 && air <= 7){
    dataAir = "Waspada";
    Serial.println("Waspada");
  }
  else {
    dataAir = "Normal";
    Serial.println("Normal");
  }
  delay(100);
}

void rain() {
  
  digitalWrite(POWER_PIN2, HIGH);  
  delay(10);                      

  int cuaca = analogRead(AO_PIN);
  digitalWrite(POWER_PIN2, LOW);  
  
 //Serial.println(cuaca);
  if (cuaca >=4000){
    dataCuaca = "Tidak Hujan";
    Serial.println("Tidak Hujan");
  }
  else{
    dataCuaca = "Hujan";
    Serial.println("Hujan");
  }
  delay(100);
}

void loop() {
  Serial.println("-----------------------");
  waterlv();
  dhtsensor();
  rain();
  delay(1000);
  if (millis ()- zero > jeda){
     String URL = String("") + server + "/MonitoringBanjir/input.php?air=" + urlEncode(dataAir) +
               "&suhu=" + dataSuhu +
               "&kelembapan=" + dataKelembapan +
               "&cuaca=" + urlEncode(dataCuaca);
    Serial.println(URL);

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(URL);
      int httpCode = http.GET();
      if (httpCode > 0) {
        String payload = http.getString();
        Serial.println(payload);
      }
      http.end();
    }
  }
}