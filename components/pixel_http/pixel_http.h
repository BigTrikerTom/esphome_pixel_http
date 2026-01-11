#pragma once

#include "esphome.h"
#include <FastLED.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>

namespace esphome {
namespace pixel_http {

class PixelHTTPComponent : public Component {
 public:
  PixelHTTPComponent();
  void setup() override;
  void loop() override;

  void show();

 private:
  void setupHttpEndpoints();
  void setupWebSocket();

  CRGB leds[120];  // Anzahl LEDs anpassen
  AsyncWebServer *server;
  AsyncWebSocket *ws;
};

}  // namespace pixel_http
}  // namespace esphome
