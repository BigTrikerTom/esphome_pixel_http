#include "pixel_http.h"

namespace esphome {
namespace pixel_http {

void PixelHttp::setup() {
  server_.begin();
  server_.setNoDelay(true);
}

void PixelHttp::loop() {
  if (!client_ || !client_.connected()) {
    client_ = server_.available();
    return;
  }

  if (millis() - last_update_ < 450)
    return;

  last_update_ = millis();
  send_pixels_();
}

void PixelHttp::send_pixels_() {
  if (!light_ || !client_.connected())
    return;

  uint16_t count = light_->size();

  // Header: LED count (little endian)
  uint8_t header[2];
  header[0] = count & 0xFF;
  header[1] = (count >> 8) & 0xFF;

  client_.write(header, 2);

  // Send pixels
  for (uint16_t i = 0; i < count; i++) {
    auto c = light_->get_pixel(i);
    uint8_t rgb[3] = {c.r, c.g, c.b};
    client_.write(rgb, 3);
  }
}

}  // namespace pixel_http
}  // namespace esphome
