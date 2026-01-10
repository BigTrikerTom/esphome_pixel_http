#include "pixel_http.h"

namespace esphome {
namespace pixel_http {

void PixelHttp::setup() {
  server_ = new AsyncServer(9090);
  server_->onClient([this](void *arg, AsyncClient *client) {
    this->client_ = client;
  }, nullptr);
  server_->begin();
}

void PixelHttp::loop() {
  if (!client_ || !client_->connected())
    return;

  if (millis() - last_update_ < 450)
    return;

  last_update_ = millis();
  send_pixels_();
}

void PixelHttp::send_pixels_() {
  if (!light_)
    return;

  uint16_t count = light_->size();
  std::string buffer;
  buffer.reserve(count * 3 + 2);

  buffer.push_back(count & 0xFF);
  buffer.push_back((count >> 8) & 0xFF);

  for (uint16_t i = 0; i < count; i++) {
    auto c = light_->get_pixel(i);
    buffer.push_back(c.r);
    buffer.push_back(c.g);
    buffer.push_back(c.b);
  }

  client_->write(buffer.data(), buffer.size());
}

}  // namespace pixel_http
}  // namespace esphome
