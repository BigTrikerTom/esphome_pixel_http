#include "pixel_http.h"

namespace esphome {
namespace pixel_http {

PixelHTTPComponent::PixelHTTPComponent(int num_leds, int pin)
    : num_leds_(num_leds), pin_(pin) {
  leds_ = new CRGB[num_leds_];
}

void PixelHTTPComponent::setup() {
  FastLED.addLeds<WS2812, 27, GRB>(leds_, num_leds_);
  FastLED.clear();
  FastLED.show();
}

void PixelHTTPComponent::loop() {
}

void PixelHTTPComponent::set_pixel(int i, uint8_t r, uint8_t g, uint8_t b) {
  if (i < 0 || i >= num_leds_) return;
  leds_[i] = CRGB(r, g, b);
}

void PixelHTTPComponent::show() {
  FastLED.show();
}

}  // namespace pixel_http
}  // namespace esphome
