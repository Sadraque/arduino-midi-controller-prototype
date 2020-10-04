/*
  This is a simple and initial prototype for a Arduino Midi controller
     All documentation about MIDI protocol are avalaible on the link below:
       https://www.midi.org/specifications
       
  Author: Sadraque Nunes - sadraquenunesmartiniano@gmail.com
                                                  10/03/2020     
*/

#include <MIDI.h>

/*ARDUINO CONFIG----------------------------------------*/
#define BAUD_RATE 115200
#define ADC 1023

/*BUTTONS-----------------------------------------------*/
const int NUM_BUTTONS = 2;
const int BUTTONS [NUM_BUTTONS] = {2,3};
int lastStateButton [NUM_BUTTONS] = {1,1}; 
int buttonState; 

/*POTENTIOMETERS----------------------------------------*/
const int NUM_POTENTIOMETERS = 1;
const int POTENTIOMETERS [NUM_POTENTIOMETERS] = {0};
int lastStatePot [NUM_POTENTIOMETERS] = {-1}; //used for fetch potentiometer value
int potValue; 
  
/*LEDS--------------------------------------------------*/
const int NUM_LEDS = 2;
const int LEDS [NUM_LEDS] = {11,12};
int ledStatus [NUM_LEDS] = {LOW,LOW};                    

/*MIDI--------------------------------------------------*/
MIDI_CREATE_DEFAULT_INSTANCE();
const int CHANNEL = 1;

void setup() {
  Serial.begin(BAUD_RATE);
  initializeButtons();
  initializeLeds();
}

void loop() {
  readButtons();
  readPotentiometers();
}

//initialize buttons digital pin config 
void initializeButtons() {
  for(int i = 0; i < NUM_BUTTONS;i++) {
    pinMode(BUTTONS[i],INPUT_PULLUP);
  }
}

//initialize leds digital pin config 
void initializeLeds() {
  for(int i = 0; i < NUM_LEDS;i++) {
    pinMode(LEDS[i],OUTPUT);
  }
}

//reads the state of the buttons
void readButtons() {
    
  
    for(int i = 0; i < NUM_BUTTONS;i++) {
      buttonState = digitalRead(BUTTONS[i]);
      
      digitalWrite(LEDS[i], ledStatus[i]);
      
      if(lastStateButton[i] != buttonState) {
        if(buttonState == LOW) {  
          
          ledStatus[i] = !ledStatus[i]; 
               
          //digitalWrite(LED, HIGH);
          MIDI.sendNoteOn(60 + i,40,CHANNEL);
          //Serial.print("BUTTON["); Serial.print(i); Serial.print("]: HIGH\n");
              
        } else {
          //digitalWrite(LED, LOW);
          MIDI.sendNoteOn(60 + i,0,CHANNEL);
          //Serial.print("BUTTON["); Serial.print(i); Serial.print("]: LOW\n");
          
        }

      //trigger action button
      lastStateButton[i] = buttonState;
      }
    }
       
}

void readPotentiometers() {
    for(int i = 0; i < NUM_POTENTIOMETERS;i++) {
        int potValue = map(analogRead(POTENTIOMETERS[i]),0,ADC,0,127);
        if(lastStatePot[i] != potValue) {
        MIDI.sendControlChange(7,potValue,CHANNEL);  
        //Serial.print("POTENTIOMETER["); Serial.print(i); Serial.print("]: "); Serial.print(potValue); Serial.print("\n");

        //trigger action button
        lastStatePot[i] = potValue;

        }
    }  
}
