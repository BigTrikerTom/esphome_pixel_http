#include "pixel_http.h"

namespace esphome {
namespace pixel_http {

PixelHTTPComponent::PixelHTTPComponent() : server(nullptr), ws(nullptr) {}

void PixelHTTPComponent::setup() {
  FastLED.addLeds<WS2812, 27, GRB>(leds, 120);  // LED_PIN und NUM_LEDS anpassen
  FastLED.clear();
  FastLED.show();

  server = new AsyncWebServer(80);
  ws = new AsyncWebSocket("/ws");

  setupHttpEndpoints();
  setupWebSocket();

  server->begin();
}

void PixelHTTPComponent::loop() {
  ws->cleanupClients();
}

void PixelHTTPComponent::setupHttpEndpoints() {
  server->on("/pixels.bin", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send_P(200, "application/octet-stream", (char*)leds, 120 * 3);
  });

  server->on("/info.json", HTTP_GET, [this](AsyncWebServerRequest *request){
    String json = "{\"led_count\": 120}";
    request->send(200, "application/json", json);
  });

  server->on("/set", HTTP_POST, [this](AsyncWebServerRequest *request){
    // später JSON parsen und leds[] updaten
    request->send(200, "application/json", "{\"status\":\"ok\"}");
  });
}

void PixelHTTPComponent::setupWebSocket() {
  ws->onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client,
                     AwsEventType type, void *arg, uint8_t *data, size_t len){
    if(type == WS_EVT_CONNECT){
      ESP_LOGD("PixelHTTP", "WS Client connected: %u", client->id());
    } else if(type == WS_EVT_DISCONNECT){
      ESP_LOGD("PixelHTTP", "WS Client disconnected: %u", client->id());
    } else if(type == WS_EVT_DATA){
      // JSON empfangen und leds[] updaten
    }
  });
  server->addHandler(ws);
}

void PixelHTTPComponent::show() {
  FastLED.show();
}

}  // namespace pixel_http
}  // namespace esphome
