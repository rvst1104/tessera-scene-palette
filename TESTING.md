# Testing version 0.2.0

Use unmodified Tessera with plugin API 0.8. No compatibility patch is required.

Add `https://github.com/rvst1104/tessera-scene-palette` through the normal Screen Manager plugin flow. If it cached 0.1.0, remove the failed entry and read the link again; verify that the description says 0.2.0.

After the normal plugin firmware build, add a 2×2 or larger Scene Palette tile. Leave all scene fields empty to test the initial view. Then enter scene entity IDs and optional short names in the tile options and save the layout. This step does not rebuild firmware.

Check scene taps, pagination, unused slots, duplicate slots, invalid IDs, light/dark theme, swipes and standby on the real panel. Scene buttons are inside the tile; there is no popup card. Only the core-filtered tile tap dispatches actions. No scene is executed by the automated preview.

GitHub Actions uses the stock manifest validator, tests empty options and selection logic, and runs the official strict preview on CYD and Guition. Its images and findings are uploaded as `screen-preview`. Physical panel validation and measured memory usage remain pending.

