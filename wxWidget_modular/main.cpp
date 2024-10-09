#include <wx/wx.h>
#include "LoginFrame.h"

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    // Startet die GUI für das Login
    LoginFrame* loginFrame = new LoginFrame("Login", wxPoint(50, 50), wxSize(450, 340));
    loginFrame->Show(true);
    return true;
}
