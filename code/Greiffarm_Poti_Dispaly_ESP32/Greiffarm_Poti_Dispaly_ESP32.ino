/************************************************************************************************* 
                                      PROGRAMMINFO
************************************************************************************************** 
Funktion: Greifarm mit 5 Servos, 5 Potis und einem I2C OLED 1,3" Display
**************************************************************************************************
Version: 15.11.2026
**************************************************************************************************
Board: ESP32vn IoT UNO V1.0.6
**************************************************************************************************
C++ Arduino IDE V1.8.13
**************************************************************************************************
Einstellungen:
https://dl.espressif.com/dl/package_esp32_index.json
http://dan.drown.org/stm32duino/package_STM32duino_index.json
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json 
 **************************************************************************************************/

#include <Arduino.h>
#include <U8g2lib.h>
#include <ESP32Servo.h> //V3.2.1 https://www.arduinolibraries.info/libraries/esp32-servo

// create four servo objects 
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

int delayTime =50;

// Possible PWM GPIO pins on the ESP32: 0(used by on-board button),2,4,5(used by on-board LED),12-19,21-23,25-27,32-33 
int servoPin1 = 12;      // GPIO pin used to connect the servo control (digital out)
int servoPin2 = 14;
int servoPin3 = 18;
int servoPin4 = 19;
int servoPin5 = 23;

// Possible ADC pins on the ESP32: 0,2,4,12-15,32-39; 34-39 are recommended for analog input
int potPin1 = 4;        // GPIO pin used to connect the potentiometer (analog in)
int potPin2 = 35;        // GPIO pin used to connect the potentiometer (analog in)
int potPin3 = 34;        // GPIO pin used to connect the potentiometer (analog in)
int potPin4 = 36;        // GPIO pin used to connect the potentiometer (analog in)
int potPin5 = 39;        // GPIO pin used to connect the potentiometer (analog in)

int ADC_Max = 4096;     // This is the default ADC max value on the ESP32 (12 bit ADC width);
                        // this width can be set (in low-level oode) from 9-12 bits, for a
                        // a range of max values of 512-4096
  
int val1=0;//Read potentiometer 0 data
int val2=0;//Read potentiometer 0 data
int val3=0;//Read potentiometer 0 data
int val4=0;//Read potentiometer 0 data
int val5=0;//Read potentiometer 0 data




#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//unsigned long delayTime;

void setup() {
    Serial.begin(9600);
    u8g2.begin();

  servo1.attach(servoPin1, 500, 2400);   // attaches the servo on pin 18 to the servo object
                                         // using SG90 servo min/max of 500us and 2400us
                                         // for MG995 large servo, use 1000us and 2000us,
                                         // which are the defaults, so this line could be
                                         // "myservo.attach(servoPin);"
                                         
  servo2.attach(servoPin2, 500, 2400);   // attaches the servo on pin 18 to the servo object
                                         // using SG90 servo min/max of 500us and 2400us
                                         // for MG995 large servo, use 1000us and 2000us,
                                         // which are the defaults, so this line could be
                                         // "myservo.attach(servoPin);"
                                         
  servo3.attach(servoPin3, 500, 2400);   // attaches the servo on pin 18 to the servo object
                                         // using SG90 servo min/max of 500us and 2400us
                                         // for MG995 large servo, use 1000us and 2000us,
                                         // which are the defaults, so this line could be
                                         // "myservo.attach(servoPin);"

  servo4.attach(servoPin4, 500, 2400);   // attaches the servo on pin 18 to the servo object
                                         // using SG90 servo min/max of 500us and 2400us
                                         // for MG995 large servo, use 1000us and 2000us,
                                         // which are the defaults, so this line could be
                                         // "myservo.attach(servoPin);"

  servo5.attach(servoPin5, 500, 2400);   // attaches the servo on pin 18 to the servo object
                                         // using SG90 servo min/max of 500us and 2400us
                                         // for MG995 large servo, use 1000us and 2000us,
                                         // which are the defaults, so this line could be
                                         // "myservo.attach(servoPin);"                                         
  }
    

void loop() { 



  val1 = analogRead(potPin1);            // read the value of the potentiometer (value between 0 and 1023)
  val1 = map(val1, 0, ADC_Max, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  servo1.write(val1);                  // set the servo position according to the scaled value
  Serial.print("val1: ");
  Serial.println(val1);
  delay(delayTime);                          // wait for the servo to get there


  val2 = analogRead(potPin2);            // read the value of the potentiometer (value between 0 and 1023)
  val2 = map(val2, 0, ADC_Max, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  servo2.write(val2);                  // set the servo position according to the scaled value
  Serial.print("val2: ");
  Serial.println(val2);  
  delay(delayTime);

  
  val3 = analogRead(potPin3);            // read the value of the potentiometer (value between 0 and 1023)
  val3 = map(val3, 0, ADC_Max, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  servo3.write(val3);                  // set the servo position according to the scaled value
  Serial.print("val3: ");
  Serial.println(val3);  
  delay(delayTime);

  
  val4 = analogRead(potPin4);            // read the value of the potentiometer (value between 0 and 1023)
  val4 = map(val4, 0, ADC_Max, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  servo4.write(val4);                  // set the servo position according to the scaled value
  Serial.print("val4: ");
  Serial.println("val4");  
  delay(delayTime);

  
  val5 = analogRead(potPin5);            // read the value of the potentiometer (value between 0 and 1023)
  val5 = map(val5, 0, ADC_Max, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  servo5.write(val5);                  // set the servo position according to the scaled value
  Serial.print("val5: ");
  Serial.println(val5);  
  delay(delayTime);
  
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
