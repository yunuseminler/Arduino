/* Kütüphane Dosyaları */
#include "config.h"
#include "DHT.h"

/* Wifi ve Adafruit tanımlamaları */
AdafruitIO_Feed *button = io.feed("give_water");
AdafruitIO_Feed *toprakNemFeed = io.feed("toprakNem");
AdafruitIO_Feed *sicaklikFeed = io.feed("Sicaklik");
AdafruitIO_Feed *text = io.feed("text");


#define ToprakSensor A0
#define MotorLed D6
#define DHTPIN D2
#define Motor D1
#define Buzzer D5


#define DHTTYPE DHT11

DHT nemSensor(DHTPIN, DHTTYPE);
String buttonValue;

void setup() {
  
  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  button->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  nemSensor.begin();
  Serial.println(F("Kurulum Hazır"));
}

void loop() {
  io.run();
  if (buttonValue == "on"){
    MotorAc();
    text->save("Motor Açık.");
  }
  else if(buttonValue == "off"){
    MotorKapa();
    text->save("Motor Kapalı.");
    sensorOku();
  }
}

void sensorOku(){
  delay(10000);
  float Nem = nemSensor.readHumidity();
  float Sicaklik = nemSensor.readTemperature();
  float ToprakNem = toprakNemHesapla(analogRead(ToprakSensor));

  serialMonitor(Nem, Sicaklik, ToprakNem);
  sicaklikFeed->save(Sicaklik/100000);
  toprakNemFeed->save(ToprakNem);
  

  if ( ToprakNem < 40) {
    MotorAc();
    delay(10000);
    MotorKapa();
  }
  else if(ToprakNem < 60 && ToprakNem > 40 ){ 
    MotorAc();
    delay(6000);
    MotorKapa();
  }
  
}

void MotorAc(){
  analogWrite(MotorLed, 255); 
  digitalWrite(Motor, HIGH);
  analogWrite(Buzzer, 255);
  Serial.println("Motor Çalıştırıldı.");
  
}

void MotorKapa(){
  analogWrite(Motor, 0);
  digitalWrite(MotorLed, LOW);
  analogWrite(Buzzer, 0);
  Serial.println("Motor Kapatıldı.");
}

float toprakNemHesapla(float analogDeger){
  //çok ıslak -> 200-300
  //ideal     -> 500-700
  //kuru      -> 800
  
  if(800 < analogDeger){
    analogDeger = 800;
  }else if(analogDeger < 200){
    analogDeger = 200;
  }
  
  return ((1000-analogDeger)/800)*100; 
}

void serialMonitor(float Nem,float Sicaklik,float ToprakNem){
  Serial.print("\n--------------------\nToprak Nem:   ");
  Serial.print(ToprakNem);
  Serial.print("\nHava Nem:     %");
  Serial.print(Nem);
  Serial.print("\nSıcaklık:    " );
  Serial.print(Sicaklik);
  Serial.print("\n--------------------\n\n~~~~~~~~~~~~~~~~~~~~\n");
}

void handleMessage(AdafruitIO_Data *data) {
  buttonValue= data->value();
}
