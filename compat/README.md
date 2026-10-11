# Local Tessera extension

Base: `MaxGramser/homeassistant_espscreen`, main commit
`8fee7045b048fd4202579e207000e970e88fa57e` (app 0.4.87, firmware 0.55.0).
Development branch in the sibling checkout: `codex/scene-palette-controls`.

`card-controls.patch` contains both filtered detail-card controls and optional manifest
inputs, including editor source and built assets. It is a local prototype, not an
upstream plugin API release. It adds no Home Assistant entities and changes no user
layout. Existing plugin inputs remain required unless explicitly marked optional.

Apply to a clean copy of the exact base:

```sh
git apply --check /path/to/scene_palette/compat/card-controls.patch
git apply /path/to/scene_palette/compat/card-controls.patch
```

The sibling checkout already has these changes; do not apply the patch there again.
To validate applicability in that checkout use `git apply --reverse --check` instead.

Deploying requires both the patched add-on/editor and firmware built from the patched
core. Replacing a plugin folder alone is insufficient. Before installing on a real
panel, run the official preview bench with `TESSERA_TREE` pointing to this core,
compile the actual panel's profile and verify tap, back, swipe and standby on the
glass. No production configuration, selected scene or firmware was changed here.

The extension deliberately keeps the upstream API number at 0.8, with a separate
compile-time capability marker. The manifest's `optional` field also requires the
patched app. Do not submit this plugin to the community index until upstream offers
the necessary capabilities and the manifest can name their official API version.
