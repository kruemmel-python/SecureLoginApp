#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <vector>
#include <string>

class MainFrame : public wxFrame {
public:
    // Konstruktor für das Hauptfenster
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    // Event-Handler für Menüaktionen
    void OnFetchData(wxCommandEvent& event);     // Methode für "Daten abrufen"
    void OnEditUser(wxCommandEvent& event);      // Methode für "Benutzer bearbeiten"
    void OnEditCustomer(wxCommandEvent& event);  // Methode für "Kunde bearbeiten"
    void OnExit(wxCommandEvent& event);          // Methode für "Beenden"

    wxDECLARE_EVENT_TABLE();  // Deklaration der Ereignistabelle
};

#endif // MAINFRAME_H
