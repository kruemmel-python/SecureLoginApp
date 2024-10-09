#ifndef MITARBEITER_H
#define MITARBEITER_H

#include <string>

class Mitarbeiter {
protected:
    int mitarbeiterID;
    std::string name;
    std::string benutzername;
    std::string passwort;

public:
    Mitarbeiter(int id, const std::string& name, const std::string& benutzername, const std::string& passwort);

    // Methoden für Mitarbeiter
    bool Registrieren();
    bool Login();
    bool Logout();
};

#endif // MITARBEITER_H
