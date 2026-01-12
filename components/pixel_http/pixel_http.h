#pragma once
#include "esphome.h"
#include <FastLED.h>

namespace esphome {
namespace pixel_http {

class PixelHTTPComponent : public Component {
 public:
  PixelHTTPComponent(int num_leds);   // Konstruktor mit LED-Anzahl
  void setup() override;
  void loop() override;

  void show();
  void set_pixel(int index, int r, int g, int b);
  void set_pixels_from_json(JsonArray array);

  int get_num_leds() const { return NUM_LEDS; }

 private:
  int NUM_LEDS;
  CRGB* leds;  // Dynamisch allociert
};

}  // namespace pixel_http
}  // namespace esphome
