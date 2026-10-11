# Testversion von GitHub

Dieses Repository enthält den Plugin-Prototyp und die dafür notwendige Tessera-Erweiterung. Es ist noch kein direkt installierbares Plugin für unverändertes Tessera. Alle sechs Szenenplätze starten leer.

## Voraussetzungen

- Tessera-Basis: `MaxGramser/homeassistant_espscreen` am Commit `8fee7045b048fd4202579e207000e970e88fa57e`.
- Den Patch `compat/card-controls.patch` auf diesen Stand anwenden. Er ergänzt die gefilterten Schaltflächen und optionale Szenenfelder im Editor.
- Sowohl die Tessera-App als auch die Panel-Firmware müssen diesen angepassten Stand verwenden. Nur die Plugin-URL einzutragen reicht nicht.

Die Patch-Anleitung steht in [compat/README.md](compat/README.md). Der Patch enthält den neu gebauten Editor. Er richtet selbst keine Szenen ein, überschreibt kein Layout und startet keine Home-Assistant-Aktionen.

## Plugin hinzufügen

1. In der angepassten Tessera-App „Plugins → Mit einem Link hinzufügen“ öffnen.
2. `https://github.com/rvst1104/tessera-scene-palette` eintragen.
3. Dein Panel auswählen. Alle sechs Szenenfelder können leer bleiben; später lassen sie sich wieder leeren.
4. Die Firmware mit dem angepassten Tessera-Code bauen. Eine geänderte Szenenauswahl benötigt einen erneuten Firmware-Build.
5. Eine Szenenpalette-Kachel hinzufügen. Namen und Symbole in ihren Kacheloptionen ändern.
6. Auf dem Panel zuerst die leere Ansicht, Zurück, Standby und Wischgesten prüfen. Anschließend gewünschte Szenen auswählen und deren Aufruf testen.

Es gibt absichtlich keinen stabilen Release. Tessera kennzeichnet den Hauptzweig als Testversion. Neue Commits können als Updates angeboten werden. „Gesendet“ bedeutet nur, dass der Szenenbefehl verschickt wurde.

## Automatische Prüfungen

Unter GitHub Actions läuft die Prüfung von Plugin, Konfiguration und optionalen Eingaben. Die offizielle Linux-Bildschirmvorschau lässt sich über „Run workflow“ zusätzlich starten; ihre Ergebnisse werden als Artefakt gespeichert. Ein grüner Code-Check ersetzt keinen Test auf dem echten Panel.

Die bisherigen lokalen Prüfergebnisse und offenen Prüfungen stehen in [VALIDATION.md](VALIDATION.md).
