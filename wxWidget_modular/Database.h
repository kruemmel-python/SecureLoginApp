#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <mutex>  // Für Thread-Sicherheit

class Database {
public:
    // Initialisiert die Datenbank und erstellt Tabellen, falls sie nicht existieren
    static bool Initialize();

    // Login und Registrierung für Mitarbeiter
    static bool Login(const std::string& username, std::string password);
    static bool Register(const std::string& username, const std::string& password);

    // Kundenbezogene Methoden
    static bool AddCustomer(const std::string& name, const std::string& adresse);

    // Benutzerdaten abrufen (Mitarbeiter)
    static std::vector<std::pair<int, std::string>> FetchAllUsers();

    // Hashing und Salt-Generierung
    static std::string HashPassword(const std::string& password, const std::string& salt);
    static std::string GenerateSalt();  // Methode zur Generierung eines Salts

private:
    static sqlite3* db;
    static std::mutex dbMutex;  // Mutex für Thread-Sicherheit
};

#endif // DATABASE_H
