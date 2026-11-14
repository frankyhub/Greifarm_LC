
/************************************************************************************************* 
                                      PROGRAMMINFO
************************************************************************************************** 
Funktion: Servo Motor Steuerung mit Poti und OLED Display 
**************************************************************************************************
Version: 06.01.2022
**************************************************************************************************
Board: ESP32vn IoT UNO V1.0.4
**************************************************************************************************
C++ Arduino IDE V1.8.13
**************************************************************************************************
Einstellungen:
https://dl.espressif.com/dl/package_esp32_index.json
http://dan.drown.org/stm32duino/package_STM32duino_index.json
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
**************************************************************************************************/
#include <Servo.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


unsigned long delayTime;

Servo myservo27;//create servo object to control a servo
Servo myservo16;//create servo object to control a servo
Servo myservo17;//create servo object to control a servo

int val1=0;//Read potentiometer 0 data
int val2=0;//Read potentiometer 1 data
int val3=0;//Read potentiometer 2 data

void setup()
{
  u8g2.begin();
  Serial.begin(115200);

  myservo27.attach(27);
  myservo16.attach(16);
  myservo17.attach(17);
}

void loop()
{  
    val1 = map(analogRead(34), 0, 2048, 0, 90);  
    val2 = map(analogRead(36), 0, 2048, 0, 90);  
    val3 = map(analogRead(39), 0, 2048, 0, 90);  

    Serial.print("A34:");Serial.println(val1);
    Serial.print("A36:");Serial.println(val2);
    Serial.print("A39:");Serial.println(val3);    
;
    Serial.println("************");
    delay(100);

  myservo27.write(val1);//
  myservo16.write(val2);//
  myservo17.write(val3);//  
  delay(100);//wait 

    u8g2.setFont(u8g2_font_courR10_tf);
    u8g2.firstPage();

    do {
      
      u8g2.setCursor(18, 10);
      u8g2.print("MakerMonday");      
      u8g2.setCursor(8, 22);
      u8g2.print("Servo-Control");
       
      u8g2.setCursor(2, 35); 
      u8g2.print("Servo 1: ");
      u8g2.setCursor(80, 35);
      u8g2.print(val1);
      u8g2.print("\xB0");
  
      u8g2.setCursor(2, 48); 
      u8g2.print("Servo 2:");
      u8g2.setCursor(80, 48);
      u8g2.print(val2);
      u8g2.print("\xB0");

      
      u8g2.setCursor(2, 63); 
      u8g2.print("Servo 3:");
      u8g2.setCursor(80, 63);
      u8g2.print(val3);
      u8g2.print("\xB0");     

      } while ( u8g2.nextPage() );

}
















  
