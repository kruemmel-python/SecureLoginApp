#include "LoginFrame.h"
#include "MainFrame.h"
#include "Controller.h"
#include "Logger.h"
#include "Database.h"  
#include <thread>
#include <wx/msgdlg.h>

using namespace std;

wxBEGIN_EVENT_TABLE(LoginFrame, wxFrame)
EVT_BUTTON(10001, LoginFrame::OnLogin)
EVT_BUTTON(10002, LoginFrame::OnRegister)
wxEND_EVENT_TABLE()

LoginFrame::LoginFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    new wxStaticText(panel, wxID_ANY, "Username:", wxPoint(10, 10));
    usernameCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(150, 10), wxSize(200, 30));

    new wxStaticText(panel, wxID_ANY, "Password:", wxPoint(10, 60));
    passwordCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(150, 60), wxSize(200, 30), wxTE_PASSWORD);

    new wxButton(panel, 10001, "Login", wxPoint(150, 120), wxSize(100, 30));
    new wxButton(panel, 10002, "Register", wxPoint(260, 120), wxSize(100, 30));

    Logger::Log("GUI initialized.");

    // Hier wird die Datenbank initialisiert
    if (!Database::Initialize()) {
        wxMessageBox("Failed to initialize the database.", "Error", wxICON_ERROR);
    }
}

void LoginFrame::OnLogin(wxCommandEvent& event) {
    wxString username = usernameCtrl->GetValue();
    wxString password = passwordCtrl->GetValue();

    if (username.IsEmpty() || password.IsEmpty()) {
        wxMessageBox("Username and password cannot be empty!");
        return;
    }

    Logger::Log("Attempting login for user: " + username.ToStdString());

    bool loginSuccess = Database::Login(username.ToStdString(), password.ToStdString());

    if (loginSuccess) {
        Logger::Log("Login successful for user: " + username.ToStdString());
        wxMessageBox("Login successful!");

        // Neues Hauptfenster (MainFrame) öffnen und LoginFrame schließen
        MainFrame* mainFrame = new MainFrame("Hauptfenster", wxPoint(50, 50), wxSize(450, 340));
        mainFrame->Show(true);
        this->Close(true);  // LoginFrame schließen
    }
    else {
        Logger::Log("Login failed for user: " + username.ToStdString());
        wxMessageBox("Login failed. Please check your username and password.");
    }
}

void LoginFrame::OnRegister(wxCommandEvent& event) {
    wxString username = usernameCtrl->GetValue();
    wxString password = passwordCtrl->GetValue();

    if (username.IsEmpty() || password.IsEmpty()) {
        wxMessageBox("Username and password cannot be empty!");
        return;
    }

    if (password.Length() < 8 || password.find_first_of("!@#$%^&*") == wxString::npos) {
        wxMessageBox("Password must be at least 8 characters long and contain a special character.");
        return;
    }

    Logger::Log("Attempting registration for user: " + username.ToStdString());

    try {
        thread registerThread(&LoginFrame::PerformRegister, this, username.ToStdString(), password.ToStdString());
        registerThread.detach();
    }
    catch (const exception& e) {
        wxMessageBox("Error starting registration thread: " + wxString(e.what()));
        Logger::Log("Error starting registration thread: " + string(e.what()));
    }
}

void LoginFrame::PerformRegister(const std::string& username, const std::string& password) {
    try {
        bool result = Controller::Register(username, password);
        if (result) {
            Logger::Log("Registration successful for user: " + username);
            wxMessageBox("Registration successful!");
        }
        else {
            Logger::Log("Registration failed for user: " + username);
            wxMessageBox("Registration failed.");
        }
    }
    catch (const std::exception& e) {
        Logger::Log("Error during registration: " + std::string(e.what()));
        wxMessageBox("Registration error: " + wxString(e.what()));
    }
}
