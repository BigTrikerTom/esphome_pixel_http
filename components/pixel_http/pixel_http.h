#pragma once
#include "esphome.h"
#include <FastLED.h>

namespace esphome {
namespace pixel_http {

class PixelHTTPComponent : public Component {
 public:
  PixelHTTPComponent();
  void setup() override;
  void loop() override;

  void show();
  void set_pixel(int index, int r, int g, int b);
  void set_pixels_from_json(JsonArray array); // JSON Array für /set

 private:
  CRGB leds[110]; // neue LED-Anzahl
};

}  // namespace pixel_http
}  // namespace esphome
