/*************************************************************************************************
                                      PROGRAMMINFO
**************************************************************************************************
  Funktion: Greifarm mit 5 Servos, 5 Potis, 1,3" OLED Display
**************************************************************************************************
  Version: 19.11.2026
**************************************************************************************************
  Board: ESP32vn IoT UNO V3.3.12
**************************************************************************************************
  C++ Arduino IDE V1.8.13
**************************************************************************************************
  Einstellungen:
  https://dl.espressif.com/dl/package_esp32_index.json
  http://dan.drown.org/stm32duino/package_STM32duino_index.json
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
 **************************************************************************************************/

#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#include <RoboServo.h> //V1.2.0 https://github.com/dunknowcoding/RoboServo

int delayTime = 150;

// erstelle fünf Servo-Objekte
RoboServo servo1;
RoboServo servo2;
RoboServo servo3;
RoboServo servo4;
RoboServo servo5;


// Mögliche PWM-GPIO-Pins am ESP32: 0 , 2, 4, 5 (vom Onboard-LED verwendet), 12-19, 21-23, 25-27, 32-33
const int servoPin1 = 12;
const int servoPin2 = 14;
const int servoPin3 = 18;
const int servoPin4 = 19;
const int servoPin5 = 23;

// Mögliche ADC-Pins am ESP32: 0,2,4,12-15,32-39; 34-39 werden für analoge Eingänge empfohlen
int potPin1 = 4;   // Potentiometer GPIO-Pin (analog in)
int potPin2 = 35;
int potPin3 = 34;
int potPin4 = 36;
int potPin5 = 39;

int ADC_Max = 4096;     // Dies ist der Standard-ADC-Maximalwert auf dem ESP32 (12-Bit-ADC-Breite);

int val1 = 0; //Daten des Potentiometers 1
int val2 = 0; //Daten des Potentiometers 2
int val3 = 0; //Daten des Potentiometers 3
int val4 = 0; //Daten des Potentiometers 4
int val5 = 0; //Daten des Potentiometers 5


void setup() {
  Serial.begin(115200);
  u8g2.begin();

  if (servo1.attach(servoPin1) == ROBOSERVO_INVALID_SERVO) {
    Serial.println("Servomotor ist nicht angeschlossen!");
    while (1) delay(1000);
  }

  Serial.println("Servo ist angeschlossen. Starte das Programm...");
  servo1.write(90);  // Beginne in der Mitte
  delay(500);

  servo2.attach(servoPin2, 500, 2400);   //Verwende den SG90-Servos mit Min/Max von 500 µs und 2400 µs

  servo3.attach(servoPin3, 500, 2400);

  servo4.attach(servoPin4, 500, 2400);

  servo5.attach(servoPin5, 500, 2400);

}

void loop() {

  val1 = analogRead(potPin1);               // den Wert des Potentiometers auslesen (Wert zwischen 0 und 1023)
  val1 = map(val1, 0, ADC_Max, 10, 170);     // Skaliere es, um es mit dem Servo zu verwenden (Wert zwischen 0 und 180)
  servo1.write(val1);                       // Stelle die Servoposition entsprechend dem skalierten Wert ein
  Serial.print("val1: ");
  Serial.println(val1);
  delay(delayTime);                          // warte, bis der Servo dort ist

  val2 = analogRead(potPin2);
  val2 = map(val2, 0, ADC_Max, 70, 150);
  servo2.write(val2);
  Serial.print("val2: ");
  Serial.println(val2);
  delay(delayTime);

  val3 = analogRead(potPin3);
  val3 = map(val3, 0, ADC_Max, 30, 150);
  servo3.write(val3);
  Serial.print("val3: ");
  Serial.println(val3);
  delay(delayTime);

  val4 = analogRead(potPin4);
  val4 = map(val4, 0, ADC_Max, 30, 150);
  servo4.write(val4);
  Serial.print("val4: ");
  Serial.println("val4");
  delay(delayTime);

  val5 = analogRead(potPin5);
  val5 = map(val5, 0, ADC_Max, 5, 170);
  servo5.write(val5);
  Serial.print("val5: ");
  Serial.println(val5);

  u8g2.setFont(u8g2_font_courR10_tf);
  u8g2.firstPage();

  do {

    u8g2.setCursor(2, 10);
    u8g2.print("Servo 1: ");
    u8g2.print(val1);
    u8g2.print("\xB0");

    u8g2.setCursor(2, 22);
    u8g2.print("Servo 2: ");
    u8g2.print(val2);
    u8g2.print("\xB0");

    u8g2.setCursor(2, 35);
    u8g2.print("Servo 3: ");
    u8g2.setCursor(80, 35);
    u8g2.print(val3);
    u8g2.print("\xB0");

    u8g2.setCursor(2, 48);
    u8g2.print("Servo 4: ");
    u8g2.print(val4);
    u8g2.print("\xB0");

    u8g2.setCursor(2, 63);
    u8g2.print("Servo 5: ");
    u8g2.print(val5);
    u8g2.print("\xB0");


  } while ( u8g2.nextPage() );


  delay(delayTime);
}
