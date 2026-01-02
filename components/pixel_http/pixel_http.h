#pragma once

#include "esphome/core/component.h"
#include "esphome/components/light/addressable_light.h"
#include <ESPAsyncWebServer.h>

namespace esphome {
namespace pixel_http {

class PixelHttp : public Component {
 public:
  void set_light(light::AddressableLight *light) { light_ = light; }
  void setup() override;

 protected:
  light::AddressableLight *light_{nullptr};
};

}  // namespace pixel_http
}  // namespace esphome
