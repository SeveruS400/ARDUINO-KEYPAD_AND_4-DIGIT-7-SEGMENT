/*
 * 4-DIGIT 7 SEGMENT AND KEYPAD
 * WHEN PRESS BUTTONS, NUMBERS OR CHARACTER SHOW ON DIGIT SCREEN
 * 
 *    SeveruS 9/09/2020
 */
#include <Keypad.h>
#include <TM1637Display.h>
#include <SPI.h>

#define CLK 3                         //DEFINE 4-DIGIT 7 SEGMENT
#define DIO 4

const byte row = 4;                   
const byte col = 4;

TM1637Display display(CLK, DIO);

char tus_takimi[row][col] = {         
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte satir_pinleri[row] = {13, 12, 11, 10};
byte sutun_pinleri[col] = {9, 8, 7, 6};

unsigned int i=0;                 
uint8_t tire=0b01000000;

Keypad lcd = Keypad(makeKeymap(tus_takimi) , satir_pinleri , sutun_pinleri , row , col);

//======================================================================
void setup()
{
  Serial.begin(9600);
  lcd.addEventListener(keypadEvent);
  display.setBrightness(0x0f);
  uint8_t data[] = { 0b01000000, 0b01000000, 0b01000000, 0b01000000 };        //(- - - -)IN DIGIT
  display.setSegments(data);
  
}
//======================================================================
void loop()
  {    
       key =lcd.getKey();      //READ TO INPUT FROM KEYPAD
  }
//======================================================================
uint8_t fonk(int k){            //CONVERT HEX TO BINARY
  switch(k){
    case 48:return 0b00111111;  //0
    case 49:return 0b00000110;  //1
    case 50:return 0b01011011;  //2
    case 51:return 0b01001111;  //3
    case 52:return 0b01100110;  //4
    case 53:return 0b01101101;  //5
    case 54:return 0b01111101;  //6
    case 55:return 0b00000111;  //7
    case 56:return 0b01111111;  //8
    case 57:return 0b01101111;  //9
    case 65:return 0b01110111;  //A
    case 66:return 0b01111100;  //b
    case 67:return 0b00111001;  //C
    case 68:return 0b01011110;  //d
    case 42:return 0b01000000;  //*
    case 35:return 0b01000000;  //#
    default: return 0000;
  }
}
//======================================================================
void keypadEvent(KeypadEvent key){
  
   switch (lcd.getState()){
     case PRESSED:
      
       if (key == '#') {                  // WHEN PRESSED # BUTTON   
            i=0;
            for(int q=0;q<4;q++){         //DATA[] WİLL BE NULL(-)
              data[q]=0b01000000;         //0b01000000="-"
            }
         display.setSegments(data);      //SHOW DATA
         delay(10);
      }
      else if (key == '*') {            //DELETE LOOP
             i--;
            data[i]=0b01000000;     
            display.setSegments(data);
            delay(10);
      }
      else{
          if(i<4){
              if((key=='A'|| key=='B'|| key=='C'|| key=='D')&& data[0]==tire){        //IN MY PROCECT, FIRST DATA MUST CHARACTER
                data[0]=fonk(key);
                display.setBrightness(0x0f); 
                display.setSegments(data); 
                Serial.println(key);
                i++;
                delay(10);
              }
              else if(data[0]!=tire &&(key!='A'&& key!='B'&& key!='C'&& key!='D')){   //ANOTHER DATAS MUST NUMBER
                data[i]=fonk(key);
                display.setBrightness(0x0f); 
                display.setSegments(data); 
                i++;
                delay(10);
              }
        }
    }
        
      break;

    case HOLD:
        if (key == '*') {                     //PUSH 1-2 SECOND ON * BUTTON
            for(int j=0;j<4;j++){
              data[j]=0b01000000;             
            }
            display.setSegments(data);
            i=0;
            delay(100);            
        }
      break;
    }
}
