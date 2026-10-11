# Validation of 0.2.0

This version removes the unsupported optional-input schema and custom card-controls API. It uses no inputs, no cards, six empty-by-default text options for scene IDs and six for names. All actions run from the standard Tile::on_tap callback.

Local validation uses the original released API header and manifest validator from Tessera commit 8fee7045b048fd4202579e207000e970e88fa57e, not the earlier modified checkout.

GitHub Actions checks the plugin and scene-selection regressions and runs the official strict preview against unmodified Tessera. See the current run for results; local object compilation is not a firmware link or a physical panel test. No real Home Assistant actions are sent during automated checks. Physical testing and measured RAM/flash usage remain outstanding.

