
# SecureLoginApp

**SecureLoginApp** ist ein C++-Programm, das eine sichere Benutzerverwaltung mit SQLite-Datenbank und einer grafischen Benutzeroberfläche (GUI) auf Basis von wxWidgets bietet. Benutzer können sich registrieren, anmelden und die Datenbankeinträge über die GUI anzeigen. Für die Passwortsicherheit verwenden wir SHA-256-Hashing mit Salt und mehreren Iterationen.

## Funktionen
- **Benutzerregistrierung**: Neue Benutzer können sich mit einem sicheren Passwort registrieren.
- **Benutzeranmeldung**: Registrierte Benutzer können sich über die GUI anmelden.
- **Passwort-Hashing**: Sichere Speicherung von Passwörtern mit SHA-256 und Salt.
- **SQLite-Datenbank**: Alle Benutzerdaten werden in einer SQLite-Datenbank gespeichert.
- **GUI-Unterstützung**: Die Anwendung verwendet wxWidgets für eine benutzerfreundliche Oberfläche.

## Voraussetzungen

Um dieses Projekt auszuführen, benötigst du folgende Software:

- **C++ Compiler** (z.B. GCC, Clang oder MSVC)
- **wxWidgets** (für die GUI)
- **SQLite** (für die Datenbank)
- **OpenSSL** (für SHA-256-Hashing)

### Installation der Bibliotheken:

1. **wxWidgets** installieren: [Offizielle wxWidgets-Dokumentation](https://www.wxwidgets.org/downloads/)
2. **SQLite**: Die SQLite-Bibliothek wird direkt im Programm verwendet. Du musst sicherstellen, dass sie in deinem Projekt integriert ist.
3. **OpenSSL**: Für das Passwort-Hashing wird OpenSSL verwendet. Informationen zur Installation findest du hier: [OpenSSL-Dokumentation](https://www.openssl.org/source/)

## Einrichtung des Projekts

1. **Projekt klonen**: Klone das Repository auf deinen lokalen Rechner.
   ```bash
   git clone https://github.com/username/SecureLoginApp.git
   ```
2. **Build-Konfiguration**: Stelle sicher, dass du wxWidgets und SQLite korrekt in deiner Entwicklungsumgebung eingerichtet hast. 
   
   - Füge die entsprechenden Bibliotheken (wxWidgets, SQLite, OpenSSL) zu den Include- und Bibliothekspfaden deines Projekts hinzu.

3. **Build-Prozess**: Nutze deinen bevorzugten Compiler (GCC, Clang oder MSVC), um das Projekt zu kompilieren und auszuführen. Hier ein Beispiel für einen GCC-Build:
   ```bash
   g++ -o SecureLoginApp main.cpp Database.cpp LoginFrame.cpp MainFrame.cpp -lwx_baseu-3.1 -lwx_gtk2u_core-3.1 -lsqlite3 -lssl -lcrypto
   ```

## Anleitung zur Benutzung

1. **Registrierung**:
   - Öffne das Programm und gebe im Registrierungsformular deinen gewünschten Benutzernamen und ein sicheres Passwort (mindestens 8 Zeichen, inklusive eines Sonderzeichens) ein.
   - Klicke auf **"Register"**. Dein Benutzername und das gehashte Passwort werden in der SQLite-Datenbank gespeichert.

2. **Anmeldung**:
   - Melde dich mit deinem Benutzernamen und Passwort an, indem du die entsprechenden Felder in der GUI ausfüllst.
   - Klicke auf **"Login"**, um dich anzumelden.
   - Bei erfolgreicher Anmeldung wird das Hauptfenster geöffnet, wo du die Benutzerliste anzeigen lassen kannst.

3. **Benutzerdaten anzeigen**:
   - Im Hauptfenster gibt es die Option, alle registrierten Benutzer anzuzeigen. Klicke auf **"Daten abrufen"**, um eine Liste der Benutzer aus der Datenbank anzuzeigen.

## Sicherheitshinweise

- **Passwort-Hashing**: Alle Passwörter werden mit SHA-256 gehasht und zusätzlich mit einem Salt versehen, um die Sicherheit zu erhöhen. Mehrfache Iterationen machen das Knacken des Passworts schwieriger.
- **Thread-Sicherheit**: Durch die Verwendung von Mutexen werden alle Datenbankoperationen thread-sicher durchgeführt.
- **SQL-Injection-Schutz**: SQL-Abfragen verwenden Parameterbindung, um SQL-Injection-Angriffe zu verhindern.

## Weiterentwicklung

- **Passwort-Stärke verbessern**: Derzeit wird nur eine minimale Überprüfung der Passwortsicherheit durchgeführt. Eine zukünftige Verbesserung könnte eine detailliertere Überprüfung der Passwortstärke umfassen.
- **Zwei-Faktor-Authentifizierung**: Eine mögliche Erweiterung könnte die Implementierung einer Zwei-Faktor-Authentifizierung (2FA) sein.
- **Log-Funktion erweitern**: Die Log-Dateien können detaillierter gestaltet werden, um Debugging zu erleichtern.

## Lizenz

Dieses Projekt steht unter der [MIT-Lizenz](LICENSE).
```
