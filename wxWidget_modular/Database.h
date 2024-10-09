#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <mutex>  // Für Thread-Sicherheit
#include <openssl/sha.h>  // Für SHA-256 Hashing

class Database {
public:
    static bool Initialize();

    // Login-Methode: Zweiter Parameter jetzt ohne const, da das Passwort verändert wird
    static bool Login(const std::string& username, std::string password);

    static bool Register(const std::string& username, const std::string& password);

    // Neue Methode zum Abrufen aller Benutzer
    static std::vector<std::pair<std::string, std::string>> FetchAllUsers();  // Deklaration der Methode

private:
    static sqlite3* db;
    static std::string HashPassword(const std::string& password, const std::string& salt);
    static std::string GenerateSalt();  // Methode zur Generierung eines Salts
    static std::mutex dbMutex;  // Mutex für Thread-Sicherheit
};

#endif // DATABASE_H
