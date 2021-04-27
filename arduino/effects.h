#include "utils.h"

#define NORMAL 0
#define RAINBOW 1
#define charSpacing 2

extern int currentMessageChar = 0;

void fillAll(CRGB color) {
  
  for (byte i = 0; i < NUM_LEDS; i++) {
    leds[i] = color; CRGB(100, 80, 200);
  }
}

void fillAll() {
  
  for (byte i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(100, 80, 200);
  }
}

void confirmBlink() {
    fillAll(CRGB::DarkRed);
    FastLED.show();
    FastLED.delay(200);
    fillAll(CRGB::Black);
    FastLED.delay(200);
    fillAll(CRGB::DarkRed);
    FastLED.show();
    FastLED.delay(200);
    fillAll(CRGB::Black);
    FastLED.delay(200);

}

void threeSine() {

  static byte sineOffset = 0; // counter for current position of sine waves

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    //effectDelay = 20;
  }

  // Draw one frame of the animation into the LED array
  for (byte x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {

      // Calculate "sine" waves with varying periods
      // sin8 is used for speed; cos8, quadwave8, or triwave8 would also work here
      byte sinDistanceR = qmul8(abs(y * (255 / kMatrixHeight) - sin8(sineOffset * 9 + x * 16)), 2);
      byte sinDistanceG = qmul8(abs(y * (255 / kMatrixHeight) - sin8(sineOffset * 10 + x * 16)), 2);
      byte sinDistanceB = qmul8(abs(y * (255 / kMatrixHeight) - sin8(sineOffset * 11 + x * 16)), 2);

      leds[XY(x, y)] = CRGB(255 - sinDistanceR, 255 - sinDistanceG, 255 - sinDistanceB);
    }
  }

  sineOffset++; // byte will wrap from 255 to 0, matching sin8 0-255 cycle

}

void scrollText(char *message, byte style, CRGB fgColor, CRGB bgColor) {
  
  static byte currentCharColumn = 0;
  static byte paletteCycle = 0;
  static CRGB currentColor;
  static byte bitBuffer[16] = {0};
  static byte bitBufferPointer = 0;
  
  if (effectInit == false) {
    effectInit = true;
    //effectDelay = 35;
    currentMessageChar = 0;
    currentCharColumn = 0;
//    selectFlashString(message);
    loadCharBuffer(loadStringChar(currentMessageChar));
    currentPalette = RainbowColors_p;
    for (byte i = 0; i < kMatrixWidth; i++) bitBuffer[i] = 0;
  }

  paletteCycle += 15;

  if (currentCharColumn < 5) { // characters are 5 pixels wide
    bitBuffer[(bitBufferPointer + kMatrixWidth - 1) % kMatrixWidth] = charBuffer[currentCharColumn];
  } else {
    bitBuffer[(bitBufferPointer + kMatrixWidth - 1) % kMatrixWidth] = 0; // espaço
  }

  CRGB pixelColor;
  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < 5; y++) { // characters are 5 pixels tall
      if (bitRead(bitBuffer[(bitBufferPointer + x) % kMatrixWidth], y) == 1) {
        if (style == RAINBOW) {
          pixelColor = ColorFromPalette(currentPalette, paletteCycle+y*16, 255);
        } else {
          pixelColor = fgColor;
        }
      } else {
        pixelColor = bgColor;
      }
      leds[XY(x, y)] = pixelColor;
    }
  }

  currentCharColumn++;
  if (currentCharColumn > (4 + charSpacing)) {
    currentCharColumn = 0;
    currentMessageChar++;
    char nextChar = loadStringChar(currentMessageChar);
    if (nextChar == 0) { // null character at end of strong
      currentMessageChar = 0;
      nextChar = loadStringChar(currentMessageChar);
    }
    loadCharBuffer(nextChar);
  }

  bitBufferPointer++;
  if (bitBufferPointer > 15) bitBufferPointer = 0;

}


void textEffectOne() {
  scrollText(string0, NORMAL, CRGB::Red, CRGB::Black);
}

void textEffectTwo() {
  scrollText(string0, RAINBOW, 0, CRGB::Black);
}

void textEffectThree() {
  scrollText(string0, charSpacing, CRGB::Green, CRGB(0,0,8));
}
