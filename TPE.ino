// Définition Horloge-Temps-Réel
#include <Wire.h>
#include "RTClib.h"
RTC_DS3231 rtc;

// Définition Display_Screen
#include <Arduino.h>
#include <TM1637Display.h>
#define CLK 2
#define DIO 3
TM1637Display display(CLK, DIO);

//Leds pins
#define LED1 5
#define LED2 6

//Def température
#include <dht.h>
dht DHT;
#define DHTPIN 7

// Buzzer pins
#define BUZ 9

//Definition bluetooth
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

//Touch Sensor
#define TOUCH 13

int alarm = 2500;
int finalTime;
int affichageEcran = 7;
// Notes de musique
#define DON 65
#define DOD 69
#define REN 74
#define RED 78
#define MIN 83
#define FAN 87
#define FAD 93
#define SON 98
#define SOD 104
#define LAN 110
#define LAD 117
#define SIN 123

void setup () {
  //Setup Horloge-Temps-Réel
#ifndef ESP8266
#endif
  rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime(2019, 1, 13, 17, 04, 0));

  //Setup Display_Screen
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
  display.setBrightness(affichageEcran);
  display.setSegments(data);

  // Buzzer
  pinMode(BUZ, OUTPUT);

  //Touch
  pinMode(TOUCH, INPUT);

  //Bluetooth
  mySerial.begin(9600);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect.
  }
}

void loop () {

  lumiereEcran();

  // Bluetooth
  if (mySerial.available()) {
    communicationBluetooth();
  }


  //Affichage de l'heure
  display.showNumberDecEx(calculHeure(), (0b01000000), true);

  //ALARM
  if (finalTime == alarm)
  {

    allumageProgressif();
    //Musique
    auClair(3, 700);
    delay(1400);
    auClair(3, 700);
    delay(1400);
    auClairSuite(2, 700);
    delay(1400);
    auClair(3, 700);

    // init
    alarm = 2500;

  }

}

//Fonction musique partie 1
void auClair(float octave, int noire)
{
  tone(BUZ, DON * pow(2, octave), noire);

  delay(noire);
  noTone(BUZ);
  tone(BUZ, DON * pow(2, octave), noire);

  delay(noire);
  noTone(BUZ);
  tone(BUZ, DON * pow(2, octave), noire);

  delay(noire);
  tone(BUZ, REN * pow(2, octave), noire);

  delay(noire);
  tone(BUZ, MIN * pow(2, octave), noire * 2);

  delay(noire * 2);
  tone(BUZ, REN * pow(2, octave), noire * 2);

  delay(noire * 2);
  tone(BUZ, DON * pow(2, octave), noire);

  delay(noire);
  tone(BUZ, MIN * pow(2, octave), noire);

  delay(noire);
  tone(BUZ, REN * pow(2, octave), noire);

  delay(noire);
  noTone(BUZ);
  tone(BUZ, REN * pow(2, octave), noire);

  delay(noire);
  tone(BUZ, DON * pow(2, octave), noire * 2);

  delay(noire * 2);
}

// Fonction  musique partie 2
void auClairSuite(float octave, int noire)
{
  tone(BUZ, REN * pow(2, octave + 1), noire);
  delay(noire);
  noTone(BUZ);
  tone(BUZ, REN * pow(2, octave + 1), noire);
  delay(noire);
  noTone(BUZ);
  tone(BUZ, REN * pow(2, octave + 1), noire);
  delay(noire);
  noTone(BUZ);
  tone(BUZ, REN * pow(2, octave + 1), noire);
  delay(noire);

  tone(BUZ, LAN * pow(2, octave), noire * 2);
  delay(noire * 2);
  noTone(BUZ);
  tone(BUZ, LAN * pow(2, octave), noire * 2);
  delay(noire * 2);
  tone(BUZ, REN * pow(2, octave + 1), noire);
  delay(noire);
  tone(BUZ, DON * pow(2, octave + 1), noire);
  delay(noire);
  tone(BUZ, SIN * pow(2, octave), noire);
  delay(noire);
  tone(BUZ, LAN * pow(2, octave), noire);
  delay(noire);
  tone(BUZ, SON * pow(2, octave + 1), noire * 2);
  delay(noire * 2);
}

// Calcul de l'heure
int calculHeure()
{
  DateTime now = rtc.now();
  byte finalHour = now.hour();
  byte finalMinute = now.minute();
  finalTime = 100 * finalHour + finalMinute;
  return finalTime;
}

// Allumage progressif
void allumageProgressif() {

  for (int i = 0; i <= 255; i = i + 5) {
    analogWrite(LED1, i);
    analogWrite(LED2, i);
    delay(500);
  }
  analogWrite(LED1, 0);
  analogWrite(LED2, 0);
}

//Touch Sensor test
void lumiereEcran() {
  int touchValue = digitalRead(TOUCH);
  if (touchValue == HIGH) {
    switch (affichageEcran) {
      case 7:
        display.setBrightness(0);
        break;
      case 0:
        display.setBrightness(7);
        break;
    }
    analogWrite(LED1, 255);
    analogWrite(LED2, 255);
    delay(3000);
  }
}

void communicationBluetooth(){
    alarm = (mySerial.read());
    
    int tempRead = DHT.read11(DHTPIN);
    mySerial.write(DHT.temperature); //Envoi temp.
    mySerial.write(analogRead(A0)); //Envoi lum.
}
