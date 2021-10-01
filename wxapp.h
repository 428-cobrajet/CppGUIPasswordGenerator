class PasswordGenerator
{
public:
    wxString generate(int length, bool specialChars, bool numbers, bool capitals);

private:
    //Strings whose characters will be used to build password
    wxString alphabet = wxT("abcdefghijklmnopqrstuvwxyz");
    wxString capitals = wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    wxString numbers = wxT("0123456789");
    wxString special = wxT("!@#$%&{}[]():;^,.=+-~*");
    // Total adds to 56
};

class myApp : public wxApp
{
public:
    bool OnInit();
};

class GeneratorThread : public wxThread {
	virtual ExitCode Entry();
};



class myFrame : public wxFrame
{
public:
    myFrame();
    wxTextCtrl *passwordOutput;
    wxPanel *passwordContainer;
    wxTextCtrl *lengthInput;
    wxCheckBox *capitals;
    wxCheckBox *numbers;
    wxCheckBox *specialSymbols;
    PasswordGenerator generator;

private:
    // Declare an event table to map events to functions
    wxDECLARE_EVENT_TABLE();
    // And event handler functions
    void OnHelp(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event);
    void OnGenerate(wxCommandEvent &event);
};
