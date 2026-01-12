#include "pixel_http.h"
#include <ArduinoJson.h> // Für JSON Parsing

namespace esphome {
namespace pixel_http {

PixelHTTPComponent::PixelHTTPComponent() : server(nullptr), ws(nullptr) {}

void PixelHTTPComponent::setup() {
  // FastLED Setup
  FastLED.addLeds<WS2812, 27, GRB>(leds, 120);
  FastLED.clear();
  FastLED.show();

  // AsyncWebServer auf Port 81
  server = new AsyncWebServer(81); // Port geändert von 80 auf 81
  ws = new AsyncWebSocket("/ws");

  setupHttpEndpoints();
  setupWebSocket();

  server->begin();
}

// ---------------- HTTP ----------------
void PixelHTTPComponent::setupHttpEndpoints() {
  server->on("/pixels.bin", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send_P(200, "application/octet-stream", (char*)leds, 120 * 3);
  });

  server->on("/info.json", HTTP_GET, [this](AsyncWebServerRequest *request){
    String json = "{\"led_count\": 120}";
    request->send(200, "application/json", json);
  });

  // JSON /set
  server->on("/set", HTTP_POST, [this](AsyncWebServerRequest *request){
    if (request->hasParam("body", true)) {
      AsyncWebParameter* p = request->getParam("body", true);
      const char* body = p->value().c_str();

      DynamicJsonDocument doc(4096); // Größe anpassen je nach LED-Anzahl
      DeserializationError error = deserializeJson(doc, body);
      if (error) {
        request->send(400, "application/json", "{\"error\":\"invalid JSON\"}");
        return;
      }

      bool updated = false;
      for (JsonObject elem : doc.as<JsonArray>()) {
        int index = elem["index"];
        if (index < 0 || index >= 120) continue;
        leds[index].r = elem["r"] | 0;
        leds[index].g = elem["g"] | 0;
        leds[index].b = elem["b"] | 0;
        updated = true;
      }

      if (updated) {
        FastLED.show();
        // Push update an WebSocket Clients
        sendWsUpdate();
      }

      request->send(200, "application/json", "{\"status\":\"ok\"}");
    } else {
      request->send(400, "application/json", "{\"error\":\"no body\"}");
    }
  });
}

// ---------------- WebSocket ----------------
void PixelHTTPComponent::setupWebSocket() {
  ws->onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client,
                     AwsEventType type, void *arg, uint8_t *data, size_t len){
    if(type == WS_EVT_CONNECT){
      ESP_LOGD("PixelHTTP", "WS Client connected: %u", client->id());
    } else if(type == WS_EVT_DISCONNECT){
      ESP_LOGD("PixelHTTP", "WS Client disconnected: %u", client->id());
    } else if(type == WS_EVT_DATA){
      // JSON empfangen
      DynamicJsonDocument doc(4096);
      DeserializationError error = deserializeJson(doc, data, len);
      if (!error) {
        for (JsonObject elem : doc.as<JsonArray>()) {
          int index = elem["index"];
          if (index < 0 || index >= 120) continue;
          leds[index].r = elem["r"] | 0;
          leds[index].g = elem["g"] | 0;
          leds[index].b = elem["b"] | 0;
        }
        FastLED.show();
        // Broadcast an alle Clients
        sendWsUpdate();
      }
    }
  });
  server->addHandler(ws);
}

// ---------------- Hilfsfunktion ----------------
void PixelHTTPComponent::sendWsUpdate() {
  // sendet kompletten Framebuffer als Base64 JSON
  size_t buf_size = 120 * 3;
  uint8_t buffer[buf_size];
  for(int i=0;i<120;i++){
    buffer[i*3]   = leds[i].r;
    buffer[i*3+1] = leds[i].g;
    buffer[i*3+2] = leds[i].b;
  }

  // Base64 encodieren
  String encoded = base64::encode(buffer, buf_size);

  String msg = "{\"type\":\"full\",\"data\":\"" + encoded + "\"}";
  ws->textAll(msg);
}

}  // namespace pixel_http
}  // namespace esphome
