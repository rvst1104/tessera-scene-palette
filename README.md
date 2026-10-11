# Scene Palette

[Deutsche Anleitung](README.de.md)

Version 0.2.0 fixes installation on **unmodified Tessera plugin API 0.8**. No patched Screen Manager, special firmware source, or required setup inputs. The old 0.1.0 prototype incorrectly required changes to Tessera and must not be used.

Six scene slots start empty. The scenes appear together **inside one larger tile**, with fixed pages when necessary. This replaces the prototype's unsupported popup. Each scene has an optional display name. Scene choices are entered as entity IDs in the tile options, rather than an entity-picker dropdown.

## Set up

1. In Tessera Screen Manager, open Plugins > Add with a link and enter `https://github.com/rvst1104/tessera-scene-palette`.
2. Install the plugin on your panel and build its firmware through the normal Screen Manager flow.
3. Add a Scene Palette tile, sized at least 2×2 cells.
4. In the tile options, fill only the scene slots you want, for example `scene.reading`. Leave the others empty. The entity IDs are available in Home Assistant's entity list.
5. Set optional short names. Slot numbers define the order. Duplicate scenes appear once; malformed IDs are ignored and reported.
6. Save the layout. Scene/name changes now need **no firmware rebuild**. Tap a scene to send `scene.turn_on`; use Previous/Next for more scenes.

No scene runs during installation. “Sent” confirms dispatch only, not that a scene remains active. No custom per-scene icons in this version; all buttons use Tessera's native palette icon.

## Validation

The official checker and regression tests use the unmodified upstream manifest validator. The C++ code compiles against the unmodified API header. GitHub Actions runs checks and the official strict preview on small and larger screens. Current run status is available on the repository's Actions tab. No physical panel test is claimed.

Only `scene.turn_on` is requested. No network connections, secrets, custom touch handlers or Home Assistant helpers. Layout memory reserves 6000 bytes per tile; physical flash/RAM measurements remain pending.

