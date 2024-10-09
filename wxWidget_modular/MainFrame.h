#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    void OnFetchData(wxCommandEvent& event);  // Methode für "Daten abrufen"
    void OnEditUser(wxCommandEvent& event);   // Methode für "Benutzer bearbeiten"
    void OnExit(wxCommandEvent& event);       // Methode für "Beenden"

    wxDECLARE_EVENT_TABLE();
};

#endif // MAINFRAME_H
