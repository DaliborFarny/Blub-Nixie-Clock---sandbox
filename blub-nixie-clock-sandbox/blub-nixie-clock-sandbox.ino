/*
|| @author         Dalibor Farny <dalibor@farny.cz>
|| @url            https://www.daliborfarny.com/
||
|| @description
|| | A minimal code for use with Blub Nixie Clock hardware
|| |
|| | Written by Dalibor Farny
|| | https://www.daliborfarny.com/
|| | dalibor@farny.cz
|| |
|| | This is a minimal code that demostrates the functions of the Blub hardware. You can base your applications on this code.
|| | Use "Arduino Nano" as the board option and "ATmega328p" as the processor option in the Arduino IDE application.
|| |
|| #
||
|| @license Please see the accompanying LICENSE file for this project.
||
|| @name Blub Nixie Clock sandbox
|| @target Atmel AVR 8 Bit
||
|| @version 1.0.0
||
*/

/****** DEFINITIONS ************************************************************************************/

#define NIXIE_0 5
#define NIXIE_1 2
#define NIXIE_2 4
#define NIXIE_3 9
#define NIXIE_4 3
#define NIXIE_5 A0
#define NIXIE_6 6
#define NIXIE_7 10
#define NIXIE_8 A2
#define NIXIE_9 A1
#define BUTTON_SET 8
#define BUTTON_ADJ 7

const int cathodes[] = {NIXIE_0,NIXIE_1,NIXIE_2,NIXIE_3,NIXIE_4,NIXIE_5,NIXIE_6,NIXIE_7,NIXIE_8,NIXIE_9};

/******** VARIABLES ************************************************************************************/

boolean button_set_pressed = false; //flag for SET button (true when pressed)
boolean button_adj_pressed = false; //flag for ADJ button (true when pressed)


/******** FUNCTIONS ************************************************************************************/

void InitialiseInterrupt() {
  cli(); // switch interrupts off while messing with their settings
  PCMSK2 |= (1 << PCINT23); //button_adj
  PCMSK0 |= (1 << PCINT0);  //button_set
  PCICR |= (1 << PCIE2);    // Enable PCINT interrupt
  PCICR |= (1 << PCIE0);    // Enable PCINT interrupt
  sei(); // turn interrupts back on
}
ISR(PCINT2_vect) { // interrupt routine for the ADJ button
  if (digitalRead(BUTTON_ADJ) == 1) {
     button_adj_pressed = true;
  }
}
ISR(PCINT0_vect) { // interrupt routine for the SET button
  if (digitalRead(BUTTON_SET) == 1) {
     button_set_pressed = true;
  }
}
/**/

void initialize_IO() {
  //initialization of the output pins (nixie tube cathodes)
  for (uint8_t n = 0; n < 10; n++) {
    pinMode(cathodes[n], OUTPUT);
    digitalWrite(cathodes[n], HIGH);
  }
  //initialization of the button pins
  pinMode(BUTTON_SET, INPUT);
  pinMode(BUTTON_ADJ, INPUT);
}

void display_number(int digit) {
  //blank all digits
  for (int n = 0; n < 10; n++) {
    digitalWrite(cathodes[n], HIGH);
  }  
  //if digit is from 0-9, activate the correct cathode
  if (digit>=0 && digit<10) digitalWrite(cathodes[digit], LOW);
}

void clear_display() {
  display_number(11);
}

void setup() { 
  initialize_IO();
  InitialiseInterrupt();
  clear_display();
}

void loop() {
  if (!button_adj_pressed) { //pause cycling through digits if ADJ pressed
    for (uint8_t i=0; i<10; i++){
      if (button_adj_pressed) { //exit the FOR cycle if ADJ pressed
        break;
      }
      display_number(i);
      delay(200);
    }
  }
  if (button_set_pressed) { // continue cycling through digits if SET pressed
    button_set_pressed = false;
    button_adj_pressed = false;  
  }
}
