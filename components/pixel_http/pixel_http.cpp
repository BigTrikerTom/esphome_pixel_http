#include "pixel_http.h"
#include <ArduinoJson.h>

namespace esphome {
namespace pixel_http {

PixelHTTPComponent::PixelHTTPComponent() {}

void PixelHTTPComponent::setup() {
  FastLED.addLeds<WS2812, 27, GRB>(leds, 110);
  FastLED.clear();
  FastLED.show();
}

void PixelHTTPComponent::loop() {}

void PixelHTTPComponent::show() {
  FastLED.show();
}

void PixelHTTPComponent::set_pixel(int index, int r, int g, int b) {
  if(index < 0 || index >= 110) return;
  leds[index].r = r;
  leds[index].g = g;
  leds[index].b = b;
}

void PixelHTTPComponent::set_pixels_from_json(JsonArray array) {
  for (JsonObject elem : array) {
    int index = elem["index"];
    int r = elem["r"] | 0;
    int g = elem["g"] | 0;
    int b = elem["b"] | 0;
    set_pixel(index, r, g, b);
  }
  show();
}

}  // namespace pixel_http
}  // namespace esphome
