#include "MainFrame.h"
#include "Database.h"
#include "Logger.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(10001, MainFrame::OnFetchData)  // Menüpunkt "Daten abrufen"
EVT_MENU(wxID_EXIT, MainFrame::OnExit)   // Menüpunkt "Beenden"
EVT_MENU(10002, MainFrame::OnEditUser)   // Menüpunkt "Benutzer bearbeiten"
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {

    // Menüleiste erstellen
    wxMenuBar* menuBar = new wxMenuBar;

    // "Datei"-Menü erstellen
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(10001, "Daten abrufen");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "Beenden");
    menuBar->Append(fileMenu, "Datei");

    // "Bearbeiten"-Menü erstellen (zweite Menüleiste)
    wxMenu* editMenu = new wxMenu;
    editMenu->Append(10002, "Benutzer bearbeiten");
    menuBar->Append(editMenu, "Bearbeiten");

    // Menüleiste dem Frame hinzufügen
    SetMenuBar(menuBar);

    // Statusleiste
    CreateStatusBar();
    SetStatusText("Willkommen im Hauptfenster!");
}

void MainFrame::OnFetchData(wxCommandEvent& event) {
    // Hier wird der Datenabruf durchgeführt
    wxString data;

    std::vector<std::pair<std::string, std::string>> users = Database::FetchAllUsers();  // Abruf aller Benutzer

    if (users.empty()) {
        data = "Keine Benutzerdaten gefunden.";
    }
    else {
        for (const auto& user : users) {
            data += "Benutzername: " + user.first + ", Passwort: " + user.second + "\n";
        }
    }

    // Zeige die Daten in einer MessageBox an
    wxMessageBox(data, "Benutzerdaten", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnEditUser(wxCommandEvent& event) {
    // Hier kannst du die Logik zum Bearbeiten von Benutzern hinzufügen
    wxMessageBox("Benutzer bearbeiten wurde ausgewählt.", "Bearbeiten", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);  // Anwendung beenden
}
