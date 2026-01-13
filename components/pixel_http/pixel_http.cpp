#include "pixel_http.h"

namespace esphome {
namespace pixel_http {

PixelHTTPComponent::PixelHTTPComponent(int num_leds, int pin, int port)
    : num_leds_(num_leds), pin_(pin), port_(port) {

  framebuffer_ = new RgbColor[num_leds_];
  strip_ = new NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>(num_leds_, pin_);
  server_ = new AsyncWebServer(port_);
}

void PixelHTTPComponent::setup() {
  // LED Driver starten
  strip_->Begin();
  strip_->Show();

  // Framebuffer initialisieren (alles aus)
  for (int i = 0; i < num_leds_; i++) {
    framebuffer_[i] = RgbColor(0, 0, 0);
  }

  // HTTP: RAW Framebuffer Dump
  server_->on("/pixels.bin", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send_P(
        200,
        "application/octet-stream",
        (uint8_t *)framebuffer_,
        num_leds_ * sizeof(RgbColor)
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
  // Framebuffer → Strip übertragen
  for (int i = 0; i < num_leds_; i++) {
    strip_->SetPixelColor(i, framebuffer_[i]);
  }
  strip_->Show();
}

}  // namespace pixel_http
}  // namespace esphome
