#include "../components/scene_palette/scene_model.h"
#include <cassert>

int main() {
  using namespace esphome::scene_palette;
  std::array<std::string, 6> entities{}, labels{}, icons{};
  assert(make_selection(entities, labels, icons).empty());
  for (const auto &invalid : {"light.example", "scene.", "scene.UPPER", "scene.bad id", "scene.a;other"})
    assert(!valid_scene(invalid));
  assert(valid_scene("scene.valid_1"));
  entities[1] = "scene.reading_time";
  entities[3] = "scene.cinema";
  labels[3] = "Film"; icons[3] = "movie";
  entities[5] = entities[1]; labels[5] = "Duplicate";
  auto picked = make_selection(entities, labels, icons);
  assert(picked.size() == 2);
  assert(picked[0].entity == entities[1]);
  assert(picked[0].label == "reading time");
  assert(picked[1].label == "Film" && picked[1].icon == "movie");
  entities[1].clear();
  picked = make_selection(entities, labels, icons);
  assert(picked[0].entity == "scene.cinema");
  assert(picked[1].label == "Duplicate");
  entities[0] = "light.not_a_scene";
  assert(make_selection(entities, labels, icons).size() == 2);
}
