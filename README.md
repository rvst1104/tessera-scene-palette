# Scene Palette

[Deutsche Anleitung](README.de.md) · [Testing from GitHub](TESTING.md)

Test repository: https://github.com/rvst1104/tessera-scene-palette

A single tile opens your selected Home Assistant scenes. All six slots start empty. Choose only the scenes you want; unused slots stay hidden. Names and icons are per tile, slot order is display order, duplicate entities appear once. Small displays use fixed pages. Buttons report whether the command was sent, never an active scene state.

**Compatibility:** this is a local prototype requiring `compat/card-controls.patch` applied to Tessera core main, including its add-on and editor. Stock Tessera API 0.8 and dev API 0.9 do not expose filtered card controls; the stock editor also requires every input to be filled. The patch adds filtered controls and optional inputs. The manifest names API 0.8 for all other facilities; a compile-time check refuses an unpatched core. Do not install this plugin on stock firmware or flash an untested build.

## Set up

1. Apply `compat/card-controls.patch` to the matching Tessera checkout. See `compat/README.md` for the pinned base revision and checks.
2. Use that patched checkout for the screen's smart_display external component and package source. This must also be used by the preview bench. A plugin folder alone does not replace the core.
3. In the patched Tessera app open Plugins > Add with a link and enter `https://github.com/rvst1104/tessera-scene-palette`. There is no release: Tessera treats the default branch as a test version. Alternatively copy this folder to `/config/tessera-plugins/scene_palette/` and add it from a folder.
4. Leave all six scene inputs empty for an empty palette, or choose scene entities. Rebuild the screen after changing this selection.
5. Add one Scene Palette tile to the page. In its tile options choose short names and icons. Names/icons are layout options and do not require a firmware rebuild.
6. Open the tile and tap a scene. Previous/Next appears when needed. Back uses Tessera's normal navigation.

## Status and testing

Development prototype. No scene is selected by default and no Home Assistant action runs during setup. This folder is not installed on a real panel. Hardware, full ESPHome compilation and official preview acceptance must pass before deployment. Local checks are recorded in `VALIDATION.md`.

## Permissions and memory

Only `scene.turn_on`. No network requests, secrets, sensor subscriptions, Home Assistant helper entities or raw touch callbacks. The manifest reserves 1800 bytes per tile; the temporary detail view creates at most six scene buttons plus navigation and must still be measured on real hardware. Font and colour roles come from Tessera.

