#include "font.h"
#include "XYmap.h"

boolean effectInit = false; 
CRGBPalette16 currentPalette(RainbowColors_p);

//unsigned int currentStringAddress = 0;
//void selectFlashString(const char string) {
//  currentStringAddress = &string;
//}

byte charBuffer[5] = {0};

void loadCharBuffer(char character) {
  char mappedCharacter = character;
  if (mappedCharacter >= 32 && mappedCharacter <= 95) {
    mappedCharacter -= 32;
  } else if (mappedCharacter >= 97 && mappedCharacter <= 122) {
    mappedCharacter -= 64;
  } else {
    mappedCharacter = 96;
  }
  
  for (int i = 0; i < 5; i++) {
    charBuffer[i] = pgm_read_byte(Font[mappedCharacter]+i);
  }
  
}

char loadStringChar(int character) {
  return (char) string0[character];
}
