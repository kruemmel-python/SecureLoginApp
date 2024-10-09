#include "Database.h"
#include "Logger.h"
#include <openssl/sha.h>
#include <iostream>
#include <random>  // Für Salt-Generierung
#include <cstring>  // Für std::fill

sqlite3* Database::db = nullptr;
std::mutex Database::dbMutex;  // Initialisiere den Mutex

bool Database::Initialize() {
    std::lock_guard<std::mutex> lock(dbMutex);  // Thread-Sicherheit durch Mutex
    // Versuche, die Datenbank zu öffnen oder zu erstellen
    int rc = sqlite3_open("employees.db", &db);
    if (rc) {
        Logger::Log("Can't open database: " + std::string(sqlite3_errmsg(db)));
        return false;
    }

    // SQL-Abfrage zur Erstellung der Benutzertabelle, falls sie noch nicht existiert
    const char* sql = "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE, "
        "password TEXT, "
        "salt TEXT);";  // Salt wird in der Tabelle gespeichert

    char* errmsg;
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        Logger::Log("SQL error: " + std::string(errmsg));
        sqlite3_free(errmsg);
        return false;
    }

    Logger::Log("Database initialized and user table created (if not exists).");
    return true;
}

// Generiere ein zufälliges Salt
std::string Database::GenerateSalt() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(33, 126);  // ASCII-Werte für druckbare Zeichen
    std::string salt;
    for (int i = 0; i < 16; ++i) {  // Salt von 16 Zeichen Länge
        salt += static_cast<char>(dis(gen));
    }
    return salt;
}

// Verwende SHA-256 für das Hashing mit Salt und Iterationen
std::string Database::HashPassword(const std::string& password, const std::string& salt) {
    std::string saltedPassword = password + salt;
    unsigned char hash[SHA256_DIGEST_LENGTH];

    // Mehrfache Iterationen anwenden, um die Sicherheit zu erhöhen
    for (int i = 0; i < 10000; ++i) {
        SHA256(reinterpret_cast<const unsigned char*>(saltedPassword.c_str()), saltedPassword.length(), hash);
        saltedPassword = std::string(reinterpret_cast<char*>(hash), SHA256_DIGEST_LENGTH);
    }

    // Rückgabe des finalen gehashten Passworts als Hex-String
    std::string hashedPassword;
    char buffer[3];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(buffer, "%02x", hash[i]);
        hashedPassword += buffer;
    }
    return hashedPassword;
}

// Registrierung eines Benutzers mit SHA-256 Hashing und Salt
bool Database::Register(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(dbMutex);  // Thread-Sicherheit

    if (!db) {
        Logger::Log("Database not initialized.");
        return false;
    }

    // Generiere Salt
    std::string salt = GenerateSalt();  // Generiere Salt
    std::string hashedPassword = HashPassword(password, salt);  // SHA-256 Hashing mit Salt

    // SQL-Abfrage zur Registrierung eines neuen Benutzers
    std::string sql = "INSERT INTO users (username, password, salt) VALUES (?, ?, ?);";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        Logger::Log("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
        return false;
    }

    // Binde die Parameter (Benutzername, Passwort und Salt) an die SQL-Abfrage
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hashedPassword.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, salt.c_str(), -1, SQLITE_STATIC);

    // Führe die SQL-Abfrage aus
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        Logger::Log("Failed to execute statement: " + std::string(sqlite3_errmsg(db)));
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    Logger::Log("User " + username + " registered successfully.");
    return true;
}


// Login-Funktion mit Salt und SHA-256 Validierung
bool Database::Login(const std::string& username, std::string password) {
    std::lock_guard<std::mutex> lock(dbMutex);

    if (!db) {
        Logger::Log("Database not initialized.");
        return false;
    }

    // Hole das gespeicherte Passwort und Salt
    std::string sql = "SELECT password, salt FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        Logger::Log("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        Logger::Log("Login failed for user: " + username);
        return false;
    }

    std::string storedHash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    std::string salt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    sqlite3_finalize(stmt);

    // Hash das eingegebene Passwort mit dem Salt
    std::string hashedPassword = HashPassword(password, salt);

    if (hashedPassword == storedHash) {
        Logger::Log("User " + username + " logged in successfully.");
        // Das Passwort nach der Verwendung überschreiben
        std::fill(password.begin(), password.end(), 0);  // Jetzt sollte es funktionieren, da 'password' nicht const ist
        return true;
    }
    else {
        Logger::Log("Login failed for user: " + username);
        return false;
    }
}


// Abrufen aller Benutzer
std::vector<std::pair<std::string, std::string>> Database::FetchAllUsers() {
    std::lock_guard<std::mutex> lock(dbMutex);  // Thread-Sicherheit
    std::vector<std::pair<std::string, std::string>> users;

    if (!db) {
        Logger::Log("Database not initialized.");
        return users;
    }

    std::string sql = "SELECT username, password FROM users;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        Logger::Log("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
        return users;
    }

    // Durch die Ergebnismenge iterieren und die Benutzerdaten sammeln
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        users.push_back(std::make_pair(username, password));
    }

    sqlite3_finalize(stmt);
    return users;
}
