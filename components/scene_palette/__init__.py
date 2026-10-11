"""Six optional scene selectors. Selection changes require a firmware rebuild."""
import esphome.codegen as cg
import re
import esphome.config_validation as cv
from esphome.components import smart_display
from esphome.const import CONF_ID

DEPENDENCIES = ["smart_display"]
ns = cg.esphome_ns.namespace("scene_palette")
ScenePalette = ns.class_("ScenePalette", cg.Component)

def optional_scene(value):
    value = cv.string_strict(value).strip()
    if value and not re.fullmatch(r"scene\.[a-z0-9_]+", value):
        raise cv.Invalid("Choose a scene entity or leave this slot empty")
    return value

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ScenePalette),
    **{cv.Optional(f"scene_{i}", default=""): optional_scene for i in range(1, 7)},
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    for i in range(1, 7):
        cg.add(var.set_scene(i - 1, config[f"scene_{i}"]))
    await smart_display.register_plugin(var, __file__)

