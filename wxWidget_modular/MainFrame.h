#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    void OnFetchData(wxCommandEvent& event);  // Methode zum Abrufen der Daten
    void OnExit(wxCommandEvent& event);  // Methode zum Beenden der Anwendung

    wxDECLARE_EVENT_TABLE();
};

#endif // MAINFRAME_H
