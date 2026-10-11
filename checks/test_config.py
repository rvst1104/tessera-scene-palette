"""Regression: validate against the UNMODIFIED released Tessera manifest."""
import importlib.util
import os
import unittest
from pathlib import Path
import yaml

PLUGIN = Path(__file__).resolve().parents[1]
path = Path(os.environ.get("TESSERA_MANIFEST", PLUGIN / "checks/stock/plugin_manifest.py"))
spec = importlib.util.spec_from_file_location("stock_manifest", path)
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)

class Configuration(unittest.TestCase):
    def setUp(self):
        self.raw = yaml.safe_load((PLUGIN / "tessera-plugin.yaml").read_text())
        self.words = __import__("json").loads((PLUGIN / "translations/en.json").read_text())
    def test_installs_with_released_validator(self):
        parsed = module.check(self.raw, self.words)
        self.assertEqual(parsed["api"], "0.8")
        self.assertEqual(parsed["inputs"], [])
        self.assertEqual(parsed["cards"], [])
    def test_all_scenes_default_empty(self):
        parsed = module.check(self.raw, self.words)
        options = parsed["tiles"][0]["options"]
        self.assertEqual([o["default"] for o in options if o["id"].startswith("scene_")], [""] * 6)
        self.assertEqual(len(options), 12)
    def test_package_requires_no_values(self):
        self.assertEqual(yaml.safe_load((PLUGIN / "plugin.yaml").read_text()), {"scene_palette": None})
    def test_no_core_extensions_or_raw_tap_handlers(self):
        code = (PLUGIN / "components/scene_palette/scene_palette.cpp").read_text()
        header = (PLUGIN / "components/scene_palette/scene_palette.h").read_text()
        for forbidden in ("ui::control(", "on_control(", "TESSERA_CARD_CONTROLS", "lv_obj_add_event_cb"):
            self.assertNotIn(forbidden, code + header)
        self.assertIn("void on_tap() override", code)

if __name__ == "__main__":
    unittest.main()

