#include "pixel_http.h"

namespace esphome {
namespace pixel_http {

PixelHTTPComponent::PixelHTTPComponent(int num_leds, int pin, int port)
    : num_leds_(num_leds), pin_(pin), port_(port) {
  leds_ = new CRGB[num_leds_];
  server_ = new AsyncWebServer(port_);
}

void PixelHTTPComponent::setup() {
  FastLED.addLeds<WS2812, 27, GRB>(leds_, num_leds_);
  FastLED.clear();
  FastLED.show();

  // /pixels.bin → RAW Framebuffer
  server_->on("/pixels.bin", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send_P(
        200,
        "application/octet-stream",
        (uint8_t *)leds_,
        num_leds_ * sizeof(CRGB)
    );
  });

  // /info.json
  server_->on("/info.json", HTTP_GET, [this](AsyncWebServerRequest *request) {
    String json = "{";
    json += "\"num_leds\":" + String(num_leds_) + ",";
    json += "\"pin\":" + String(pin_) + ",";
    json += "\"port\":" + String(port_);
    json += "}";

    request->send(200, "application/json", json);
  });

  server_->begin();
  ESP_LOGI("pixel_http", "HTTP server started on port %d", port_);
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
