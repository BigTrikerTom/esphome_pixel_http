#pragma once

#include "esphome.h"
#include "FastLED.h"
#include <ESPAsyncWebServer.h>  // Für HTTP Server

namespace esphome {
namespace pixel_http {

class PixelHTTPComponent : public Component {
 public:
  PixelHTTPComponent(int num_leds, int pin, int port);

  void setup() override;
  void loop() override;

  void set_pixel(int i, uint8_t r, uint8_t g, uint8_t b);
  void show();

 private:
  int num_leds_;
  int pin_;
  int port_;

  CRGB *leds_;
  AsyncWebServer *server_;
};

}  // namespace pixel_http
}  // namespace esphome
