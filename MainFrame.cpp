#include "MainFrame.h"
#include "MyClass.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <algorithm>
#include <vector>
#include <utility>
#include <wx/odcombo.h>
#include <wx/combobox.h>
#include <wx/stc/stc.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/msgdlg.h>  // Correct header for wxMessageDialog
#include <wx/textctrl.h>
#include <wx/colour.h>


// Custom style IDs
enum {
    STYLE_DEFAULT = 0,
    STYLE_DEBUG,
    STYLE_IMPLEMENTATION,
    STYLE_SOLUTION,
    STYLE_ERROR,
    STYLE_FATAL,
    STYLE_OBJECTIVE,
    STYLE_LESSON,
    STYLE_CODE,
    STYLE_TRACE
};
enum {
    STYLE_THEME,
};
MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, "Logger for Devs") {
        // MAIN ELEMENTS (PANELS, BUTTONS...)
        wxPanel* panel = new wxPanel(this, wxID_ANY);
        wxPanel* panelBottom = new wxPanel(this, wxID_ANY, wxPoint(0, 240), wxSize(720, 30));
        wxButton* timeCapture = new wxButton(panelBottom, wxID_ANY, "T", wxPoint(0, 0), wxSize(30, 30)); // used for now to send text to panel top
        wxButton* timeEdit = new wxButton(panelBottom, wxID_ANY, "S", wxPoint(31, 0), wxSize(30, 30));
        m_list = new wxStyledTextCtrl(panel, wxID_ANY, wxDefaultPosition, wxSize(1000, 500), wxBORDER_SUNKEN);
        m_add = new wxTextCtrl(panelBottom, wxID_ANY, "", wxPoint(91, 0), wxSize(490, 30), wxTE_PROCESS_ENTER);

        wxFileName filePath;
        wxString path = filePath.GetPath();

        // CHOICES ARRAY
        wxArrayString choices;
        choices.Add("DEBUG");
        choices.Add("IMPLEMENTATION");
        choices.Add("SOLUTION");
        choices.Add("ERROR");
        choices.Add("FATAL");
        choices.Add("OBJECTIVE");
        choices.Add("LESSON");
        choices.Add("CODE");
        choices.Add("TRACE");

        // SHORTENED ARRAY
        choiceMap["DEBUG"] = "DBUG";
        choiceMap["IMPLEMENTATION"] = "IMPT";
        choiceMap["SOLUTION"] = "SLTN";
        choiceMap["OBJECTIVE"] = "OBJV";
        choiceMap["ERROR"] = "EROR";
        choiceMap["FATAL"] = "FTAL";
        choiceMap["LESSON"] = "LSSN";
        choiceMap["CODE"] = "CODE";
        choiceMap["TRACE"] = "TRCE";

        // COLOR MAPPING
        colorMap["DEBUG"] = wxColour(23, 32, 42); // Green
        colorMap["IMPLEMENTATION"] = wxColour(0, 89, 179); // Red
        colorMap["SOLUTION"] = wxColour(0, 51, 102); // Blue
        colorMap["ERROR"] = wxColour(255, 15, 15); // Red Warning
        colorMap["FATAL"] = wxColour(153, 0, 0); // Crimson Red
        colorMap["OBJECTIVE"] = wxColour(39, 174, 96); // Green
        colorMap["LESSON"] = wxColour(25, 111, 61); // Dark Green
        colorMap["CODE"] = wxColour(211, 84, 0); // Gray
        colorMap["TRACE"] = wxColour(185, 119, 14); // Gray

        m_problemType = new wxChoice(panelBottom, ID_PROBLEM_TYPE, wxPoint(61, 0), wxSize(150, 30), choices);
        m_problemType->Select(0);

        // BINDERS FOR BUTTONS
        timeEdit->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
        timeEdit->SetToolTip("Shift+Enter to upload");
        timeCapture->Bind(wxEVT_BUTTON, &MainFrame::OnTimeCaptureClicked, this);
        timeCapture->SetToolTip("Ctrl+T to capture current timestamp");

        // SIZERS
        //panel->SetBackgroundColour(wxColour(173, 216, 230));
        panel->SetBackgroundColour(wxColour(0, 0, 0));  // Light Blue
        wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);
        panelSizer->Add(m_list, 1, wxEXPAND | wxALL, 1);
        panel->SetSizer(panelSizer);  // 0 means fixed size

        wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
        bottomSizer->Add(timeCapture, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
        bottomSizer->Add(timeEdit, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
        bottomSizer->Add(m_problemType, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
        bottomSizer->Add(m_add, 1, wxEXPAND | wxALL, 1);

        panelBottom->SetSizer(bottomSizer);
        wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
        frameSizer->Add(panel, 1, wxEXPAND | wxALL, 1);
        frameSizer->Add(panelBottom, 0, wxEXPAND | wxALL, 0);

        this->SetSizerAndFit(frameSizer);

        this->SetMinSize(wxSize(720, 300));

        // Initialize wxStyledTextCtrl
        m_list->SetLexer(wxSTC_LEX_NULL); // No lexer for plain text
        m_list->SetKeyWords(0, ""); // No keywords for plain text

        // Set THEME COLOUR
        //m_list->StyleSetForeground(STYLE_DEFAULT, wxColour(0, 0, 0)); // Black
        ////m_list->StyleSetBackground(STYLE_DEFAULT, wxColour(46, 52, 54)); // Black
        ////m_list->StyleSetBackground(wxSTC_STYLE_DEFAULT, bgColor);


        // Set custom styles
        m_list->StyleSetForeground(STYLE_DEBUG, colorMap["DEBUG"]);
        m_list->StyleSetForeground(STYLE_IMPLEMENTATION, colorMap["IMPLEMENTATION"]);
        m_list->StyleSetForeground(STYLE_SOLUTION, colorMap["SOLUTION"]);
        m_list->StyleSetForeground(STYLE_ERROR, colorMap["ERROR"]);
        m_list->StyleSetForeground(STYLE_FATAL, colorMap["FATAL"]);
        m_list->StyleSetForeground(STYLE_OBJECTIVE, colorMap["OBJECTIVE"]);
        m_list->StyleSetForeground(STYLE_LESSON, colorMap["LESSON"]);
        m_list->StyleSetForeground(STYLE_CODE, colorMap["CODE"]);
        m_list->StyleSetForeground(STYLE_TRACE, colorMap["TRACE"]);

        // Menu Bar
        wxMenu* fileMenu = new wxMenu;
        fileMenu->Append(ID_NEW, "&New\tCtrl+N", "Create a new log file");
        fileMenu->Append(ID_OPEN, "&Open\tCtrl+O", "Open an existing log file");
        fileMenu->AppendSeparator();
        fileMenu->Append(ID_SAVE, "&Save\tCtrl+S", "Save the log to a text file");
        fileMenu->Append(ID_SAVEAS, "&Save As\tCtrl+Shift+S", "Save the log to a new text file");
        fileMenu->AppendSeparator();
        fileMenu->Append(ID_CLOSE, "&Close\tCtrl+W", "Close the current log file");

        wxMenu* windowMenu = new wxMenu;
        windowMenu->Append(ID_MINIMIZE, "&Minimize\tCtrl+M", "Minimize the window");
        windowMenu->Append(ID_MAXIMIZE, "&Maximize\tCtrl+L", "Maximize the window");

        wxMenu* themeMenu = new wxMenu;
        themeMenu->Append(ID_THEME1, "&Default White\tCtrl+1", "Default White");
        themeMenu->Append(ID_THEME2, "&Charade Nova\tCtrl+2", "Charade Nova");
        themeMenu->Append(ID_THEME3, "&Outer Space\tCtrl+3", "Outer Space");

        wxMenu* fontMenu = new wxMenu;
        fontMenu->Append(ID_FONT1, "&Small Font\tCtrl+4", "Default Font");
        fontMenu->Append(ID_FONT2, "&Medium Font\tCtrl+5", "Medium Font");
        fontMenu->Append(ID_FONT3, "&Large Font\tCtrl+6", "Large Font");


        wxMenu* filepathMenu = new wxMenu;
        filepathMenu->Append(ID_CHECK, "&Check\tCtrl+Shift+C", "open file path address");

        wxMenu* helpMenu = new wxMenu;
        helpMenu->Append(ID_HELP, "&How To Use\tCtrl+H", "open help dialog box");

        wxMenuBar* menuBar = new wxMenuBar;
        menuBar->Append(fileMenu, "&File");
        menuBar->Append(windowMenu, "&Window");
        menuBar->Append(themeMenu, "&Theme");
        menuBar->Append(fontMenu, "&Font");
        menuBar->Append(filepathMenu, "&Check File Path");
        menuBar->Append(helpMenu, "&Help");
        SetMenuBar(menuBar);

        // Bind the events
        Bind(wxEVT_MENU, &MainFrame::OnNew, this, ID_NEW);
        Bind(wxEVT_MENU, &MainFrame::OnOpen, this, ID_OPEN);
        Bind(wxEVT_MENU, &MainFrame::OnSave, this, ID_SAVE);
        Bind(wxEVT_MENU, &MainFrame::OnSaveAs, this, ID_SAVEAS);
        Bind(wxEVT_MENU, &MainFrame::OnCloseMenuItem, this, ID_CLOSE);
        Bind(wxEVT_MENU, &MainFrame::OnMinimize, this, ID_MINIMIZE);
        Bind(wxEVT_MENU, &MainFrame::OnMaximize, this, ID_MAXIMIZE);
        Bind(wxEVT_MENU, &MainFrame::CheckFilePath, this, ID_CHECK);
        Bind(wxEVT_MENU, &MainFrame::Theme1, this, ID_THEME1);
        Bind(wxEVT_MENU, &MainFrame::Theme2, this, ID_THEME2);
        Bind(wxEVT_MENU, &MainFrame::Theme3, this, ID_THEME3);
        Bind(wxEVT_MENU, &MainFrame::Font1, this, ID_FONT1);
        Bind(wxEVT_MENU, &MainFrame::Font2, this, ID_FONT2);
        Bind(wxEVT_MENU, &MainFrame::Font3, this, ID_FONT3);
        Bind(wxEVT_MENU, &MainFrame::Help, this, ID_HELP);
        Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);

        //EASE OF USE BINDINGS
        m_list->Bind(wxEVT_CHAR_HOOK, &MainFrame::OnTextCtrlKeyDown, this);
        timeEdit->Bind(wxEVT_CHAR_HOOK, &MainFrame::OnTextCtrlKeyDown, this);
        timeCapture->Bind(wxEVT_CHAR_HOOK, &MainFrame::OnTextCtrlKeyDown, this);
        menuBar->Bind(wxEVT_CHAR_HOOK, &MainFrame::OnTextCtrlKeyDown, this);
        m_problemType->Bind(wxEVT_CHAR_HOOK, &MainFrame::OnTextCtrlKeyDown, this);
        m_list->Bind(wxEVT_CHAR_HOOK, &MainFrame::OnTextCtrlKeyDown2, this);
        m_add->Bind(wxEVT_CHAR_HOOK, &MainFrame::OnTextCtrlKeyDown2, this);
        timeEdit->Bind(wxEVT_CHAR_HOOK, &MainFrame::OnTextCtrlKeyDown2, this);
        timeCapture->Bind(wxEVT_CHAR_HOOK, &MainFrame::OnTextCtrlKeyDown2, this);
        menuBar->Bind(wxEVT_CHAR_HOOK, &MainFrame::OnTextCtrlKeyDown2, this);
        m_problemType->Bind(wxEVT_CHAR_HOOK, &MainFrame::OnTextCtrlKeyDown2, this);

        // Define the accelerator table for Ctrl + T
        wxAcceleratorEntry entries[2];
        entries[0].Set(wxACCEL_CTRL, 'T', ID_TIME_CAPTURE); // ID_TIME_CAPTURE is a new ID for the time capture event
        entries[1].Set(wxACCEL_SHIFT, WXK_RETURN, ID_UPLOAD);

        wxAcceleratorTable accel(2, entries);
        SetAcceleratorTable(accel);

        // Bind the time capture event to the OnTimeCaptureClicked method
        Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnTimeCaptureClicked, this, ID_TIME_CAPTURE);
        Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnButtonClicked, this, ID_UPLOAD);
        m_problemType->Bind(wxEVT_CHAR, &MainFrame::OnChar, this);

        //wxString* m_filePath = new wxString;
        filePath = "";

        // Show the frame
        this->Show();

}

void MainFrame::OnButtonClicked(wxCommandEvent& evt) {
    // Get the text from m_add
    wxString text = m_add->GetValue();

    // Get selected choice text
    wxString selectedChoice = m_problemType->GetStringSelection();

    wxString shortenedChoice = choiceMap[selectedChoice];

    wxString fullText = shortenedChoice + " " + text;

    // Append the text to m_list with color
    if (!text.IsEmpty()) {
        wxString currentContent = m_list->GetText();
        if (!currentContent.IsEmpty() && currentContent.Last() != '\n') {
            m_list->AppendText("\n");
        }

        int start = m_list->GetLength();
        m_list->AppendText(shortenedChoice + " "); // Append the problem type
        int end = m_list->GetLength();

        // Set color for the problem type
        int styleId = STYLE_DEFAULT;
        if (selectedChoice == "DEBUG") styleId = STYLE_DEBUG;
        else if (selectedChoice == "IMPLEMENTATION") styleId = STYLE_IMPLEMENTATION;
        else if (selectedChoice == "SOLUTION") styleId = STYLE_SOLUTION;
        else if (selectedChoice == "ERROR") styleId = STYLE_ERROR;
        else if (selectedChoice == "FATAL") styleId = STYLE_FATAL;
        else if (selectedChoice == "OBJECTIVE") styleId = STYLE_OBJECTIVE;
        else if (selectedChoice == "LESSON") styleId = STYLE_LESSON;
        else if (selectedChoice == "CODE") styleId = STYLE_CODE;
        else if (selectedChoice == "TRACE") styleId = STYLE_TRACE;

        m_list->StartStyling(start, 0x1f);
        m_list->SetStyling(end - start, styleId);

        // Append the rest of the text in default color
        m_list->StartStyling(end, 0x1f);
        m_list->SetStyling(text.Length() + 1, STYLE_DEFAULT); // +1 for the space
        m_list->AppendText(text + "\n");
    }

    // Clear the input in m_add
    m_add->Clear();
}

void MainFrame::OnTimeCaptureClicked(wxCommandEvent& evt) {
    wxDateTime now = wxDateTime::Now();

    wxString timestamp = now.Format("[%Y-%m-%d %H:%M:%S] ");

    m_add->WriteText(timestamp);
}
void MainFrame::OnNew(wxCommandEvent& evt) {
    // Check if there is any unsaved content
    if (!m_list->GetText().IsEmpty()) {
        // Show a dialog box asking if the user wants to save the current project
        wxMessageDialog* msgDlg = new wxMessageDialog(this, "You really wanna quit without saving huh? (CTRL+Z if you need it) \n If you already hit Ctrl+S you may click Yes", "Unsaved Changes", wxYES_NO | wxCANCEL | wxICON_QUESTION);
        int result = msgDlg->ShowModal();
        msgDlg->Destroy();

        if (result == wxID_NO) {
            // User wants to save the current log
            OnSave(evt);
        } else if (result == wxID_CANCEL) {
            // User canceled the operation
            return;
        }
    }

    // Clear the content of the text control
    m_list->ClearAll();
    m_add->Clear();
}

void MainFrame::OnOpen(wxCommandEvent& evt) {
    // Open a file dialog to choose the file to open
    if (filePath.IsEmpty() && m_list->GetText().IsEmpty()) {
        wxFileDialog openDialog(this, "Open Log File", "", "", "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

        if (openDialog.ShowModal() == wxID_OK) {
            filePath = openDialog.GetPath();

            // Open the file for reading
            wxTextFile file(filePath);
            if (file.Open()) {
                // Clear the current content
                m_list->ClearAll();

                // Read and append each line to the text control
                int lineCount = file.GetLineCount();
                for (int i = 0; i < lineCount; ++i) {
                    wxString line = file.GetLine(i);
                    m_list->AppendText(line + "\n");
                }

                file.Close();
                wxLogMessage("Log opened from: " + filePath);
            } else {
                wxLogError("Failed to open file for reading.");
            }
        }
    } else {
        wxMessageDialog* msgDlg = new wxMessageDialog(this, "You really wanna quit without saving huh? (CTRL+Z if you need it) \n If you already hit Ctrl+S you may click Yes", "Unsaved Changes", wxYES_NO | wxCANCEL | wxICON_QUESTION);
        int result = msgDlg->ShowModal();
        msgDlg->Destroy();

        if (result == wxID_NO) {
            // User wants to save the current log
            OnSave(evt);
        } else if (result == wxID_CANCEL) {
            // User canceled the operation
            return;
        }
        m_list->ClearAll();
        m_add->Clear();
    }
}

void MainFrame::OnSaveAs(wxCommandEvent& evt) {
    wxFileDialog saveDialog(this, "Save Text File", "", "",
        "Text files (*.txt)|*.txt",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveDialog.ShowModal() == wxID_OK) {
        wxString filePath = saveDialog.GetPath();
        //wxLogMessage("Attempting to save file at: " + filePath);

        // Ensure the file has the .txt extension
        wxFileName fn(filePath);
        if (!fn.HasExt())
            fn.SetExt("txt");

        // Update the filePath with the new path (including the .txt extension if added)
        filePath = fn.GetFullPath();

        // Update the member variable filePath in MainFrame
        this->filePath = filePath;

        // Open the file for writing
        wxFile file(filePath, wxFile::write);
        if (file.IsOpened()) {
            //wxLogMessage("File opened successfully.");

            // Get the text from the wxTextCtrl
            wxString text = m_list->GetValue();

            // Write the text to the file
            if (file.Write(text.ToUTF8())) {
                //wxLogMessage("File written successfully.");
                file.Close();
                //wxLogMessage("File closed successfully.");
                //wxLogMessage("Text saved to: " + fn.GetFullPath());
            } else {
                wxLogError("Failed to write to file.");
                file.Close();
                wxLogMessage("File closed due to write failure.");
            }
        } else {
            wxLogError("Failed to open file for writing.");
        }
    }
}
void MainFrame::OnSave(wxCommandEvent& evt) {
    if (!filePath.IsEmpty()) {
        // Open the file for writing
        wxTextFile file(filePath);

        // Check if the file exists, if not create it
        if (!file.Exists()) {
            file.Create();
        }

        // Open the file
        if (file.Open()) {
            // Clear the file content (optional, depending on your use case)
            file.Clear();

            // Get the text from the wxStyledTextCtrl (m_list)
            wxString text = m_list->GetValue();

            // Split the text into lines and add each line to the file
            wxArrayString lines = wxSplit(text, '\n');
            for (const wxString& line : lines) {
                file.AddLine(line);
            }

            // Write changes to the file
            if (file.Write()) {
                //wxLogMessage("File saved successfully: " + filePath);
            } else {
                wxLogError("Failed to write to file: " + filePath);
            }

            // Close the file
            file.Close();
        } else {
            wxLogError("Failed to open file for writing: " + filePath);
        }
    } else {
        // If filePath is empty, fall back to Save As
        OnSaveAs(evt);

    }
}
void MainFrame::OnClose(wxCloseEvent& evt) {
    // Check if there is any unsaved content
    if (!m_list->GetText().IsEmpty()) {
        // Show a dialog box asking if the user wants to save the current project
        wxMessageDialog* msgDlg = new wxMessageDialog(this, "You really wanna quit without saving huh? \n If you already hit Ctrl+S you may click Yes", "Unsaved Changes", wxYES_NO | wxCANCEL | wxICON_QUESTION);
        int result = msgDlg->ShowModal();
        msgDlg->Destroy();

        if (result == wxID_NO) {
            // User wants to save the current log
            wxCommandEvent saveEvent;
            OnSave(saveEvent);
        } else if (result == wxID_CANCEL) {
            // User canceled the operation
            evt.Veto(); // Prevent the window from closing
            return;
        }
    }

    // Allow the window to close
    evt.Skip();
}

void MainFrame::OnCloseMenuItem(wxCommandEvent& evt) {

    // Allow the window to close
    this->Close(true);

}

void MainFrame::OnMinimize(wxCommandEvent& evt) {
    this->Iconize(true);
}

void MainFrame::OnMaximize(wxCommandEvent& evt) {
    this->Maximize(true);
}

void MainFrame::OnTextCtrlKeyDown(wxKeyEvent& event) {
    if (event.GetKeyCode() == '/') { // Check for backtick key
        //wxLogMessage("Backtick key pressed!");
        //wxLogMessage(filePath); (used for debugging)
        FocusTextbox(); // Call your method to expand the checkbox
    } else {
        event.Skip(); // Allow default key handling
    }
}
void MainFrame::OnTextCtrlKeyDown2(wxKeyEvent& event) {
    if (event.GetKeyCode() == '`') {
        ExpandCheckbox(); // Call your method to expand the checkbox
    } else {
        event.Skip(); // Allow default key handling
    }
}

void MainFrame::FocusTextbox() {
    m_add->SetFocus();
    return; // Prevent further processing
    // Add your logic here to expand the checkbox or perform other actions
}
void MainFrame::ExpandCheckbox() {
    m_problemType->SetFocus();
    return; // Prevent further processing
    // Add your logic here to expand the checkbox or perform other actions
}
void MainFrame::OnChar(wxKeyEvent& event) {
    if (wxWindow::FindFocus() == m_problemType) {
        // Get the key code
        int keyCode = event.GetKeyCode();

        // Convert the key code to a character
        char keyChar = static_cast<char>(keyCode);

        // Convert to uppercase to handle case-insensitive matching
        keyChar = toupper(keyChar);
        if (event.GetKeyCode() == ' ') {
        wxLogMessage("use following hot letters to select type: \n (D)EBUG \n (I)MPLEMENTATION \n (S)OLUTION \n (E)RROR \n (F)ATAL \n (O)JECTIVE \n (L)ESSON \n (C)ODE \n (T)RACE");
        }   else {
        for (size_t i = 0; i < m_problemType->GetCount(); ++i) {
            wxString item = m_problemType->GetString(i);
            if (!item.IsEmpty() && toupper(item[0]) == keyChar) {
                // Select the item
                m_problemType->SetSelection(i);
                break;
            } // Allow default key handling
        }
        // Find the item that starts with the pressed key

        }
    } else {
        event.Skip(); // Allow default key handling
    }
}
void MainFrame::CheckFilePath(wxCommandEvent& evt) {
    wxLogMessage(filePath);
}
void MainFrame::Theme1(wxCommandEvent& evt) {
    wxColour bgColor(255, 255, 255);
    wxColour fgColor(0, 0, 0);
    m_list->StyleSetForeground(wxSTC_STYLE_DEFAULT, fgColor);
    m_list->SetForegroundColour(fgColor);
    m_list->SetBackgroundColour(bgColor);
    m_list->StyleSetBackground(STYLE_DEFAULT, wxColour(255, 255, 255)); // Black
    m_list->StyleSetBackground(wxSTC_STYLE_DEFAULT, bgColor);
    m_list->Refresh();

    /*m_add->SetForegroundColour(wxColour(0, 0, 0)); // black text
    m_add->SetBackgroundColour(wxColour(255, 255, 255));  // white background

    m_add->Refresh(); */

    m_list->StyleSetForeground(STYLE_THEME, fgColor);
    m_list->StyleSetBackground(STYLE_THEME, bgColor);

    // Select the range of text to apply the style
    int styleTheme = STYLE_THEME;
    int start = 0;
    int end = m_list->GetLength();
    m_list->StartStyling(start, styleTheme);
    m_list->SetStyling(end - start, styleTheme);

}
void MainFrame::Theme2(wxCommandEvent& evt) {
    //wxLogMessage("Outer Space");
    wxColour bgColor(46, 52, 54);
    wxColour fgColor(217, 215, 219);
    m_list->StyleSetForeground(wxSTC_STYLE_DEFAULT, fgColor);
    m_list->SetForegroundColour(fgColor);
    m_list->StyleSetBackground(STYLE_DEFAULT, wxColour(255, 255, 255)); // Black
    m_list->StyleSetBackground(wxSTC_STYLE_DEFAULT, bgColor);

    m_list->StyleSetForeground(STYLE_THEME, fgColor);
    m_list->StyleSetBackground(STYLE_THEME, bgColor);

    // Select the range of text to apply the style
    int styleTheme = STYLE_THEME;
    int start = 0;
    int end = m_list->GetLength();
    m_list->StartStyling(start, styleTheme);
    m_list->SetStyling(end - start, styleTheme);
    m_list->Refresh();

}
void MainFrame::Theme3(wxCommandEvent& evt) {
    //wxLogMessage("Outer Space");
    wxColour bgColor(30, 34, 44);
    wxColour fgColor(94, 190, 196);
    m_list->StyleSetForeground(wxSTC_STYLE_DEFAULT, fgColor);
    m_list->SetForegroundColour(fgColor);
    m_list->SetBackgroundColour(bgColor);
    m_list->StyleSetBackground(STYLE_DEFAULT, wxColour(255, 255, 255)); // Black
    m_list->StyleSetBackground(wxSTC_STYLE_DEFAULT, bgColor);
    //m_add->SetForegroundColour(wxColour(255, 255, 255)); // White text
    //m_add->SetBackgroundColour(wxColour(30,34,44));  // Teal background

    //m_add->Refresh();
    m_list->Refresh();

    m_list->StyleSetForeground(STYLE_THEME, fgColor);
    m_list->StyleSetBackground(STYLE_THEME, bgColor);

    // Select the range of text to apply the style
    int styleId = STYLE_THEME;
    int start = 0;
    int end = m_list->GetLength();
    m_list->StartStyling(start, styleId);
    m_list->SetStyling(end - start, styleId);

}
void MainFrame::Help(wxCommandEvent& evt) {
    wxLogMessage("(`) shortcut focuses the problem type option box \n (/) focuses the add text field \n Ctrl+T captures current timestamp \n Shift+Enter to upload log to main space");
}
void MainFrame::Font1(wxCommandEvent& evt) {
    wxFont font(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_list->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
}
void MainFrame::Font2(wxCommandEvent& evt) {
    wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_list->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
}
void MainFrame::Font3(wxCommandEvent& evt) {
    wxFont font(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_list->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
}
