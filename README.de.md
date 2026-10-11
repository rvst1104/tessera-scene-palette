# Szenenpalette

Das Raster muss mindestens zwei Spalten und zwei Zeilen haben. Ein einspaltiges Hochformat-Raster bietet keinen Platz für diese Kachel; verwende ein passendes Raster oder Querformat.

**Ab Version 0.2.0 für unverändertes Tessera (Plugin-API 0.8).** Keine eigene Tessera-Version nötig. Der fehlerhafte Prototyp 0.1.0 wurde ersetzt.

Alle sechs Szenenplätze starten leer. Die Szene-Schaltflächen liegen gemeinsam **innerhalb einer größeren Kachel**. Auf kleinen Displays gibt es mehrere Seiten. Die alte Popup-Ansicht ist entfallen, weil Tessera dafür keine unterstützten Schaltflächen anbietet.

## Einrichtung

1. In Tessera unter „Plugins → Mit einem Link hinzufügen“ `https://github.com/rvst1104/tessera-scene-palette` eintragen.
2. Das Plugin für dein Panel installieren und die Firmware über den normalen Screen Manager bauen.
3. Eine Szenenpalette-Kachel mit mindestens 2×2 Zellen hinzufügen.
4. In ihren Kacheloptionen die gewünschten Szenen-IDs eintragen, z. B. `scene.lesen`. Alle anderen Felder leer lassen. Die IDs findest du in Home Assistants Entitätenliste.
5. Optional kurze Anzeigenamen vergeben. Die Platznummer bestimmt die Reihenfolge.
6. Das Layout speichern. Spätere Änderungen an Szenen und Namen benötigen **keinen Firmware-Neubau**.

Es handelt sich um Textfelder für Szenen-IDs, nicht um ein Auswahlmenü aller vorhandenen Szenen. Alle Schaltflächen verwenden dasselbe Palette-Symbol. „Gesendet“ bestätigt nur den Versand des Befehls.

Falls Tessera noch den alten Beschreibungsfehler zeigt: den fehlgeschlagenen Eintrag entfernen und den Link erneut einlesen. Prüfe, dass Version 0.2.0 angezeigt wird.

Prüfungen und Vorschau stehen unter GitHub Actions. Ein Test am echten Panel ist weiterhin erforderlich; während der Entwicklung wurde keine deiner Szenen gestartet.

