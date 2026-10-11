#include "scene_palette.h"
#include "scene_model.h"
#include <algorithm>
#include <vector>

namespace esphome::scene_palette {
using tessera::Font;
namespace ui = tessera::ui;

static std::vector<Scene> selected(const ScenePalette *plugin,
                                  const std::array<std::string, 6> &labels,
                                  const std::array<std::string, 6> &icons) {
  return make_selection(plugin->scenes(), labels, icons);
}
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

class PaletteCard : public tessera::Card {
 public:
  PaletteCard(ScenePalette *plugin, std::vector<Scene> scenes) : plugin_(plugin), scenes_(std::move(scenes)) {}
  void open(const tessera::CardContext &c) override {
    parent_ = c.parent; width_ = c.width; height_ = c.height;
    const int gap = ui::px(8), foot = ui::line_height(Font::BODY) + gap;
    const int key = std::max(ui::mm(9), ui::line_height(Font::BODY) + 2 * gap);
    // Fixed pages keep every target at least 9 mm, including compact landscape glass.
    rows_ = std::max(1, (height_ - foot - key - gap) / (key + gap));
    columns_ = width_ >= 2 * ui::mm(28) + gap ? 2 : 1;
    page_size_ = std::max(1, std::min(6, rows_ * columns_));
    rows_ = (page_size_ + columns_ - 1) / columns_;
    const int available = std::max(key, height_ - foot - key - 2 * gap);
    key_height_ = std::min(ui::mm(18), std::max(key, (available - (rows_ - 1) * gap) / rows_));
    const int cell = (width_ - (columns_ - 1) * gap) / columns_;
    for (int i = 0; i < page_size_; ++i) {
      auto *block = ui::block(parent_, theme::CARD);
      lv_obj_set_pos(block, (i % columns_) * (cell + gap), (i / columns_) * (key_height_ + gap));
      lv_obj_set_size(block, cell, key_height_);
      ui::control(block, static_cast<uint16_t>(i));
      blocks_.push_back(block);
      const Font face = ui::large() ? Font::ICON : Font::ICON_SMALL;
      const int iw = ui::line_height(face), pad = ui::px(8);
      icons_.push_back(line(block, face, theme::ACCENT, pad, (key_height_ - iw) / 2, iw, ""));
      names_.push_back(line(block, Font::BODY, theme::INK, 2 * pad + iw,
                            (key_height_ - ui::line_height(Font::BODY)) / 2, cell - iw - 3 * pad, ""));
    }
    const int nav_y = std::max(rows_ * (key_height_ + gap), height_ - foot - key);
    previous_ = ui::block(parent_, theme::TRACK);
    next_ = ui::block(parent_, theme::TRACK);
    lv_obj_set_pos(previous_, 0, nav_y);
    lv_obj_set_pos(next_, width_ / 2 + gap / 2, nav_y);
    lv_obj_set_size(previous_, width_ / 2 - gap / 2, key);
    lv_obj_set_size(next_, width_ / 2 - gap / 2, key);
    ui::control(previous_, 100); ui::control(next_, 101);
    previous_text_ = line(previous_, Font::BODY, theme::INK, gap, (key - ui::line_height(Font::BODY)) / 2,
                           width_ / 2 - 3 * gap, plugin_->text("previous"));
    next_text_ = line(next_, Font::BODY, theme::INK, gap, (key - ui::line_height(Font::BODY)) / 2,
                       width_ / 2 - 3 * gap, plugin_->text("next"));
    status_ = line(parent_, Font::BODY, theme::MUTED, 0, height_ - foot + gap, width_, "");
    empty_ = line(parent_, Font::BODY, theme::MUTED, 0, gap, width_, plugin_->text("empty"));
    hint_ = line(parent_, Font::BODY, theme::MUTED, 0, gap + ui::line_height(Font::BODY) + gap,
                 width_, plugin_->text("empty_hint"));
    paint();
  }
  void on_control(uint16_t id) override {
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
    ui::set_text(status_, message_.empty() && !empty ? plugin_->text("choose") : message_);
  }
  ScenePalette *plugin_;
  std::vector<Scene> scenes_;
  lv_obj_t *parent_{}, *status_{}, *empty_{}, *hint_{}, *previous_{}, *next_{}, *previous_text_{}, *next_text_{};
  std::vector<lv_obj_t *> blocks_, names_, icons_;
  int width_{}, height_{}, page_{}, page_size_{1}, columns_{1}, rows_{1}, key_height_{};
  std::string message_;
};

class PaletteTile : public tessera::Tile {
 public:
  explicit PaletteTile(ScenePalette *plugin) : plugin_(plugin) {}
  void create(const tessera::TileContext &c) override {
    tile_ = c.tile;
    for (size_t i = 0; i < 6; ++i) {
      const auto suffix = std::to_string(i + 1);
      labels_[i] = c.options[std::string("label_") + suffix] | "";
      icons_[i] = c.options[std::string("icon_") + suffix] | "palette";
    }
    const auto scenes = selected(plugin_, labels_, icons_);
    const Font face = ui::large() ? Font::ICON : Font::ICON_SMALL;
    const int ih = ui::line_height(face), bh = ui::line_height(Font::BODY), gap = ui::px(4);
    const int top = std::max(0, (c.height - ih - bh - gap) / 2);
    icon_ = line(c.parent, face, theme::ACCENT, 0, top, c.width, ui::icon(glyph("palette")));
    count_ = line(c.parent, Font::BODY, theme::MUTED, 0, top + ih + gap, c.width,
                   scenes.empty() ? plugin_->text("tile_empty") : tessera::format(plugin_->text("count"), scenes.size()));
    for (auto *label : {icon_, count_}) lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
  }
  void on_tap() override { plugin_->open_palette(labels_, icons_, tile_); }
  void on_theme() override { ui::set_color(icon_, theme::ACCENT); ui::set_color(count_, theme::MUTED); }
 private:
  ScenePalette *plugin_;
  int tile_{-1};
  std::array<std::string, 6> labels_{}, icons_{};
  lv_obj_t *icon_{}, *count_{};
};

void ScenePalette::setup() {
  add_tile("palette", [this] { return new PaletteTile(this); });
  add_card("scenes", [this] { return new PaletteCard(this, selected(this, labels_, icons_)); });
}
void ScenePalette::open_palette(const std::array<std::string, 6> &labels,
                               const std::array<std::string, 6> &icons, int tile) {
  labels_ = labels; icons_ = icons;
  tessera::open_card(plugin_id(), "scenes", "", tile, text("title"));
}
}  // namespace esphome::scene_palette

