#ifndef LOGINFRAME_H
#define LOGINFRAME_H

#include <wx/wx.h>

class LoginFrame : public wxFrame {
public:
    LoginFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    wxTextCtrl* usernameCtrl;
    wxTextCtrl* passwordCtrl;

    void OnLogin(wxCommandEvent& event);  // Login-Button
    void OnRegister(wxCommandEvent& event);  // Registrierungs-Button
    void PerformRegister(const std::string& username, const std::string& password);  // Registrierung durchführen

    wxDECLARE_EVENT_TABLE();
};

#endif // LOGINFRAME_H
