#pragma once
#include "esphome/core/component.h"
#include "esphome/components/smart_display/plugin_api.h"
namespace esphome::scene_palette {
class ScenePalette : public Component, public tessera::Plugin {
 public:
  void setup() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
};
}  // namespace esphome::scene_palette
