#include "Kunde.h"
#include "Database.h"
#include "Logger.h"

Kunde::Kunde(int id, const std::string& name, const std::string& adresse)
    : kundenID(id), name(name), adresse(adresse) {}

// Kunde anlegen
bool Kunde::KundeAnlegen() {
    // SQL-Code zum Hinzufügen eines Kunden in die Datenbank
    Logger::Log("Kunde " + name + " wurde angelegt.");
    return true;
}

// Kunde bearbeiten
bool Kunde::KundeBearbeiten() {
    // SQL-Code zum Bearbeiten eines Kunden
    Logger::Log("Kunde " + name + " wurde bearbeitet.");
    return true;
}

// Kunde löschen
bool Kunde::KundeLöschen() {
    // SQL-Code zum Löschen eines Kunden
    Logger::Log("Kunde " + name + " wurde gelöscht.");
    return true;
}
