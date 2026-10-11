#pragma once
#include <algorithm>
#include <array>
#include <string>
#include <vector>

namespace esphome::scene_palette {
struct Scene { std::string entity, label, icon; };
inline std::vector<Scene> make_selection(const std::array<std::string, 6> &entities,
                                        const std::array<std::string, 6> &labels,
                                        const std::array<std::string, 6> &icons) {
  std::vector<Scene> out;
  for (size_t i = 0; i < entities.size(); ++i) {
    const auto &entity = entities[i];
    if (entity.empty() || std::any_of(out.begin(), out.end(), [&](const Scene &s) { return s.entity == entity; })) continue;
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
