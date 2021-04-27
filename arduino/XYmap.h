// Params for width and height
const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 8;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[ NUM_LEDS ];
#define LAST_VISIBLE_LED 127
uint8_t XY (uint8_t x, uint8_t y) {
  // any out of bounds address maps to the first hidden pixel
  if ( (x >= kMatrixWidth) || (y >= kMatrixHeight) ) {
    return (LAST_VISIBLE_LED + 1);
  }

  const uint8_t XYTable[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 64, 65, 66, 67, 68, 69, 70, 71,                      
    8, 9, 10, 11, 12, 13, 14, 15, 72, 73, 74, 75, 76, 77, 78, 79,
    16, 17, 18, 19, 20, 21, 22, 23, 80, 81, 82, 83, 84, 85, 86, 87,
    24, 25, 26, 27, 28, 29, 30, 31, 88, 89, 90, 91, 92, 93, 94, 95,
    32, 33, 34, 35, 36, 37, 38, 39, 96, 97, 98, 99, 100, 101, 102, 103,
    40, 41, 42, 43, 44, 45, 46, 47, 104, 105, 106, 107, 108, 109, 110, 111,
    48, 49, 50, 51, 52, 53, 54, 55, 112, 113, 114, 115, 116, 117, 118, 119,
    56, 57, 52, 59, 60, 61, 62, 63, 120, 121, 122, 123, 124, 125, 126, 127
  };

  uint8_t i = (y * kMatrixWidth) + x;
  uint8_t j = XYTable[i];
  return j;
}
