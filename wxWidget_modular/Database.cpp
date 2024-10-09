#include "Database.h"
#include "Logger.h"
#include <openssl/sha.h>
#include <iostream>
#include <random>
#include <cstring>

sqlite3* Database::db = nullptr;
std::mutex Database::dbMutex;  // Initialisiere den Mutex

bool Database::Initialize() {
    std::lock_guard<std::mutex> lock(dbMutex);

    // Öffne die SQLite-Datenbank
    int rc = sqlite3_open("employees.db", &db);
    if (rc) {
        Logger::Log("Can't open database: " + std::string(sqlite3_errmsg(db)));
        return false;
    }

    // Erstelle die Tabelle für Mitarbeiter
    const char* sql_mitarbeiter = "CREATE TABLE IF NOT EXISTS mitarbeiter ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT, "
        "benutzername TEXT UNIQUE, "
        "passwort TEXT, "
        "salt TEXT);";

    char* errmsg;
    rc = sqlite3_exec(db, sql_mitarbeiter, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        Logger::Log("SQL error (Mitarbeiter): " + std::string(errmsg));
        sqlite3_free(errmsg);
        return false;
    }

    // Erstelle die Tabelle für Kunden
    const char* sql_kunde = "CREATE TABLE IF NOT EXISTS kunden ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT, "
        "adresse TEXT);";

    rc = sqlite3_exec(db, sql_kunde, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        Logger::Log("SQL error (Kunde): " + std::string(errmsg));
        sqlite3_free(errmsg);
        return false;
    }

    // Erstelle die Tabelle für Artikel
    const char* sql_artikel = "CREATE TABLE IF NOT EXISTS artikel ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "titel TEXT, "
        "beschreibung TEXT, "
        "isbn TEXT);";

    rc = sqlite3_exec(db, sql_artikel, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        Logger::Log("SQL error (Artikel): " + std::string(errmsg));
        sqlite3_free(errmsg);
        return false;
    }

    // Erstelle die Tabelle für Verleih
    const char* sql_verleih = "CREATE TABLE IF NOT EXISTS verleih ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "artikelID INTEGER, "
        "kundenID INTEGER, "
        "verliehenAm TEXT, "
        "zurückAm TEXT, "
        "FOREIGN KEY(artikelID) REFERENCES artikel(id), "
        "FOREIGN KEY(kundenID) REFERENCES kunden(id));";

    rc = sqlite3_exec(db, sql_verleih, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        Logger::Log("SQL error (Verleih): " + std::string(errmsg));
        sqlite3_free(errmsg);
        return false;
    }

    Logger::Log("Database initialized successfully.");
    return true;
}

// Registrierung eines Mitarbeiters
bool Database::Register(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(dbMutex);

    std::string salt = GenerateSalt();
    std::string hashedPassword = HashPassword(password, salt);

    std::string sql = "INSERT INTO mitarbeiter (benutzername, passwort, salt) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        Logger::Log("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hashedPassword.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, salt.c_str(), -1, SQLITE_STATIC);

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

// Mitarbeiter-Login
bool Database::Login(const std::string& username, std::string password) {
    std::lock_guard<std::mutex> lock(dbMutex);

    std::string sql = "SELECT passwort, salt FROM mitarbeiter WHERE benutzername = ?;";
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

    std::string hashedPassword = HashPassword(password, salt);

    if (hashedPassword == storedHash) {
        Logger::Log("User " + username + " logged in successfully.");
        std::fill(password.begin(), password.end(), 0);
        return true;
    }
    else {
        Logger::Log("Login failed for user: " + username);
        return false;
    }
}

// Kundenbezogene Funktionen
bool Database::AddCustomer(const std::string& name, const std::string& adresse) {
    std::lock_guard<std::mutex> lock(dbMutex);

    std::string sql = "INSERT INTO kunden (name, adresse) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        Logger::Log("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, adresse.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        Logger::Log("Failed to execute statement: " + std::string(sqlite3_errmsg(db)));
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    Logger::Log("Customer " + name + " added successfully.");
    return true;
}

// Methode zur Generierung eines Salts
std::string Database::GenerateSalt() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(33, 126);
    std::string salt;
    for (int i = 0; i < 16; ++i) {
        salt += static_cast<char>(dis(gen));
    }
    return salt;
}

// Hashing-Funktion mit SHA-256
std::string Database::HashPassword(const std::string& password, const std::string& salt) {
    std::string saltedPassword = password + salt;
    unsigned char hash[SHA256_DIGEST_LENGTH];

    for (int i = 0; i < 10000; ++i) {
        SHA256(reinterpret_cast<const unsigned char*>(saltedPassword.c_str()), saltedPassword.length(), hash);
        saltedPassword = std::string(reinterpret_cast<char*>(hash), SHA256_DIGEST_LENGTH);
    }

    std::string hashedPassword;
    char buffer[3];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(buffer, "%02x", hash[i]);
        hashedPassword += buffer;
    }
    return hashedPassword;
}

// Methode zum Abrufen aller Benutzer
std::vector<std::pair<int, std::string>> Database::FetchAllUsers() {
    std::vector<std::pair<int, std::string>> users;
    std::lock_guard<std::mutex> lock(dbMutex);  // Thread-Sicherheit

    if (!db) {
        Logger::Log("Database not initialized.");
        return users;
    }

    // SQL-Abfrage zum Abrufen aller Benutzer
    std::string sql = "SELECT id, benutzername FROM mitarbeiter;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        Logger::Log("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
        return users;
    }

    // Durch die Ergebnismenge iterieren und die Benutzerdaten sammeln
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        users.push_back(std::make_pair(id, username));
    }

    sqlite3_finalize(stmt);
    return users;
}
