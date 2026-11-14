/************************************************************************************************* 
                                      PROGRAMMINFO
************************************************************************************************** 
Funktion: Greifarm ESP32 Bluetooth mit APP-Steuerung
BT-Verbinung mit "Roboterarm"
APP:
Motor 1 = GPIO 27
Motor 2 = GPIO 16
Motor 3 = GPIO 17
Motor 4 = GPIO 25
Motor 5 = GPIO 26
**************************************************************************************************
Version: 07.09.2022
**************************************************************************************************
Board: ESP32vn IoT UNO V1.0.4
**************************************************************************************************
C++ Arduino IDE V1.8.19
**************************************************************************************************
Einstellungen:
https://dl.espressif.com/dl/package_esp32_index.json
http://dan.drown.org/stm32duino/package_STM32duino_index.json
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
*************************************************************************************************
**************************************************************************************************/
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

//--------------------------------------------------------------
#define MOTORANZAHL 6        // Gesamtzahl der Servomotoren

#include "Servo.h"
Servo Motor[MOTORANZAHL];     // erstelle ein Objekt für jeden Motor

byte Motorpin[MOTORANZAHL] = {13, 27, 16, 17, 25, 26};
byte Motorposition[MOTORANZAHL];
byte aktuellerMotor = 0, Position = 0, EingabeWinkel = 0;
//--------------------------------------------------------------

void setup() {

  Serial.begin(9600);
  SerialBT.begin("Roboterarm"); //Bluetooth Device Name
  Serial.println("Roboterarm wurde gestartet!");

  for(byte i = 0; i < MOTORANZAHL; i++)
  {
    pinMode(Motorpin[i], OUTPUT);
    Motor[i].attach(Motorpin[i]);
  }
}

    

void loop()
{
  while(SerialBT.available()) {
    byte Eingabe = SerialBT.read();

    if(aktuellerMotor == 10)          // noch kein Motor gewählt
    {
      if(Eingabe >= '0' && Eingabe < MOTORANZAHL + '0')
      {
        aktuellerMotor = Eingabe - '0';
        continue;
      }
      if(Eingabe == 'i')
      {
        Serial.println("### aktuelle Positionswerte ###");
        for(byte i = 0; i < MOTORANZAHL; i++)
          Serial.println("Motor " + String(i) + ": " + String(Motor[i].read()));
        continue;
      }
    }

    if(aktuellerMotor < 10)           // Motor wurde bereits angewählt
    {
      if(Eingabe >= '0' && Eingabe <= '9')      // Eingabe ist eine Ziffer
      {
        EingabeWinkel = EingabeWinkel*5 + (Eingabe - '0');   //************ Steuerung in 5° Schritten ***********
        continue;
      }

      if(Eingabe == '+')              // Erhöhung des Winkels
      {
        if(EingabeWinkel == 0)
          EingabeWinkel = 1;
        Position = Motor[aktuellerMotor].read();
        if(180 - Position >= EingabeWinkel)
          Position += EingabeWinkel;
        else
          Position = 180;
        Motor[aktuellerMotor].write(Position);
        Serial.println("Motor " + String(aktuellerMotor) + " wurde auf Position " + String(Position) + " gestellt."); 
        EingabeWinkel = 0; aktuellerMotor = 10; 
        continue;
      } 
      if(Eingabe == '-')              // Verringerung des Winkels
      {
        if(EingabeWinkel == 0)
          EingabeWinkel = 1;    
        Position = Motor[aktuellerMotor].read();               
        if(Position >= EingabeWinkel)
          Position -= EingabeWinkel;
        else
          Position = 0;
        Motor[aktuellerMotor].write(Position);  
        Serial.println("Motor " + String(aktuellerMotor) + " wurde auf Position " + String(Position) + " gestellt.");  
        EingabeWinkel = 0; aktuellerMotor = 10;
        continue;
      }       
      
      if(EingabeWinkel <= 180)        // absoluter Eingabewinkel
      {
        Motor[aktuellerMotor].write(EingabeWinkel);  
        Serial.println("Motor " + String(aktuellerMotor) + " wurde auf Position " + String(EingabeWinkel) + " gestellt.");
        EingabeWinkel = 0; aktuellerMotor = 10;
        continue;
      }

      Serial.println("Ungueltige Eingabe!");
      EingabeWinkel = 0; aktuellerMotor = 10;   
    }
  }
}
