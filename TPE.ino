#include <SoftwareSerial.h>

// Définition Horloge-Temps-Réel
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Définition Display_Screen
#include <Arduino.h>
#include <TM1637Display.h>

// Afficheur connection pins (Digital Pins)
#define CLK 2
#define DIO 3

// Buzzer pins
#define BUZ 9

//Leds pins
#define LED1 5
#define LED2 6

//Touch Sensor
#define TOUCH 13
TM1637Display display(CLK, DIO);

// Heure de réveil
int alarm = 2500;

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

//Definition bluetooth
SoftwareSerial mySerial(10, 11); // RX, TX

void setup () {
  //Setup Horloge-Temps-Réel
#ifndef ESP8266
#endif
  rtc.begin();
  // following line sets the RTC to the date & time this sketch was compiled
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime(2019, 1, 13, 17, 04, 0));

  Serial.begin(9600);
  //Setup Display_Screen
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
  display.setBrightness(0x0f);

  // All segments on
  display.setSegments(data);
  delay(2000);
  // Buzzer output
  pinMode(BUZ, OUTPUT);
  //Touch input
  pinMode(TOUCH, INPUT);

  Serial.begin(9600);
  mySerial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop () {
  //Touch Sensor test
  int touchValue = digitalRead(TOUCH);

  // Bluetooth
  if (mySerial.available())
  {
    Serial.write(mySerial.read());
    alarm = (mySerial.read());
  }
  if (Serial.available())
    mySerial.write(Serial.read());

  //Récupération de l'heure
  DateTime now = rtc.now();
  int finalHour = now.hour();
  int finalMinute = now.minute();
  int finalTime = 100 * finalHour + finalMinute;

  //Affichage de l'heure
  display.showNumberDecEx(finalTime, (0b01000000), true);
  delay(1000);

  //ALARM

  if (finalTime == alarm || touchValue == HIGH)
  {

    for (int i=0; i <= 255; i=i+5){

      // impulsion 
      analogWrite(LED1, i);
      analogWrite(LED2, i);
      delay(1000);
      }
      analogWrite(LED1, 0);
      analogWrite(LED2, 0);

    auClair(3, 700);
    delay(1400);
    auClair(3, 700);
    delay(1400);
    auClairSuite(2, 700);
    delay(1400);
    auClair(3, 700);

    alarm = 2500;
    touchValue = LOW;
  }

}

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
