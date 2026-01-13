#pragma once

#include "esphome.h"
#include <NeoPixelBus.h>
#include <ESPAsyncWebServer.h>

namespace esphome {
namespace pixel_http {

class PixelHTTPComponent : public Component {
 public:
  PixelHTTPComponent(int num_leds, int pin, int port);

  void setup() override;
  void loop() override;

 private:
  int num_leds_;
  int pin_;
  int port_;

  // Framebuffer
  RgbColor *framebuffer_;

  // NeoPixelBus RMT driver
  NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> *strip_;

  // HTTP Server
  AsyncWebServer *server_;
};

}  // namespace pixel_http
}  // namespace esphome
