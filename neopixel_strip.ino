#include <Adafruit_NeoPixel.h>
#include <stdint.h>

template<class T, size_t N>
constexpr size_t size(T (&)[N]) { return N; }


namespace strip {
  constexpr int pin = 6;
  constexpr int count = 4;
  Adafruit_NeoPixel s(count, pin, NEO_BGR + NEO_KHZ800);
}
namespace colors {
  // A list of the built-in HTML colours
  // This list is incomplete; you can help by expanding it.
  uint32_t ts(uint32_t rgb) {
    uint32_t r = (0xFF0000 & rgb) >> 16;
    uint32_t g = (0x00FF00 & rgb) >> 8;
    uint32_t b =  0x0000FF & rgb;
    return strip::s.Color(r, g, b);
  }
  uint32_t black   = ts(0x000000);
  uint32_t silver  = ts(0xc0c0c0);
  uint32_t gray    = ts(0x808080);
  uint32_t white   = ts(0xffffff);
  uint32_t maroon  = ts(0x800000);
  uint32_t red     = ts(0xff0000);
  uint32_t purple  = ts(0x800080);
  uint32_t fuchsia = ts(0xff00ff);
  uint32_t green   = ts(0x008000);
  uint32_t lime    = ts(0x00ff00);
  uint32_t olive   = ts(0x808000);
  uint32_t yellow  = ts(0xffff00);
  uint32_t navy    = ts(0x000080);
  uint32_t blue    = ts(0x0000ff);
  uint32_t teal    = ts(0x008080);
  uint32_t aqua    = ts(0x00ffff);
  uint32_t orange  = ts(0xffa500);
  uint32_t all[] = {
    black,   // 1
    silver,  // 2
    gray,    // 3
    white,   // 4
    maroon,  // 5 
    red,     // 6 
    purple,  // 7
    fuchsia, // 8
    green,   // 9
    lime,    // 10
    olive,   // 11
    yellow,  // 12
    navy,    // 13
    blue,    // 14
    teal,    // 15
    aqua,    // 16
    orange   // 17
  };
  constexpr int count = 17;
}


// A helper function that causes the colors to transition once to a fixed color.
void base_program_h(int t, uint32_t color) {
  using namespace strip;
  for (int i = 0; i < count; i++) {
    s.setPixelColor(i, color);
    delay(t);
    s.show();
  }
}

// A variant of the base program that gets the colors from an array.
// Waits t seconds before changing to the next color
// col[] is the set of colors.
template<int t, uint32_t col[], int c>
void base_program_a() {
  using namespace strip;
  for (int i = 0; i < c; i++) {
    base_program_h(t, col[i]);
  }
}

namespace base_program_hue {
  uint16_t hue;
  // A variant of the base program that gets the colors by incrementing the hue
  // in an HSV color
  template<uint16_t step, uint8_t saturation, uint8_t value, int t>
  void func() {
    hue += step;
    base_program_h(t, strip::s.ColorHSV(hue, saturation, value));
  }
}

void clear_strip() {
  using namespace strip;
  for (int i = 0; i < count; i++)
    s.setPixelColor(i, 0, 0, 0);
  s.show();
}

// This function should return whether the program should be changed
bool changeEvent() {
  if (Serial.available()) 
    return Serial.read() == '\n';
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  strip::s.begin();
  strip::s.show();
}

typedef void(*Program)();

constexpr Program progs[] = {
  //base_program_a<50, colors::all, colors::count>,
  base_program_hue::func<125, 255, 255, 25>
};
size_t idx = 0;
constexpr size_t pr_count = size(progs);
void loop() {
  // put your main code here, to run repeatedly:
  if (changeEvent()) {
    idx += 1;
    idx %= pr_count;
  }
  progs[idx]();
}
