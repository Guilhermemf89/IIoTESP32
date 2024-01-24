/*
  picktolight.cpp - Library for controlling slots of a pick to light system using WS2812B led strip.
  Created by Guilherme M. Franco, November 17, 2023.
  Inspired from https://docs.arduino.cc/learn/contributions/arduino-library-style-guide
  Released into the public domain.
*/

#include "Picktolight.h"

// define as portas das saídas
#define ledOutput0                         13
#define ledOutput1                         12
#define ledOutput2                         14
#define ledOutput3                         27

Picktolight::Picktolight(CRGB delimiterColor)
{
  _last_scan = millis();
  _blink_state = true;
  _delimiterColor = delimiterColor;
}

// put in void setup()
void Picktolight::begin()
{
  //FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds_STRIP, NUM_LEDS);
  FastLED.addLeds<WS2812B, ledOutput0, GRB>(ledstrip_output[0], 300);
  FastLED.addLeds<WS2812B, ledOutput1, GRB>(ledstrip_output[1], 300);
  FastLED.addLeds<WS2812B, ledOutput2, GRB>(ledstrip_output[2], 300);
  FastLED.addLeds<WS2812B, ledOutput3, GRB>(ledstrip_output[3], 300);
  FastLED.setBrightness(40);
}

// create a slot
void Picktolight::addSlot(byte ledStrip, byte id, int first, int last, bool initialDelimiter, bool finalDelimiter)
{
  slots[id].ledStrip = ledStrip;
  slots[id].firstLed = first;
  slots[id].lastLed = last;
  slots[id].initialDelimiter = initialDelimiter;
  slots[id].finalDelimiter = finalDelimiter;
}

// put in void loop() for blink to work
void Picktolight::scanPicktolight()
{
  // execute every "_time_blink" millisenconds
  if ((millis() - _last_scan) >= _time_blink){
    _last_scan = millis();
    // loop through each slot configuration
    for (int i = 0; i < 256; i++){
      // if the slot is set to blink turn it on or off
      if (slots[i].blinkEffect){
        if (_blink_state){
          lightSlot(i, slots[i].color);
        }
        else{
          lightSlot(i, CRGB::Black);
        }
      }
    }
    _blink_state = !_blink_state;
  } 
}

// loop through each led of the slot setting the color
void Picktolight::lightSlot(byte id, CRGB color){
  // start by the delimiters
  byte initialOffset = 0;
  byte finalOffset = 0;
  if (slots[id].initialDelimiter){
    initialOffset = 1;
    ledstrip_output[slots[id].ledStrip][slots[id].firstLed] = _delimiterColor;
  }
  if (slots[id].finalDelimiter){
    finalOffset = 1;
    ledstrip_output[slots[id].ledStrip][slots[id].lastLed] = _delimiterColor;
  }
  // then procede to light the "infill"
  for (int i = (slots[id].firstLed + initialOffset); i <= (slots[id].lastLed - finalOffset); i++){
    ledstrip_output[slots[id].ledStrip][i] = color;
  }
  FastLED.show();
}

// change the color and blink effect of the slot
void Picktolight::setColor(byte id, CRGB color, bool blinkEffect)
{
  slots[id].color = color;
  slots[id].blinkEffect = blinkEffect;
  lightSlot(id, color);
}
