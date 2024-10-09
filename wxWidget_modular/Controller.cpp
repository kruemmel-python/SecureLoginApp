#include "Controller.h"
#include "Database.h"
#include "Logger.h"
#include <wx/wx.h>

bool Controller::Login(const std::string& username, const std::string& password) {
    // Der Login-Code ist noch nicht implementiert
    return false;
}

bool Controller::Register(const std::string& username, const std::string& password) {
    try {
        if (Database::Register(username, password)) {
            Logger::Log("Registration successful for user: " + username);
            return true;
        }
        else {
            Logger::Log("Registration failed for user: " + username);
            return false;
        }
    }
    catch (const std::exception& e) {
        Logger::Log("Error during registration: " + std::string(e.what()));
        wxMessageBox("Registration error: " + wxString(e.what()));
        return false;
    }
}
