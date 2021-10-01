#include <wx/wx.h>
#include <iostream>
#include "wxapp.h"
#include <future>
#include <thread>
#include <random>

// Generates a password. Using wxString to easily change UI elements without changing data type
wxString PasswordGenerator::generate(int length, bool caps, bool nums, bool specialChars)
{
    // Check what needs to be included in the password
    wxString charList = alphabet;
    if (caps)
        charList += capitals;

    if (nums)
        charList += numbers;

    if (specialChars)
        charList+=special;

    // Setup random number stuff
    // Seed with a real random value, if available
    std::random_device r;

    // Choose a random mean between 1 and charList length
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(1, charList.size());


    // Now generate the password with the chosen characters
    std::string password = "";

     for(int i=0; i<length; i++)
     {
         int randomIndex = uniform_dist(e1);
         password += charList[randomIndex];
         //std::cout << "RUN " << i << ": " << randomIndex << "(" << char(charList[randomIndex]) << ")" << std::endl;
     }
     std::cout << "'" << password << "'" << std::endl;
     std::cout << "PASSWORD LENGTH: " << password.size() << std::endl;
     return password;
}

wxIMPLEMENT_APP(myApp);

// IDs
enum
{
    ID_HELLO = 1,
    ID_NEWFILE = 2,
    ID_SAVE = 3,
    ID_SAVEAS = 4,
    ID_GENERATEBUTTON = 5
};

// Map events to their appropriate functions
wxBEGIN_EVENT_TABLE(myFrame, wxFrame)
    EVT_MENU(ID_HELLO, myFrame::OnHelp)
    EVT_MENU(wxID_ABOUT, myFrame::OnAbout)
    EVT_MENU(wxID_EXIT, myFrame::OnExit)
    EVT_BUTTON(ID_GENERATEBUTTON, myFrame::OnGenerate)
wxEND_EVENT_TABLE()


// This is the main function
bool myApp::OnInit()
{
    setlocale( LC_NUMERIC, "English" );
    myFrame *frame = new myFrame();
    frame->Show(true);
    return true;
}

// Now define functions for the events in wxFrame
void myFrame::OnHelp(wxCommandEvent &event)
{
    wxLogMessage("You pressed the hello button!");
}

void myFrame::OnAbout(wxCommandEvent &event)
{
    wxLogMessage("You pressed the about button!");
}

void myFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}

void myFrame::OnGenerate(wxCommandEvent &event)
{
    //passwordOutput->SetValue("TEST");
    wxString rawLength = lengthInput->GetValue();
    if (rawLength != "")
    {
        int length = std::stoi(rawLength.ToStdString());
        if (length < 0)
        {
            wxLogMessage("The password length must be greater than 0");
        }

        else if (length == 0)
        {
            wxLogMessage("The password length can't be zero");
        }

        else
        {
            // Now get the other true/false values
            bool usingCapitals = capitals->GetValue();
            bool usingNumbers = numbers->GetValue();
            bool usingSymbols = specialSymbols->GetValue();
            wxString password = (generator.generate(length, usingCapitals, usingNumbers, usingSymbols));
            passwordOutput->ChangeValue(password);

        }

    }

    else
    {
        // The length value is ""
        wxLogMessage("Enter a password length and trying to break my app");
    }

}



myFrame::myFrame() : wxFrame(NULL, wxID_ANY, "This is my frame thingy")
{
    // Colours for the menu bar and status bar
    wxColor barColours = wxColor(60,60,60);

    // Standard menubar and statusbar shit
    // Create a 'File' tab in the menu bar
    wxMenu *menuFile = new wxMenu;
    // Add items to the 'File' tab
    menuFile->Append(ID_NEWFILE, "New file", "Create a new file");
    menuFile->AppendSeparator();
    menuFile->Append(ID_SAVE, "Save", "Save the current file");
    menuFile->AppendSeparator();
    menuFile->Append(ID_SAVEAS, "Save as", "Save to an explicit location");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    // Create an 'About' tab
    wxMenu *menuAbout = new wxMenu;
    menuAbout->Append(wxID_ABOUT);
    menuAbout->Append(ID_HELLO, "Help", "Get help");

    // Create a menu to hold the above tabs
    wxMenuBar * menuBar = new wxMenuBar;
    // And add all of the tabs to the menu
    menuBar->Append(menuFile, "File");
    menuBar->Append(menuAbout, "About");
    menuBar->SetBackgroundColour(barColours);
    // Set the menu bar
    SetMenuBar(menuBar);

    // Now create a status bar at the bottom
    wxStatusBar *statusBar = new wxStatusBar(this, wxID_ANY);
    this->SetStatusBar(statusBar);
    SetStatusText("Hello world");
    statusBar->SetBackgroundColour(barColours);
    // Now the actual page content
    wxBoxSizer *mainArea = new wxBoxSizer(wxVERTICAL);
    this->SetBackgroundColour(wxColor(20,20,20));
    wxStaticText *titleText = new wxStaticText(this, wxID_ANY, "Very secure military grade password generator");
    // Change the font size
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize( 24 );
    titleText->SetFont( titleFont );
    // Add the title to the main sizer
    mainArea->Add(titleText, 0);
    mainArea->AddSpacer(10);

    // Create a window for the password output and its label (this lets me set a max size for the box)
    passwordContainer = new wxPanel(this, wxID_ANY);
    // Now set the max height (this is the main purpose of this wxPanel)
    passwordContainer->SetMaxSize(wxSize(-1, 100));
    // Text control to show the generated passwords
    wxBoxSizer *passwordSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText *passwordOutputLabel = new wxStaticText(passwordContainer, wxID_ANY, "Generated Password:");
    passwordOutput = new wxTextCtrl(passwordContainer, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_LEFT | wxTE_READONLY);
    // Change the font size
    wxFont passwordOutputLabelFont = passwordOutputLabel->GetFont();
    passwordOutputLabelFont.SetPointSize( 14 );
    passwordOutputLabel->SetFont( passwordOutputLabelFont );
    // Add these items to the sizer
    passwordSizer->Add(passwordOutputLabel, 0);
    passwordSizer->Add(passwordOutput, 1, wxLEFT | wxRIGHT | wxTOP | wxALIGN_CENTER | wxEXPAND, 15 | 15 | 20);
    passwordSizer->AddSpacer(25);
    // Add the passwordContainer window to the main sizer
    mainArea->Add(passwordContainer, 1, wxLEFT | wxRIGHT | wxEXPAND, 15 | 15 );
    passwordContainer->SetBackgroundColour(wxColor(50,50,50));
    passwordContainer->SetSizerAndFit(passwordSizer);

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    // Create another sizer to hold the label and password length textCtrl
    wxBoxSizer *optionsSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *lengthSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *lengthLabel = new wxStaticText(this, wxID_ANY, "Passsword Length:");
    lengthInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_LEFT, wxTextValidator(wxFILTER_NUMERIC));
    // Set this to 16 as default
    lengthInput->SetValue("16");

    lengthSizer->Add(lengthLabel);
    lengthSizer->Add(lengthInput);
    // Add the length option
    optionsSizer->Add(lengthSizer, 1);

    // Create a radio list of other options
    numbers = new wxCheckBox(this, wxID_ANY, "Use numbers", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    capitals = new wxCheckBox(this, wxID_ANY, "Use capital letters", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    specialSymbols = new wxCheckBox(this, wxID_ANY, "Use special symbols", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    // Check these by default
    numbers->SetValue(true);
    capitals->SetValue(true);
    specialSymbols->SetValue(true);

    optionsSizer->Add(capitals, 1);
    optionsSizer->Add(numbers, 1);
    optionsSizer->Add(specialSymbols, 1);

    wxButton *generateButton = new wxButton(this, ID_GENERATEBUTTON, "Generate Password");
    // Add the buttons to the button sizer
    buttonSizer->Add(optionsSizer, 1);
    buttonSizer->Add(generateButton, 1);

    // Now a sizer to contain the left/right panels
    wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
    // Add the panels to the sizer
    panelSizer->Add(mainArea, 2, wxEXPAND);
    panelSizer->Add(buttonSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 30);

    this->SetSizerAndFit(panelSizer);
}
