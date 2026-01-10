#pragma once

#include "esphome/core/component.h"
#include "esphome/components/light/addressable_light.h"

#include "lwip/sockets.h"
#include "lwip/netdb.h"

namespace esphome {
namespace pixel_http {

class PixelHttp : public Component {
 public:
  void set_light(light::AddressableLight *light) { light_ = light; }
  void setup() override;
  void loop() override;

 protected:
  light::AddressableLight *light_{nullptr};

  int server_fd_{-1};
  int client_fd_{-1};

  uint32_t last_update_{0};

  void send_pixels_();
};

}  // namespace pixel_http
}  // namespace esphome
