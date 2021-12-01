#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <Adafruit_MCP4725.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

#define DEBUG true

// four capacitive touch chips:
Adafruit_MPR121 cap1 = Adafruit_MPR121();
Adafruit_MPR121 cap2 = Adafruit_MPR121();
Adafruit_MPR121 cap3 = Adafruit_MPR121();
Adafruit_MPR121 cap4 = Adafruit_MPR121();
// two 12-bit dacs:
Adafruit_MCP4725 dac1, dac2;
// LED
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

int vout1, vout2;
float pressureval; // pressure value
float psmooth = 0.9; // y(n-1) coefficient
int ptemp; // raw pressure
int curkey; // current key
int pcount; //polyphony count
int tlen; // trigger length
int TDUR = 20; // trigger length
int POFF = 40; // pressure offset value
float VPST = 68.25; // volts per semitone

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched1 = 0;
uint16_t currtouched1 = 0;
uint16_t lasttouched2 = 0;
uint16_t currtouched2 = 0;
uint16_t lasttouched3 = 0;
uint16_t currtouched3 = 0;
uint16_t lasttouched4 = 0;
uint16_t currtouched4 = 0;

void setup() {
  if(DEBUG) {
    Serial.begin(9600);
    while (!Serial) { // needed to keep leonardo/micro from starting too fast!
      delay(10);
   }
  }

  pinMode(2, OUTPUT); // trigger A
  pinMode(3, OUTPUT); // gate A
  pinMode(5, OUTPUT); // pressure
  
  if(DEBUG) Serial.println("YAY KEYBOARD"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap1.begin(0x5D)) {
    //if(DEBUG) Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 A found!");
  if (!cap2.begin(0x5C)) {
    //if(DEBUG) Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 B found!");
  if (!cap3.begin(0x5B)) {
    //if(DEBUG) Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 C found!");
  if (!cap4.begin(0x5A)) {
    //if(DEBUG) Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 D found!");

  // start DACs:
  dac1.begin(0x62);
  dac2.begin(0x63);

  // start LED:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Hi there");
  display.println("YAY");
  display.setCursor(0,0);
  display.display(); // actually display all of the above


  pressureval = 0.;
  curkey = 0;
  vout1 = 0;
  vout2 = 0;
}

void loop() {
  // Get the currently touched pads
  currtouched1 = cap1.touched(); // bitmask
  currtouched2 = cap2.touched(); // bitmask
  currtouched3 = cap3.touched(); // bitmask
  currtouched4 = cap4.touched(); // bitmask

  for (uint8_t i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched1 & _BV(i)) && !(lasttouched1 & _BV(i)) ) {
      if(DEBUG) Serial.print("keydown "); Serial.println(i);
      vout1 = round((float)i*VPST);
      pcount++; // polyphony count up
      tlen = TDUR; // new trigger
      curkey = i;
      display.clearDisplay();
      display.setCursor(0,0);
      display.print("keydown ");
      display.println(i);
      display.display(); // actually display all of the above
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched1 & _BV(i)) && (lasttouched1 & _BV(i)) ) {
      if(DEBUG) Serial.print("keyup "); Serial.println(i);
      pcount--; // polyphony count down
      display.clearDisplay();
      display.setCursor(0,0);
      display.print("keyup ");
      display.println(i);
      display.display(); // actually display all of the above
    }


    
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched2 & _BV(i)) && !(lasttouched2 & _BV(i)) ) {
      if(DEBUG) Serial.print(i); Serial.println(" touched on B");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched2 & _BV(i)) && (lasttouched2 & _BV(i)) ) {
      if(DEBUG) Serial.print(i); Serial.println(" released on B");
    }
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched3 & _BV(i)) && !(lasttouched3 & _BV(i)) ) {
      if(DEBUG) Serial.print(i); Serial.println(" touched on C");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched3 & _BV(i)) && (lasttouched3 & _BV(i)) ) {
      if(DEBUG) Serial.print(i); Serial.println(" released on C");
    }
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched4 & _BV(i)) && !(lasttouched4 & _BV(i)) ) {
      if(DEBUG) Serial.print(i); Serial.println(" touched on D");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched4 & _BV(i)) && (lasttouched4 & _BV(i)) ) {
      if(DEBUG) Serial.print(i); Serial.println(" released on D");
    }
  }

  // reset our state
  lasttouched1 = currtouched1;
  lasttouched2 = currtouched2;
  lasttouched3 = currtouched3;
  lasttouched4 = currtouched4;

  // triggers and gates
  if(tlen>0) digitalWrite(2, HIGH); else digitalWrite(2, LOW);
  if(pcount>0) digitalWrite(3, HIGH); else digitalWrite(3, LOW);

  // cv pitch out
  dac1.setVoltage(vout1, false);
  dac2.setVoltage(vout2, false);

  // pressure value (RC'ed through an 8k2 and a .22uF)
  ptemp = map(cap1.filteredData(curkey)-POFF, 0, cap1.baselineData(curkey), 255, 0);
  ptemp = constrain(ptemp, 0, 255);
  pressureval = psmooth*pressureval + (1.0-psmooth)*ptemp;
  if(DEBUG) {
        Serial.print("press ");
        Serial.println(pressureval);
  }
  analogWrite(5, (int)pressureval);

 
  tlen--;
  if(tlen<0) tlen=0;
}
