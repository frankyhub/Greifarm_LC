/*************************************************************************************************
                                      PROGRAMMINFO
**************************************************************************************************
Funktion: Greifarm mit Touch Display
          Automatik- und Manueller Modus, 5 Positionen speichen, auto-run
**************************************************************************************************
Version: 20.11.2022 Test mit Greifarm incl. automatik Modus
**************************************************************************************************
Board: Arduino Mega
Hailege 2.4" ILI9341 240X320 TFT LCD Display with Touch Panel LCD
**************************************************************************************************
Libraries:
https://github.com/espressif/arduino-esp32/tree/master/libraries
C:\Users\User\Documents\Arduino
D:\gittemp\Arduino II\A156_Wetterdaten_V3
**************************************************************************************************
C++ Arduino IDE V1.8.19

**************************************************************************************************
Einstellungen:
https://dl.espressif.com/dl/package_esp32_index.json
http://dan.drown.org/stm32duino/package_STM32duino_index.json
http://arduino.esp8266.com/stable/package_esp8266com_index.json


Joystick:

X1: A8 of Arduino Mega
Y1: A9 of Arduino Mega
X2: A10 of Arduino Mega
Y2: A11 of Arduino Mega
GND: GND of Arduino Mega
VCC: +5V of Arduino Mega

Touch Display:
Shield für Arduino MEGA

Reset           Reset-pin SPI-header
VCC             VCC-pin   SPI-header
GND             GND-pin   SPI-header
SCK             SCK-pin   SPI-header
SDI (MOSI)      MOSI-pin  SPI-header
SDO (MISO)      MOSI-pin  SPI-header
LED             3.3V
CS              Any Digital pin
DC              Any Digital pin

T-CLK           Any Digital pin
T_CS            Any Digital pin
T_DIN           Any Digital pin
T_DO            Any Digital pin
T_IRQ           Any Digital pin

SD_CS           Any Digital pin
SD_MOSI         MOSI-pin  SPI-header 
SD_MISO         MISO-pin  SPI-header 
SD_SCK          SCK-pin  SPI-header



Servo Motore:
S1 OUT: 22
S2 OUT: 24 
S3 OUT: 26 
S4 OUT: 28 
GND: GND 
VCC: +5V 


**************************************************************************************************/

#include <Servo.h> 

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

//pressure for tft touch display
#define MINPRESSURE 200
#define MAXPRESSURE 1000

String page;

boolean cs1 = false; 
boolean cs2 = false; 
boolean cs3 = false; 
boolean cs4 = false; 
boolean cs5 = false; 

int ssn = 0;

MCUFRIEND_kbv tft;

Servo s1;
Servo s2;
Servo s3;
Servo s4;

#define j1x A8
#define j1y A9
#define j2x A10
#define j2y A11

int x_out = 0;
int y_out = 0;
int x_out2 = 0;
int y_out2 = 0;

int button1Pressed = 0;
boolean button2Pressed = false;
boolean mode = false;

int servo1PosSave[]={90,90,90,90,90};
int servo2PosSave[]={90,90,90,90,90};
int servo3PosSave[]={90,90,90,90,90};
int servo4PosSave[]={90,90,90,90,90};

const int XP = 8, XM = A2, YP = A3, YM = 9; //240x320 ID = 0x9341

const int TS_LEFT = 142, TS_RT = 892, TS_TOP = 937, TS_BOT = 187;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button manual_btn, auto_btn, backmanual_btn, backauto_btn, backrecord_btn, backplay_btn, record_btn, play_btn, save_btn;

int pixel_x, pixel_y;     
bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.y, TS_TOP, TS_BOT, 0, 320); //Bildschirmauflösung: 320x240
        pixel_y = map(p.x, TS_RT, TS_LEFT, 0, 240);
    }
    return pressed;
}

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup() {
  s1.attach(22);
  s2.attach(24);
  s3.attach(26);
  s4.attach(28);

  s1.write(90);
  s2.write(90);
  s3.write(90);
  s4.write(90);

  pinMode(j1x, INPUT);
  pinMode(j1y, INPUT);
  pinMode(j2x, INPUT);
  pinMode(j2y, INPUT);

  Serial.begin(115200);

  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  Serial.println("Calibrate for your Touch Panel");
  
  tft.begin(ID);
  tft.setRotation(1);            //LANDSCAPE - 1; PORTRAIT - 0
  tft.fillScreen(BLACK); 

  page = "home";

}

void loop(){

  if (page == "home"){
    chooseModePage();
  } else if (page == "manual"){
    manualModePage();
  } else if (page == "automated"){
    automatedModePage(); 
  } else if (page == "record"){
    recordPage();
  } else if (page == "play"){
    playPage();
  }
    
}

void chooseModePage(){

  cs2 = false;
  cs3 = false;
  cs4 = false;
  cs5 = false;
  ssn = 0;

  if(cs1 == false){
    tft.fillScreen(BLACK);
    manual_btn.initButton(&tft,  160, 120, 250, 55, WHITE, CYAN, BLACK, "Manuell", 3); 
    auto_btn.initButton(&tft, 160, 190, 250, 55, WHITE, CYAN, BLACK, "Automatik", 3);  
    manual_btn.drawButton(false);
    auto_btn.drawButton(false);
    tft.setFont(&FreeSansBold18pt7b);
    tft.setCursor(20, 55);                  //x,y 75,55
    tft.setTextColor(WHITE);                //color
    tft.setTextSize(1);                     //Eg., 1,2,3
    tft.print("Greifarm-Control");                 //Text
    cs1 = true;
  }

  bool down = Touch_getXY();
    manual_btn.press(down && manual_btn.contains(pixel_x, pixel_y));
    auto_btn.press(down && auto_btn.contains(pixel_x, pixel_y));
    if (manual_btn.justReleased()){
        tft.setFont();
        manual_btn.drawButton(false);
        Serial.println("a");
        page = "manual";
    }        
    if (auto_btn.justReleased()){
        tft.setFont();
        auto_btn.drawButton(false);
        Serial.println("b");
        page = "automated";
    }
    if (manual_btn.justPressed()) {
        tft.setFont();
        manual_btn.drawButton(true);
        Serial.println("c");
    }
    if (auto_btn.justPressed()) {
        tft.setFont();
        auto_btn.drawButton(true);
        Serial.println("d");
    }  

}
  
void manualModePage(){

  Serial.print(analogRead(j1x));
  Serial.print("\t");
  Serial.print(analogRead(j2x));
  Serial.print("\t");
  Serial.print(analogRead(j1y));
  Serial.print("\t");
  Serial.println(analogRead(j2y));

  cs1 = false;
  cs3 = false;
  cs4 = false;
  cs5 = false;
  ssn = 0;

  if (cs2 == false){
    tft.fillScreen(BLACK);
    backmanual_btn.initButton(&tft,  160, 190, 250, 55, WHITE, CYAN, BLACK, "zurueck", 3);  
    backmanual_btn.drawButton(false);
    tft.setFont(&FreeSansBold18pt7b);
    tft.setCursor(50, 50);                    //x,y
    tft.setTextColor(CYAN);                  //color
    tft.setTextSize(1);                       //Eg., 1,2,3
    tft.print("Man. Modus");                 //Your Text
    tft.setFont(&FreeSansBold12pt7b);
    tft.setCursor(20, 115);                    //x,y
    tft.setTextColor(WHITE);                  //color
    tft.setTextSize(1);                       //Eg., 1,2,3
    tft.print("Verwende die Joysticks!");      //Your Text
    cs2 = true;
  }

  bool down = Touch_getXY();
  backmanual_btn.press(down && backmanual_btn.contains(pixel_x, pixel_y));
  if (backmanual_btn.justReleased()){
      tft.setFont();
      backmanual_btn.drawButton(false);
      Serial.println("a");
      page = "home";
  }        
  if (backmanual_btn.justPressed()) {
      tft.setFont();
      backmanual_btn.drawButton(true);
      Serial.println("c");
  }

  //-------------------------------Manual Joystick Control-------------------------------------------------------------------------------------------------------------

  if(analogRead(j1x) >= 800){
     for (int i = s1.read(); i < 170; i++){ //Drehteller
      if (analogRead(j1x) < 800){
        break;
      }
      s1.write(i);
      delay(20);
     }
  }

  else if(analogRead(j1x) <= 300){
     for (int i = s1.read(); i > 10; i--){ //Drehteller
      if (analogRead(j1x) > 300){
        break;
      }
      s1.write(i);
      delay(20);
     }
  }
//-------------------------------------------
  if(analogRead(j1y) >= 800){
     for (int i = s2.read(); i < 160; i++){ //140 Gelenk unten Pos unten
      if (analogRead(j1y) < 800){
        break;
      }
      s2.write(i);
      delay(20);
     }
  }

  else if(analogRead(j1y) <= 300){
     for (int i = s2.read(); i > 40; i--){ //Gelenk unten Pos oben
      if (analogRead(j1y) > 300){
        break;
      }
      s2.write(i);
      delay(20);
     }
  }
//-------------------------------------------
  if(analogRead(j2x) >= 800){
     for (int i = s4.read(); i < 120; i++){//120 Gelenk oben
      if (analogRead(j2x) < 800){
        break;
      }
      s4.write(i);
      delay(30);
     }
  }

  else if(analogRead(j2x) <= 300){
     for (int i = s4.read(); i > 50; i--){//50 Gelenk oben
      if (analogRead(j2x) > 300){
        break;
      }
      s4.write(i);
      delay(30);
     }
  }
//-------------------------------------------
  if(analogRead(j2y) >= 800){
     for (int i = s3.read(); i < 80; i++){//120 Greifarm schließen
      if (analogRead(j2y) < 800){
        break;
      }
      s3.write(i);
      delay(20);
     }
  }

  else if(analogRead(j2y) <= 300){
     for (int i = s3.read(); i > 20; i--){  //60 Greifarm offen
      if (analogRead(j2y) > 300){
        break;
      }
      s3.write(i);
      delay(20);
     }
  }
  
}

void automatedModePage(){

  cs1 = false;
  cs2 = false;
  cs4 = false;
  cs5 = false;
  ssn = 0;

  if (cs3 == false){
    tft.fillScreen(BLACK);
    backauto_btn.initButton(&tft,  160, 180, 280, 55, WHITE, CYAN, BLACK, "zurueck", 3);  
    backauto_btn.drawButton(false);
    record_btn.initButton(&tft,  245, 115, 110, 55, WHITE, CYAN, BLACK, "start", 3);  
    record_btn.drawButton(false);
    play_btn.initButton(&tft,  103, 115, 162, 55, WHITE, CYAN, BLACK, "speich.", 3); //110, 55 
    play_btn.drawButton(false);
    tft.setFont(&FreeSansBold18pt7b);
    tft.setCursor(60, 60);                   //x,y
    tft.setTextColor(CYAN);                  //color
    tft.setTextSize(1);                      //Eg., 1,2,3
    tft.print("Auto. Modus");                //Text
    cs3 = true;
  }  

  bool down = Touch_getXY();
    record_btn.press(down && record_btn.contains(pixel_x, pixel_y));
    play_btn.press(down && play_btn.contains(pixel_x, pixel_y));
    backauto_btn.press(down && backauto_btn.contains(pixel_x, pixel_y));
    if (record_btn.justReleased()){
        tft.setFont();
        record_btn.drawButton(false);
        page = "record";
    }        
    if (play_btn.justReleased()){
        tft.setFont();
        play_btn.drawButton(false);
        page = "play";
    }
    if (backauto_btn.justReleased()){
        tft.setFont();
        backauto_btn.drawButton(false);
        page = "home";
    }
    if (record_btn.justPressed()) {
        tft.setFont();
        record_btn.drawButton(true);
    }
    if (play_btn.justPressed()) {
        tft.setFont();
        play_btn.drawButton(true);
    }  
    if (backauto_btn.justPressed()) {
        tft.setFont();
        backauto_btn.drawButton(true);
    }  
}

void recordPage(){

  cs1 = false;
  cs2 = false;
  cs3 = false;
  cs5 = false;

  if (cs4 == false){
    tft.fillScreen(BLACK);
    backrecord_btn.initButton(&tft,  160, 205, 260, 55, GREEN, CYAN, BLACK, "zurueck", 3);  
    backrecord_btn.drawButton(false);
    save_btn.initButton(&tft,  160, 140, 260, 55, CYAN, GREEN, BLACK, "speichern", 3);  
    save_btn.drawButton(false);
    tft.setFont(&FreeSansBold18pt7b);
    tft.setCursor(67, 50);                   //x,y
    tft.setTextColor(CYAN);                  //color
    tft.setTextSize(1);                      //Eg., 1,2,3
    tft.print("speichern..."); 
    tft.fillRect(35, 70, 250, 30, WHITE);
    tft.drawRect(35, 70, 50, 30, BLUE);  
    tft.drawRect(85, 70, 50, 30, BLUE); 
    tft.drawRect(135, 70, 50, 30, BLUE); 
    tft.drawRect(185, 70, 50, 30, BLUE); 
    tft.drawRect(235, 70, 50, 30, BLUE); 
    cs4 = true;
  }

  bool down = Touch_getXY();
    backrecord_btn.press(down && backrecord_btn.contains(pixel_x, pixel_y));
    save_btn.press(down && save_btn.contains(pixel_x, pixel_y));
    if (backrecord_btn.justReleased()){
        tft.setFont();
        backrecord_btn.drawButton(false);
        Serial.println("a");
        page = "automated";
    }        
    if (save_btn.justReleased()){
        tft.setFont();
        save_btn.drawButton(false);
        Serial.println("b");
        switch(ssn){
          case 0:
            servo1PosSave[0] = s1.read();
            servo2PosSave[0] = s2.read();
            servo3PosSave[0] = s3.read();
            servo4PosSave[0] = s4.read();
            tft.fillRect(35, 70, 50, 30, YELLOW);
            break;
          case 1:
            servo1PosSave[1] = s1.read();
            servo2PosSave[1] = s2.read();
            servo3PosSave[1] = s3.read();
            servo4PosSave[1] = s4.read();
            tft.fillRect(85, 70, 50, 30, YELLOW);
            break;
          case 2:
            servo1PosSave[2] = s1.read();
            servo2PosSave[2] = s2.read();
            servo3PosSave[2] = s3.read();
            servo4PosSave[2] = s4.read();
            tft.fillRect(135, 70, 50, 30, YELLOW);
           break;
          case 3:
            servo1PosSave[3] = s1.read();
            servo2PosSave[3] = s2.read();
            servo3PosSave[3] = s3.read();
            servo4PosSave[3] = s4.read();
            tft.fillRect(185, 70, 50, 30, YELLOW);
            break;
          case 4:
            servo1PosSave[4] = s1.read();
            servo2PosSave[4] = s2.read();
            servo3PosSave[4] = s3.read();
            servo4PosSave[4] = s4.read();
            tft.fillRect(235, 70, 50, 30, YELLOW);
            break;
        }
        ssn++;
    }
    if (backrecord_btn.justPressed()) {
        tft.setFont();
        backrecord_btn.drawButton(true);
        Serial.println("c");
    }
    if (save_btn.justPressed()) {
        tft.setFont();
        save_btn.drawButton(true);
        Serial.println("d");
    }

   //-------------------------------Automatik Joystick Control  -------------------------------------------------------------------------------------------------------------

  if(analogRead(j1x) >= 800){
     for (int i = s1.read(); i < 180; i++){
      if (analogRead(j1x) < 800){
        break;
      }
      s1.write(i);
      delay(20);
     }
  }

  if(analogRead(j1x) <= 300){
     for (int i = s1.read(); i > 0; i--){
      if (analogRead(j1x) > 300){
        break;
      }
      s1.write(i);
      delay(20);
     }
  }
//-------------------------------------------
  if(analogRead(j1y) >= 800){
     for (int i = s2.read(); i < 140; i++){
      if (analogRead(j1y) < 800){
        break;
      }
      s2.write(i);
      delay(20);
     }
  }

  if(analogRead(j1y) <= 300){
     for (int i = s2.read(); i > 70; i--){
      if (analogRead(j1y) > 300){
        break;
      }
      s2.write(i);
      delay(20);
     }
  }
//-------------------------------------------
  if(analogRead(j2x) >= 800){
     for (int i = s4.read(); i < 120; i++){
      if (analogRead(j2x) < 800){
        break;
      }
      s4.write(i);
      delay(30);
     }
  }

  if(analogRead(j2x) <= 300){
     for (int i = s4.read(); i > 50; i--){
      if (analogRead(j2x) > 300){
        break;
      }
      s4.write(i);
      delay(30);
     }
  }
//-------------------------------------------
  if(analogRead(j2y) >= 800){
     for (int i = s3.read(); i < 120; i++){
      if (analogRead(j2y) < 800){
        break;
      }
      s3.write(i);
      delay(20);
     }
  }

  if(analogRead(j2y) <= 300){
     for (int i = s3.read(); i > 60; i--){
      if (analogRead(j2y) > 300){
        break;
      }
      s3.write(i);
      delay(20);
     }
  }
}

void playPage(){

  cs1 = false;
  cs2 = false;
  cs3 = false;
  cs4 = false;
  ssn = 0;

  if (cs5 == false){
    tft.fillScreen(BLACK);
    backplay_btn.initButton(&tft,  160, 190, 250, 55, WHITE, CYAN, BLACK, "zurueck", 3);  
    backplay_btn.drawButton(false);
    tft.setFont(&FreeSansBold18pt7b);
    tft.setCursor(65, 50);                   //x,y
    tft.setTextColor(CYAN);                  //color
    tft.setTextSize(1);                      //Eg., 1,2,3
    tft.print("gestartet...");               //Text
    tft.setFont(&FreeSansBold12pt7b);
    tft.setTextSize(1);                      //Eg., 1,2,3
    tft.setCursor(40, 100);
    tft.setTextColor(WHITE);
    tft.print("Gesamtpositionen: 5");
    cs5 = true;
  }

  bool down = Touch_getXY();
  backplay_btn.press(down && backplay_btn.contains(pixel_x, pixel_y));
  if (backplay_btn.justReleased()){
      tft.setFont();
      backplay_btn.drawButton(false);
      Serial.println("a");
      page = "automated";
  }        
  if (backplay_btn.justPressed()) {
      tft.setFont();
      backplay_btn.drawButton(true);
      Serial.println("c");
  }

  
  for (int i = 0; i < 5; i++){

    tft.fillRect(0, 110, 320, 40, BLACK);
    tft.setCursor(40, 130);                  
    tft.setTextSize(1);                     
    String a = "Aktuelle Position: ";
    int j = i + 1;
    String b = a + j;
    tft.print(b);  

    bool down = Touch_getXY();
    backplay_btn.press(down && backplay_btn.contains(pixel_x, pixel_y));
    if (backplay_btn.justReleased()){
        tft.setFont();
        backplay_btn.drawButton(false);
        Serial.println("a");
        page = "automated";
    }        
    
    if (backplay_btn.justPressed()) {
        tft.setFont();
        backplay_btn.drawButton(true);
        Serial.println("c");
    }
    
    if (servo1PosSave[i] < s1.read()){
      for (int p = s1.read(); p >= servo1PosSave[i]; p--){
        s1.write(p);
        delay(20); 
      }
    }

    if (servo1PosSave[i] > s1.read()){
      for (int p = s1.read(); p <= servo1PosSave[i]; p++){
        s1.write(p);
        delay(20); 
      }
    }

    if (servo2PosSave[i] < s2.read()){
      for (int p = s2.read(); p >= servo2PosSave[i]; p--){
        s2.write(p);
        delay(20); 
      }
    }

    if (servo2PosSave[i] > s2.read()){
      for (int p = s2.read(); p <= servo2PosSave[i]; p++){
        s2.write(p);
        delay(20); 
      }
    }
  
    if (servo3PosSave[i] < s3.read()){
      for (int p = s3.read(); p >= servo3PosSave[i]; p--){
        s3.write(p);
        delay(20); 
      }
    }

    if (servo3PosSave[i] > s3.read()){
      for (int p = s3.read(); p <= servo3PosSave[i]; p++){
        s3.write(p);
        delay(20); 
      }
    }

    if (servo4PosSave[i] < s4.read()){
      for (int p = s4.read(); p >= servo4PosSave[i]; p--){
        s4.write(p);
        delay(20); 
      }
    }

    if (servo4PosSave[i] > s4.read()){
      for (int p = s4.read(); p <= servo4PosSave[i]; p++){
        s4.write(p);
        delay(20); 
      }
    }
  }
}
