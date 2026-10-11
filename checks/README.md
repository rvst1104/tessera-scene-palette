# Checks

Use the official checker against the unmodified core. TESSERA_MANIFEST must name the stock plugin_manifest.py. Run checks/test_config.py for regression coverage of stock manifest acceptance, no required inputs, empty defaults and absence of custom core APIs.

Compile and run checks/test_selection.cpp for empty, duplicate, ordered and invalid scene selection. GitHub Actions runs these checks and the official strict firmware preview. The local checks/include API shim resolves the stock API header from an explicit include path. Generated stock headers and test binaries are excluded from Git.

