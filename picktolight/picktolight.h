/*
  picktolight.h - Library for controlling slots of a pick to light system using WS2812B led strip.
  Created by Guilherme M. Franco, November 17, 2023.
  Inspired from https://docs.arduino.cc/learn/contributions/arduino-library-style-guide
  Released into the public domain.
*/
#ifndef picktolight_h
#define picktolight_h

#include "Arduino.h"
#include <FastLED.h>                        // https://github.com/FastLED/FastLED

class Picktolight
{
  public:
    Picktolight(CRGB delimiterColor = CRGB::Black);
    void begin();                                                                                             // put in void setup()
    void addSlot(byte ledStrip, byte id, int first, int last, bool initialDelimiter, bool finalDelimiter);    // create a slot
    void scanPicktolight();                                                                                   // put in void loop() for blink to work
    void setColor(byte id, CRGB color, bool blinkEffect);                                                     // change the color and blink effect of the slot
  private:
    CRGB ledstrip_output[4][300];          // create 4 led strips with 300 leds max 
    struct slotProperties                  // array of struct to define the slot layout
    {
      byte ledStrip;
      int firstLed;
      int lastLed;
      CRGB color = CRGB::Black;
      bool blinkEffect = false;
      bool initialDelimiter = false;
      bool finalDelimiter = false;
    } slots[256];
    unsigned long _last_scan;              // stored the last time of the scan, used by the blink function
    int _time_blink = 800;                 // scan period used by the blink function
    bool _blink_state;                     // indicates if the light should be on or off in the blink function
    CRGB _delimiterColor;                  // color of the delimiters of each slot
    void lightSlot(byte id, CRGB color);   // loop through each led of the slot setting the color
};

#endif
