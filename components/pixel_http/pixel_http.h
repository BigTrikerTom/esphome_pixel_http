#pragma once
#include "esphome/core/component.h"
#include "esphome/components/light/addressable_light.h"
#include <ESPAsyncWebServer.h>

namespace pixel_http {

class PixelHttp : public esphome::Component,
                  public esphome::web_server::WebHandler {
 public:
  void set_light(esphome::light::AddressableLight *light) {
    this->light_ = light;
  }

  bool canHandle(esphome::web_server::WebRequest *request) override;
  void handleRequest(esphome::web_server::WebRequest *request) override;

 protected:
  esphome::light::AddressableLight *light_{nullptr};
};

}  // namespace pixel_http

