#ifndef KUNDE_H
#define KUNDE_H

#include <string>

class Kunde {
private:
    int kundenID;
    std::string name;
    std::string adresse;

public:
    Kunde(int id, const std::string& name, const std::string& adresse);

    // Methoden für Kunden
    bool KundeAnlegen();
    bool KundeBearbeiten();
    bool KundeLöschen();
};

#endif // KUNDE_H
