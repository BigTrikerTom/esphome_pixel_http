#pragma once

#include "esphome.h"
#include "esp32_rmt_led_strip/esp32_rmt_led_strip.h"
#include <ESPAsyncWebServer.h>

namespace esphome {
namespace pixel_http {

class PixelHTTPComponent : public Component {
 public:
  PixelHTTPComponent(int num_leds, int pin, int port);

  void setup() override;
  void loop() override;

  // Pixel manipulieren
  void set_pixel(int i, uint8_t r, uint8_t g, uint8_t b);
  void show();

 private:
  int num_leds_;
  int pin_;
  int port_;

  ESP32RMTLEDStrip *strip_;
  std::vector<CRGB> framebuffer_;
  AsyncWebServer *server_;
};

}  // namespace pixel_http
}  // namespace esphome
