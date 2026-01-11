#include "pixel_http.h"

namespace esphome {
namespace pixel_http {

void PixelHttp::setup() {
  server_fd_ = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(9090);
  addr.sin_addr.s_addr = INADDR_ANY;

  bind(server_fd_, (sockaddr *)&addr, sizeof(addr));
  listen(server_fd_, 1);
}

void PixelHttp::loop() {
  if (client_fd_ < 0) {
    sockaddr_in client_addr{};
    socklen_t len = sizeof(client_addr);
    client_fd_ = accept(server_fd_, (sockaddr *)&client_addr, &len);
    return;
  }

  if (millis() - last_update_ < 450)
    return;

  last_update_ = millis();
  send_pixels_();
}

void PixelHttp::send_pixels_() {
  if (!state_ || client_fd_ < 0)
    return;

  if (!state_->get_traits().is_addressable())
    return;

  auto *light = static_cast<light::AddressableLight *>(state_->get_output());
  if (!light)
    return;

  uint16_t count = light->size();

  uint8_t header[2];
  header[0] = count & 0xFF;
  header[1] = (count >> 8) & 0xFF;

  send(client_fd_, header, 2, 0);

  for (uint16_t i = 0; i < count; i++) {
    auto view = (*light)[i];

    uint8_t rgb[3] = {
      (uint8_t)view.get_red(),
      (uint8_t)view.get_green(),
      (uint8_t)view.get_blue()
    };

    send(client_fd_, rgb, 3, 0);
  }
}


}  // namespace pixel_http
}  // namespace esphome
