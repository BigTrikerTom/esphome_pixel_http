#include "pixel_http.h"
#include <ArduinoJson.h>

namespace esphome {
namespace pixel_http {

PixelHTTPComponent::PixelHTTPComponent() {}

void PixelHTTPComponent::setup() {
  FastLED.addLeds<WS2812, 27, GRB>(leds, 110);
  FastLED.clear();
  FastLED.show();

  // ESPHome Service registrieren
  App.register_service("pixel_http.set_pixels", [this](std::vector<std::string> args){
    if (args.empty()) return;
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, args[0]);
    if (error) return;
    this->set_pixels_from_json(doc.as<JsonArray>());
  });

  // Optional: Service für Framebuffer-Ausgabe
  App.register_service("pixel_http.get_pixels", [this](std::vector<std::string> args){
    DynamicJsonDocument doc(4096);
    JsonArray arr = doc.to<JsonArray>();
    for(int i=0;i<110;i++){
      JsonObject obj = arr.createNestedObject();
      obj["index"] = i;
      obj["r"] = leds[i].r;
      obj["g"] = leds[i].g;
      obj["b"] = leds[i].b;
    }
    std::string out;
    serializeJson(doc, out);
    App.publish_event("pixel_http.framebuffer", out);
  });
}

void PixelHTTPComponent::loop() {}

void PixelHTTPComponent::show() {
  FastLED.show();
}

void PixelHTTPComponent::set_pixel(int index, int r, int g, int b) {
  if(index < 0 || index >= 110) return;
  leds[index].r = r;
  leds[index].g = g;
  leds[index].b = b;
}

void PixelHTTPComponent::set_pixels_from_json(JsonArray array) {
  for (JsonObject elem : array) {
    int index = elem["index"];
    int r = elem["r"] | 0;
    int g = elem["g"] | 0;
    int b = elem["b"] | 0;
    set_pixel(index, r, g, b);
  }
  show();
}

}  // namespace pixel_http
}  // namespace esphome
