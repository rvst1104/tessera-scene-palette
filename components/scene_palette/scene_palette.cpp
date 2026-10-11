#include "scene_palette.h"
#include "scene_model.h"
#include <algorithm>
#include <vector>

namespace esphome::scene_palette {
using tessera::Font;
namespace ui = tessera::ui;

static uint32_t glyph(const std::string &name) {
  if (name == "bulb") return 0xF0335;
  if (name == "book") return 0xF095F;  // native desk-lamp glyph for reading
  if (name == "movie") return 0xF0FCE;
  if (name == "moon") return 0xF0594;
  return 0xF03D8;
}
static lv_obj_t *line(lv_obj_t *parent, Font font, theme::Role role, int x, int y, int width,
                      const std::string &text) {
  auto *label = ui::label(parent, font, role);
  lv_obj_set_pos(label, x, y);
  lv_obj_set_size(label, std::max(1, width), ui::line_height(font));
  ui::set_text(label, text);
  return label;
}

class PaletteTile : public tessera::Tile {
 public:
  explicit PaletteTile(ScenePalette *plugin) : plugin_(plugin) {}
  void create(const tessera::TileContext &c) override {
    std::array<std::string, 6> entities{}, labels{}, icons{};
    for (size_t i = 0; i < entities.size(); ++i) {
      const auto suffix = std::to_string(i + 1);
      entities[i] = c.options[std::string("scene_") + suffix] | "";
      labels[i] = c.options[std::string("label_") + suffix] | "";
      if (!entities[i].empty() && !valid_scene(entities[i])) invalid_ = true;
    }
    scenes_ = make_selection(entities, labels, icons);
    parent_ = c.parent; width_ = c.width; height_ = c.height;
    const int gap = ui::px(8), foot = ui::line_height(Font::BODY) + gap;
    const int key = std::max(ui::mm(9), ui::line_height(Font::BODY) + 2 * gap);
    const bool inline_navigation = height_ < 2 * key + 2 * gap + foot;
    // Fixed pages keep every target at least 9 mm, including compact landscape glass.
    rows_ = std::max(1, (height_ - foot - key - gap) / (key + gap));
    columns_ = width_ >= 2 * ui::mm(28) + gap ? 2 : 1;
    if (inline_navigation) { rows_ = 1; columns_ = 1; }
    page_size_ = std::max(1, std::min(6, rows_ * columns_));
    rows_ = (page_size_ + columns_ - 1) / columns_;
    const int available = std::max(key, height_ - foot - (inline_navigation ? gap : key + 2 * gap));
    key_height_ = std::min(ui::mm(18), std::max(key, (available - (rows_ - 1) * gap) / rows_));
    const int cell = inline_navigation ? width_ - 2 * (key + gap) : (width_ - (columns_ - 1) * gap) / columns_;
    for (int i = 0; i < page_size_; ++i) {
      auto *block = ui::block(parent_, theme::CARD);
      lv_obj_set_pos(block, inline_navigation ? key + gap : (i % columns_) * (cell + gap), (i / columns_) * (key_height_ + gap));
      lv_obj_set_size(block, cell, key_height_);
      blocks_.push_back(block);
      const Font face = ui::large() ? Font::ICON : Font::ICON_SMALL;
      const int iw = ui::line_height(face), pad = ui::px(8);
      icons_.push_back(line(block, face, theme::ACCENT, pad, (key_height_ - iw) / 2, iw, ""));
      names_.push_back(line(block, Font::BODY, theme::INK, 2 * pad + iw,
                            (key_height_ - ui::line_height(Font::BODY)) / 2, cell - iw - 3 * pad, ""));
    }
    const int nav_y = inline_navigation ? 0 : std::max(rows_ * (key_height_ + gap), height_ - foot - key);
    const int nav_width = inline_navigation ? key : width_ / 2 - gap / 2;
    const int nav_height = inline_navigation ? key_height_ : key;
    previous_ = ui::block(parent_, theme::TRACK);
    next_ = ui::block(parent_, theme::TRACK);
    lv_obj_set_pos(previous_, 0, nav_y);
    lv_obj_set_pos(next_, width_ - nav_width, nav_y);
    lv_obj_set_size(previous_, nav_width, nav_height);
    lv_obj_set_size(next_, nav_width, nav_height);
    const Font nav_font = inline_navigation ? Font::ICON_SMALL : Font::BODY;
    const int nav_text_width = inline_navigation ? ui::line_height(nav_font) : nav_width - 2 * gap;
    const int nav_text_x = inline_navigation ? (nav_width - nav_text_width) / 2 : gap;
    previous_text_ = line(previous_, nav_font, theme::INK, nav_text_x, (nav_height - ui::line_height(nav_font)) / 2,
                           nav_text_width, inline_navigation ? ui::icon(0xF0141) : plugin_->text("previous"));
    next_text_ = line(next_, nav_font, theme::INK, nav_text_x, (nav_height - ui::line_height(nav_font)) / 2,
                       nav_text_width, inline_navigation ? ui::icon(0xF0142) : plugin_->text("next"));
    status_ = line(parent_, Font::BODY, theme::MUTED, 0, height_ - foot + gap, width_, "");
    empty_ = line(parent_, Font::BODY, theme::MUTED, 0, gap, width_, plugin_->text("empty"));
    hint_ = line(parent_, Font::BODY, theme::MUTED, 0, gap + ui::line_height(Font::BODY) + gap,
                 width_, plugin_->text("empty_hint"));
    paint();
  }
  // All child blocks stay non-clickable. The native tile receives the filtered
  // tap; no LVGL touch callbacks or raw-touch subscriptions belong to this plugin.
  void on_tap() override {
    auto *input = lv_indev_active();
    if (!input || lv_indev_get_type(input) != LV_INDEV_TYPE_POINTER) return;
    lv_point_t point;
    lv_indev_get_point(input, &point);
    if (contains(previous_, point)) { dispatch(100); return; }
    if (contains(next_, point)) { dispatch(101); return; }
    for (int i = 0; i < page_size_; ++i)
      if (contains(blocks_[i], point)) { dispatch(static_cast<uint16_t>(i)); return; }
  }
  void dispatch(uint16_t id) {
    if (id == 100 && page_ > 0) { --page_; message_.clear(); paint(); return; }
    if (id == 101 && (page_ + 1) * page_size_ < static_cast<int>(scenes_.size())) {
      ++page_; message_.clear(); paint(); return;
    }
    if (id >= page_size_) return;
    const size_t index = static_cast<size_t>(page_ * page_size_ + id);
    if (index >= scenes_.size()) return;
    const bool sent = tessera::action("scene.turn_on", scenes_[index].entity);
    // An action sent is not confirmation that the scene completed or is currently active.
    message_ = std::string(plugin_->text(sent ? "sent" : "failed")) + " " + scenes_[index].label;
    ui::set_text(status_, message_);
  }
  void on_theme() override { paint(); }
 private:
  static bool contains(lv_obj_t *object, const lv_point_t &point) {
    if (!object || lv_obj_has_flag(object, LV_OBJ_FLAG_HIDDEN)) return false;
    lv_area_t area;
    lv_obj_get_coords(object, &area);
    return point.x >= area.x1 && point.x <= area.x2 && point.y >= area.y1 && point.y <= area.y2;
  }
  void paint() {
    for (int i = 0; i < page_size_; ++i) {
      const size_t index = static_cast<size_t>(page_ * page_size_ + i);
      if (index >= scenes_.size()) { lv_obj_add_flag(blocks_[i], LV_OBJ_FLAG_HIDDEN); continue; }
      lv_obj_remove_flag(blocks_[i], LV_OBJ_FLAG_HIDDEN);
      lv_obj_set_style_bg_color(blocks_[i], ui::color(theme::CARD), 0);
      lv_obj_set_style_bg_color(blocks_[i], ui::color(theme::CARD_PRESSED), LV_STATE_PRESSED);
      ui::set_color(names_[i], theme::INK); ui::set_color(icons_[i], theme::ACCENT);
      ui::set_text(names_[i], scenes_[index].label);
      ui::set_text(icons_[i], ui::icon(glyph(scenes_[index].icon)));
    }
    const bool empty = scenes_.empty();
    for (auto *label : {empty_, hint_}) {
      if (empty) lv_obj_remove_flag(label, LV_OBJ_FLAG_HIDDEN);
      else lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
      ui::set_color(label, theme::MUTED);
    }
    const bool pages = static_cast<int>(scenes_.size()) > page_size_;
    for (auto *block : {previous_, next_}) {
      if (pages) lv_obj_remove_flag(block, LV_OBJ_FLAG_HIDDEN);
      else lv_obj_add_flag(block, LV_OBJ_FLAG_HIDDEN);
      lv_obj_set_style_bg_color(block, ui::color(theme::TRACK), 0);
    }
    if (page_ == 0) lv_obj_add_state(previous_, LV_STATE_DISABLED);
    else lv_obj_remove_state(previous_, LV_STATE_DISABLED);
    if ((page_ + 1) * page_size_ >= static_cast<int>(scenes_.size())) lv_obj_add_state(next_, LV_STATE_DISABLED);
    else lv_obj_remove_state(next_, LV_STATE_DISABLED);
    ui::set_color(previous_text_, theme::INK); ui::set_color(next_text_, theme::INK);
    ui::set_color(status_, theme::MUTED);
    ui::set_text(status_, message_.empty() ? plugin_->text(invalid_ ? "invalid" : empty ? "configure" : "choose") : message_);
  }
  ScenePalette *plugin_;
  std::vector<Scene> scenes_;
  lv_obj_t *parent_{}, *status_{}, *empty_{}, *hint_{}, *previous_{}, *next_{}, *previous_text_{}, *next_text_{};
  std::vector<lv_obj_t *> blocks_, names_, icons_;
  int width_{}, height_{}, page_{}, page_size_{1}, columns_{1}, rows_{1}, key_height_{};
  std::string message_;
  bool invalid_{false};
};

void ScenePalette::setup() {
  add_tile("palette", [this] { return new PaletteTile(this); });
}
}  // namespace esphome::scene_palette
