# Local checks

The sibling `tessera-core-upstream` must carry the compatibility patch. Python checks
use ESPHome 2026.9.1 and PyYAML; `checks/test_config.py` exercises the real ESPHome
schema and package/substitution pipeline. Run it with these dependencies on PYTHONPATH.

The C++ check used Zig 0.16.0, real ESPHome headers, LVGL 9.5.0 and ArduinoJson 7.4.2.
`include/esphome/core/defines.h` represents the generated host platform switch; the
API shim loads the actual sibling core header via an include search path. No UI or
Home Assistant API is mocked for compilation. Object compilation checks types and
headers but does not link a full firmware image.

Example with dependencies in the workspace's `.tessera-tooling*` folders:

```sh
zig c++ -std=c++20 -target x86_64-linux-musl -c -w -DLV_CONF_SKIP \
  -I checks/include -I ../tessera-core-upstream/components/smart_display \
  -I ../.tessera-tooling -I ../.tessera-tooling-src/lvgl \
  -I ../.tessera-tooling-src/ArduinoJson/src \
  components/scene_palette/scene_palette.cpp -o checks/plugin.o
```

For `plugin_host.cpp`, also define `ESPHOME_COMPONENT_COUNT=1`, `USE_API`,
`USE_API_PLAINTEXT`, `USE_SOCKET_IMPL_BSD_SOCKETS`, `API_MAX_SEND_QUEUE=8`,
`MAX_API_CONNECTIONS=8`, `USE_API_HOMEASSISTANT_SERVICES`, `USE_TESSERA_PLUGINS`.

Compile and run `checks/test_selection.cpp` natively to test blank selection, sparse
slots, order, aliases, icons, duplicate elimination and clearing. The sibling core's
`tests/test_screen_input.cpp` tests the same touch guard used by the extension.

These checks do not replace the official Linux/macOS preview bench or panel testing.
