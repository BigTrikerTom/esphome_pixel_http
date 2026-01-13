#include "pixel_http.h"

namespace esphome {
namespace pixel_http {

PixelHTTPComponent::PixelHTTPComponent(int num_leds, int pin, int port)
    : num_leds_(num_leds), pin_(pin), port_(port) {

  framebuffer_.resize(num_leds_);
  strip_ = new ESP32RMTLEDStrip();
  server_ = new AsyncWebServer(port_);
}

void PixelHTTPComponent::setup() {
  // LED Strip initialisieren
  ESP32RMTLEDStripConfig config;
  config.pin = pin_;
  config.num_leds = num_leds_;
  config.type = LED_TYPE_WS2812;
  config.rgb_order = RGB_ORDER_GRB;
  strip_->setup(config);
  strip_->show();

  // HTTP: RAW Framebuffer
  server_->on("/pixels.bin", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send_P(
        200,
        "application/octet-stream",
        (uint8_t *)framebuffer_.data(),
        framebuffer_.size() * sizeof(CRGB)
    );
  });

  // Info Endpoint
  server_->on("/info.json", HTTP_GET, [this](AsyncWebServerRequest *request) {
    String json = "{";
    json += "\"num_leds\":" + String(num_leds_) + ",";
    json += "\"pin\":" + String(pin_) + ",";
    json += "\"port\":" + String(port_);
    json += "}";
    request->send(200, "application/json", json);
  });

  server_->begin();
  ESP_LOGI("pixel_http", "Pixel HTTP server running on port %d", port_);
}

void PixelHTTPComponent::loop() {
  // Framebuffer → LED Strip übertragen
  for (int i = 0; i < num_leds_; i++) {
    strip_->set_pixel_color(i, framebuffer_[i].r, framebuffer_[i].g, framebuffer_[i].b);
  }
  strip_->show();
}

void PixelHTTPComponent::set_pixel(int i, uint8_t r, uint8_t g, uint8_t b) {
  if (i < 0 || i >= num_leds_) return;
  framebuffer_[i] = CRGB(r, g, b);
}

void PixelHTTPComponent::show() {
  for (int i = 0; i < num_leds_; i++) {
    strip_->set_pixel_color(i, framebuffer_[i].r, framebuffer_[i].g, framebuffer_[i].b);
  }
  strip_->show();
}

}  // namespace pixel_http
}  // namespace esphome
