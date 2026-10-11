# Validation, 2026-10-11

Status: implemented local development prototype, not installed on the panel.
No scenes were selected and no Home Assistant actions were executed.

Passed:

- Official `tools/check.py` using the patched local manifest validator: plugin and dependencies OK, base API 0.8.
- C++20 object compilation of the plugin and the complete modified `plugin_host.cpp`, using real Tessera, ESPHome 2026.9.1, LVGL 9.5.0 and ArduinoJson 7.4.2 headers with host configuration.
- Native C++ selection tests: empty, sparse and duplicate selections, aliases, icons, order and clearing.
- Existing native Tessera touch-guard tests (`tests/test_screen_input.cpp`).
- Five Python tests of the actual ESPHome schema and package substitutions, including all-empty defaults and one selected slot.
- 21 targeted Tessera Python tests covering manifests, compatibility, board fit, layouts, hello and plugin sidecars, including the new optional-input regression.
- 29 editor tests in `tests/plugins.spec.ts` and `tests/stores/plugins.spec.ts`, including empty optional inputs and clearing.
- Editor application and test TypeScript checks; production Vite build. Vite emitted its existing browser-module and large-chunk notices.
- Git whitespace check and reverse patch applicability check against the changed checkout.

Not passed or not performed:

- Official `preview.py --strict` was attempted. WSL only has `docker-desktop`; it has no Python 3 or compiler. The Linux preview could not start. No official preview images or passing preview report exist.
- Full ESPHome firmware linking for the actual panel, flash/RAM measurements and the real panel test remain outstanding. Object compilation is not a firmware build or a physical touch test.
- A broad Python-suite attempt was stopped; only the 21 targeted tests above are claimed.

The compatibility patch includes local add-on/editor changes as well as firmware changes. Stock Tessera cannot install this manifest unchanged, because it lacks optional inputs and card controls. Do not present the prototype as a stock-compatible or hardware-validated plugin.
