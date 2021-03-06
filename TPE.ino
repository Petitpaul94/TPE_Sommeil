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
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };

//Leds pins
#define LED1 5
#define LED2 6
#define LED3 10
#define LED4 11

//Def température
#include "DHT.h"
#define DHTPIN 7 // broche ou l'on a branche le capteur
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);//déclaration du capteur

// Buzzer pins
#define BUZ 9

//Definition bluetooth
#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 8); // RX, TX

//Touch Sensor
#define TOUCH 13

int alarm = 2500;
int finalTime;
bool affichageEcran = HIGH;
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
  //Setup DHT11
  dht.begin();

  //Setup Horloge-Temps-Réel
#ifndef ESP8266
#endif
  rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime(2019, 1, 13, 17, 04, 0));

  //Setup Display_Screen
  display.setBrightness(7);
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
sortieAlarm:

  lumiereEcran();

  // Bluetooth
  if (mySerial.available()) {
    communicationBluetooth();
  }


  //Affichage de l'heure
  if (affichageEcran == HIGH) {
    display.showNumberDecEx(calculHeure(), (0b01000000), true);
  }

  //ALARM
  if (finalTime == alarm)
  {

    allumageProgressif();

    //Musique
    auClair(3, 700);
    if (testArretAlarm() == HIGH) {
      goto sortieAlarm;
    }
    delay(1400);
    if (testArretAlarm() == HIGH) {
      goto sortieAlarm;
    }
    auClair(3, 700);
    if (testArretAlarm() == HIGH) {
      goto sortieAlarm;
    }
    delay(1400);
    if (testArretAlarm() == HIGH) {
      goto sortieAlarm;
    }
    auClairSuite(2, 700);
    if (testArretAlarm() == HIGH) {
      goto sortieAlarm;
    }
    delay(1400);
    if (testArretAlarm() == HIGH) {
      goto sortieAlarm;
    }
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
    analogWrite(LED3, i);
    analogWrite(LED4, i);
    delay(500);
  }
  analogWrite(LED1, 0);
  analogWrite(LED2, 0);
  analogWrite(LED3, 0);
  analogWrite(LED4, 0);
}

//Touch Sensor test
void lumiereEcran() {
  int touchValue = digitalRead(TOUCH);
  if (touchValue == HIGH) {
    switch (affichageEcran) {
      case HIGH:
        display.setSegments(blank);
        break;
      case LOW:
        display.setSegments(data);
        break;
    }
    affichageEcran = !affichageEcran;
    analogWrite(LED1, 255);
    delay(1000);
    analogWrite(LED2, 255);
    delay(1000);
    analogWrite(LED3, 255);
    delay(1000);
    analogWrite(LED4, 255);
    delay(2500);
    analogWrite(LED1, 0);
    analogWrite(LED2, 0);
    analogWrite(LED3, 0);
    analogWrite(LED4, 0);
  }
}

void communicationBluetooth() {
  String lectureEnvoi = "";
  lectureEnvoi = mySerial.readString();
  alarm = lectureEnvoi.toInt();

  mySerial.write(dht.readTemperature()); //Envoi temp.
  mySerial.write(analogRead(A3)); //Envoi lum.
}

bool testArretAlarm() {
  int touchValue = digitalRead(TOUCH);
  if (touchValue == HIGH) {
    alarm = 2500;
    return 1 ;
  }
  else
  {
    return 0;
  }
}
