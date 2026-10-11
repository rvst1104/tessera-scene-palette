"""Use the actual ESPHome schema, without contacting Home Assistant."""
import importlib.util
import sys
import unittest
from pathlib import Path
import esphome.components
import esphome.config_validation as cv
import yaml
from esphome.yaml_util import load_yaml
from esphome.components.packages import do_packages_pass, merge_packages
from esphome.components.substitutions import do_substitution_pass

PLUGIN = Path(__file__).resolve().parents[1]
CORE = PLUGIN.parent / "tessera-core-upstream"
esphome.components.__path__.append(str(CORE / "components"))
spec = importlib.util.spec_from_file_location("scene_palette_config", PLUGIN / "components/scene_palette/__init__.py")
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)

class Configuration(unittest.TestCase):
    def test_empty_schema(self):
        config = module.CONFIG_SCHEMA({})
        self.assertEqual([config[f"scene_{i}"] for i in range(1, 7)], [""] * 6)

    def test_optional_scene_domain(self):
        self.assertEqual(module.optional_scene(" scene.example "), "scene.example")
        for value in ["light.example", "scene.", "scene.example extra", "scene.X", 123]:
            with self.subTest(value=value), self.assertRaises(cv.Invalid):
                module.optional_scene(value)

    def test_package_defaults_are_empty(self):
        package = yaml.safe_load((PLUGIN / "plugin.yaml").read_text())
        self.assertEqual(package["defaults"], {f"SCENE_{i}": "" for i in range(1, 7)})

    def package(self, name):
        config = load_yaml(PLUGIN / "checks" / name)
        config = do_packages_pass(config)
        config = do_substitution_pass(config)
        return merge_packages(config)["scene_palette"]

    def test_esphome_expands_empty_defaults(self):
        self.assertEqual(self.package("package-empty.yaml"), {f"scene_{i}": "" for i in range(1, 7)})

    def test_esphome_keeps_unselected_slots_empty(self):
        expected = {f"scene_{i}": "" for i in range(1, 7)}
        expected["scene_2"] = "scene.example"
        self.assertEqual(self.package("package-one.yaml"), expected)

if __name__ == "__main__":
    unittest.main()
