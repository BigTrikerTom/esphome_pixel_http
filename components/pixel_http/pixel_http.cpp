#include "pixel_http.h"

namespace esphome {
namespace pixel_http {

// Konstruktor
PixelHTTPComponent::PixelHTTPComponent(int num_leds, int pin, int port)
    : num_leds_(num_leds), pin_(pin), port_(port) {
  leds_ = new CRGB[num_leds_];           // dynamischer Framebuffer
  server_ = new AsyncWebServer(port_);   // HTTP Server
}

// Setup: FastLED und HTTP Server starten
void PixelHTTPComponent::setup() {
  // FastLED initialisieren
  FastLED.addLeds<WS2812, 27, GRB>(leds_, num_leds_);
  FastLED.clear();
  FastLED.show();

  // /pixels.bin Endpoint: liefert RAW Framebuffer
  server_->on("/pixels.bin", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send_P(
        200,
        "application/octet-stream",
        (uint8_t *)leds_,
        num_leds_ * sizeof(CRGB)
    );
  });

  // /info.json Endpoint: liefert LED-Anzahl, Pin, Port
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

// Loop: optional für Animationen
void PixelHTTPComponent::loop() {
  // keine Hintergrundanimationen aktuell
}

// Einzelnes Pixel setzen
void PixelHTTPComponent::set_pixel(int i, uint8_t r, uint8_t g, uint8_t b) {
  if (i < 0 || i >= num_leds_) return;
  leds_[i] = CRGB(r, g, b);
}

// Framebuffer anzeigen
void PixelHTTPComponent::show() {
  FastLED.show();
}

}  // namespace pixel_http
}  // namespace esphome
