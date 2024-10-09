#include "Mitarbeiter.h"
#include "Database.h"
#include "Logger.h"

Mitarbeiter::Mitarbeiter(int id, const std::string& name, const std::string& benutzername, const std::string& passwort)
    : mitarbeiterID(id), name(name), benutzername(benutzername), passwort(passwort) {}

// Registrierung eines neuen Mitarbeiters
bool Mitarbeiter::Registrieren() {
    return Database::Register(benutzername, passwort);
}

// Login für einen Mitarbeiter
bool Mitarbeiter::Login() {
    return Database::Login(benutzername, passwort);
}

// Logout für einen Mitarbeiter (aktuell nur Logging)
bool Mitarbeiter::Logout() {
    Logger::Log("Mitarbeiter " + benutzername + " hat sich ausgeloggt.");
    return true;
}
