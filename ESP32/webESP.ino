#include <WiFi.h>
#include <WiFiClient.h> 
//#include <ESP32Websvr.h>
#include <HTTPClient.h>
#include <DHT.h>
#define DHT11_PIN  4 // ESP32 pin GPIO21 connected to DHT22 sensor
#define POWER_PIN1  17 // ESP32 pin GPIO17 connected to sensor's VCC pin
#define SIGNAL_PIN 35 // ESP32 pin GPIO36 (ADC0) connected to sensor's signal pin
#define POWER_PIN2 19  // ESP32's pin GPIO19 that provides the power to the rain sensor
//#define DO_PIN 21     // ESP32's pin GPIO21 connected to DO pin of the rain sensor
#define AO_PIN 36     // ESP32's pin GPIO36 connected to AO pin of the rain sensor
#define SENSOR_MIN 0
#define SENSOR_MAX 521

int value = 0; // variable to store the sensor value
int air = 0; // variable to store the water level
int dataKelembapan;
float dataSuhu;
String dataAir;
String dataCuaca;
DHT dht11(DHT11_PIN, DHT11);


const char* ssid = "RAKUTEN ATAS2";
const char* pass = "toyota1000";
const char* server = "http://192.168.0.101";

long zero = 0;
long jeda = 5000;

void setup() {
  Serial.begin(115200);
  dht11.begin(); // initialize the DHT22 sensor
  analogSetAttenuation(ADC_11db);
  pinMode(POWER_PIN1, OUTPUT);   // configure D7 pin as an OUTPUT
  digitalWrite(POWER_PIN1, LOW); // turn the sensor OFF
  pinMode(POWER_PIN2, OUTPUT);  // configure the power pin pin as an OUTPUT
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
   // read humidity
  dataKelembapan  = dht11.readHumidity();
  // read temperature in Celsius
  dataSuhu = dht11.readTemperature();
  

  // check whether the reading is successful or not
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

  // wait a 2 seconds between readings
  delay(100);
}

void waterlv() {
  digitalWrite(POWER_PIN1, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  value = analogRead(SIGNAL_PIN); // read the analog value from sensor
  digitalWrite(POWER_PIN1, LOW);   // turn the sensor OFF

  air = map(value, SENSOR_MIN, SENSOR_MAX, 0, 4); // 4 levels
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
  
  digitalWrite(POWER_PIN2, HIGH);  // turn the rain sensor's power  ON
  delay(10);                      // wait 10 milliseconds

  int cuaca = analogRead(AO_PIN);
  digitalWrite(POWER_PIN2, LOW);  // turn the rain sensor's power OFF
  
 //Serial.println(rain_value);
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