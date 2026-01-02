#include "pixel_http.h"

namespace esphome {
namespace pixel_http {

static AsyncWebServer server(8080);

void PixelHttp::setup() {
  server.on("/pixels", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(200, "application/json", "[]");
  });

  server.begin();
}

}  // namespace pixel_http
}  // namespace esphome

