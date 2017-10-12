#include "FastLED.h"

// FastLED "100-lines-of-code" demo reel, showing just a few
// of the kinds of animation patterns you can quickly and easily
// compose using FastLED.
//
// This example also shows one easy way to define multiple
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    5
#define HEART_PIN   3
//#define CLK_PIN   4
#define LED_TYPE    NEOPIXEL

#define NUM_LEDS    45
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         100
#define FRAMES_PER_SECOND  120

void setup() {
  pinMode(HEART_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  // All leds black
  leds[0] = CRGB::Black;
  FastLED.show();
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 15; // rotating "base color" used by many of the patterns
float gHueStep = 2.8; // Step for Hue change
int numLedsOn = 0;

void loop()
{
  if (digitalRead(HEART_PIN) == HIGH ) {
    Serial.println(numLedsOn);
    numLedsOn++;   
  }
  
  if (numLedsOn <= NUM_LEDS ){
    fill_rainbow( leds, numLedsOn, 15, 2.8);
  } 
  
  if (numLedsOn > NUM_LEDS + 2 ) {
    numLedsOn = 0;
    Serial.println("=> Extinction");
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    FastLED.delay(1000);
  }
  
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(300);
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow()
{
  //Serial.println("rainbow");
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, 15, 2.8);
}

void rainbowWithGlitter()
{
  //Serial.println("rainbowWithGlitter");
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter)
{
  //Serial.println("addGlitter");
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti()
{
  Serial.println("confetti");
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  Serial.println("sinelon");
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  Serial.println("bpm");
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle() {
  Serial.println("juggle");
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
