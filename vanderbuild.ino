#include <U8g2lib.h>
#include <U8x8lib.h>

/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 This is the calibration sketch. Use it to determine the calibration_factor that the main example uses. It also
 outputs the zero_factor useful for projects that have a permanent mass on the scale in between power cycles.

 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Use this calibration_factor on the example sketch

 This example assumes pounds (lbs). If you prefer kilograms, change the Serial.print(" lbs"); line to kg. The
 calibration factor will be significantly different but it will be linearly related to lbs (1 lbs = 0.453592 kg).

 Your calibration factor may be very positive or very negative. It all depends on the setup of your scale system
 and the direction the sensors deflect from zero state
 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE
 Arduino pin 8 -> HX711 CLK
 9 -> DOUT
 5V -> VCC
 GND -> GND

 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.

 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

*/

#include "HX711.h"

#define DOUT  9
#define CLK  8

HX711 scale(DOUT, CLK);

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);
const float trigger = 0.3;

float lastValue;
float fullValue = 0.0;
char output[8];
float zeroValue;

void setup() {
  u8g2.begin();
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  zeroValue = lastValue = scale.read_average(10);
  write("PLACE");
  write("ITEM");
  write("ON");
  write("SCALE");
  write("TO");
  write("BEGIN!");
  float r;
  do {
    write("... ");
    r = scale.read_average(10);
    write(" ...");
  } while (abs((lastValue - r)/lastValue) < trigger);
  fullValue = scale.read_average(10);
}



void loop() {
  lastValue = scale.read_average(10);
  int v = (int)((lastValue-zeroValue)/(fullValue-zeroValue)*100);
  v = constrain(v, 0, 100);
  snprintf(output, sizeof(output), "%d %", v);
  write(output);
}

void write(char stringOutput[]) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.drawStr(8, 29, stringOutput); // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(800);
}

