#pragma once
#include <algorithm>
#include <array>
#include <string>
#include <vector>

namespace esphome::scene_palette {
struct Scene { std::string entity, label, icon; };
inline bool valid_scene(const std::string &entity) {
  if (entity.size() <= 6 || entity.size() > 64 || entity.compare(0, 6, "scene.") != 0) return false;
  return std::all_of(entity.begin() + 6, entity.end(), [](char c) {
    return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_';
  });
}
inline std::vector<Scene> make_selection(const std::array<std::string, 6> &entities,
                                        const std::array<std::string, 6> &labels,
                                        const std::array<std::string, 6> &icons) {
  std::vector<Scene> out;
  for (size_t i = 0; i < entities.size(); ++i) {
    const auto &entity = entities[i];
    if (!valid_scene(entity) || std::any_of(out.begin(), out.end(), [&](const Scene &s) { return s.entity == entity; })) continue;
    std::string name = labels[i];
    if (name.empty()) {
      name = entity.substr(6);
      std::replace(name.begin(), name.end(), '_', ' ');
    }
    out.push_back({entity, name, icons[i]});
  }
  return out;
}
}  // namespace esphome::scene_palette
