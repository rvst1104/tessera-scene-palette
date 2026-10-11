#pragma once
#include "esphome/core/component.h"
#include "esphome/components/smart_display/plugin_api.h"
#include <array>
#include <string>

#ifndef TESSERA_CARD_CONTROLS
#error "Scene Palette needs the bundled Tessera card-controls.patch. Stock API 0.8 has no card buttons."
#endif

namespace esphome::scene_palette {
class ScenePalette : public Component, public tessera::Plugin {
 public:
  void setup() override;
  void set_scene(unsigned slot, const std::string &entity) { if (slot < scenes_.size()) scenes_[slot] = entity; }
  const std::array<std::string, 6> &scenes() const { return scenes_; }
  void open_palette(const std::array<std::string, 6> &labels, const std::array<std::string, 6> &icons, int tile);
  float get_setup_priority() const override { return setup_priority::DATA; }
 private:
  std::array<std::string, 6> scenes_{}, labels_{}, icons_{};
};
}  // namespace esphome::scene_palette

