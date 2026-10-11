# Szenenpalette

Eine Kachel öffnet deine selbst ausgewählten Szenen. Alle sechs Plätze sind anfangs leer. Leere Plätze bleiben unsichtbar; gleiche Szenen erscheinen nur einmal. Die Platznummer bestimmt die Reihenfolge. Namen und Symbole stellst du in den Kacheloptionen ein.

**Entwicklungsstand:** Das Plugin benötigt die beiliegende Tessera-Erweiterung `compat/card-controls.patch`. Es läuft noch nicht mit unverändertem Tessera und ist nicht auf deinem Panel installiert.

GitHub-Testversion: https://github.com/rvst1104/tessera-scene-palette

Die genaue Testanleitung steht in [TESTING.md](TESTING.md).

## Einrichtung

1. Die Erweiterung auf den in `compat/README.md` genannten Tessera-Stand anwenden und diesen Stand als Firmware-Quelle verwenden.
2. Im angepassten Tessera unter „Plugins → Mit einem Link hinzufügen“ die Adresse `https://github.com/rvst1104/tessera-scene-palette` eingeben. Ohne Release verwendet Tessera den Hauptzweig als Testversion. Alternativ den Ordner nach `/config/tessera-plugins/scene_palette/` kopieren und als lokales Plugin hinzufügen.
3. Die sechs Szenenfelder leer lassen oder die gewünschten Szenen auswählen. Eine geänderte Szenenauswahl erfordert ein erneutes Erstellen der Firmware.
4. Eine Kachel „Szenenpalette“ hinzufügen. Eigene Namen und Symbole in ihren Optionen wählen; diese Änderungen benötigen keinen Firmware-Neubau.
5. Die Kachel öffnet die gemeinsame Auswahl. Kleine Displays erhalten bei Bedarf mehrere Seiten.

„Gesendet“ bestätigt nur den Versand des Befehls. Szenen haben keinen zuverlässigen Zustand „aktiv“. Vorschau, vollständiger Firmware-Build und Prüfung am echten Panel stehen noch aus; siehe `VALIDATION.md`.

