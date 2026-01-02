#include "pixel_http.h"

using namespace esphome;
using namespace esphome::web_server;

namespace pixel_http {

bool PixelHttp::canHandle(WebRequest *request) {
  return request->get_url() == "/pixels.bin";
}

void PixelHttp::handleRequest(WebRequest *request) {
  if (this->light_ == nullptr) {
    request->send(500, "text/plain", "Light not set");
    return;
  }

  const uint16_t count = this->light_->size();
  const size_t len = 2 + count * 3;

  auto *buf = new uint8_t[len];
  buf[0] = count & 0xFF;
  buf[1] = (count >> 8) & 0xFF;

  for (uint16_t i = 0; i < count; i++) {
    auto c = this->light_->get_pixel_color(i);
    buf[2 + i * 3 + 0] = c.red;
    buf[2 + i * 3 + 1] = c.green;
    buf[2 + i * 3 + 2] = c.blue;
  }

  request->send(200, "application/octet-stream", buf, len);
  delete[] buf;
}

}  // namespace pixel_http

