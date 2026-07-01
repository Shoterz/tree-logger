#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/frame.h>
#include <wx/stc/stc.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <map>
#include <wx/menu.h>
#include <wx/filedlg.h>


class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);

private:
    void OnButtonClicked(wxCommandEvent& evt);
    void OnTimeCaptureClicked(wxCommandEvent& evt);
    void OnClose(wxCloseEvent& evt);
    void OnNew(wxCommandEvent& evt);
    void OnOpen(wxCommandEvent& evt);
    void OnSave(wxCommandEvent& evt);
    void OnSaveAs(wxCommandEvent& evt);
    void OnMinimize(wxCommandEvent& evt);
    void OnMaximize(wxCommandEvent& evt);
    void OnCloseMenuItem(wxCommandEvent& evt);
    void CheckFilePath(wxCommandEvent& evt);
    void OnTextCtrlKeyDown(wxKeyEvent& evt);
    void OnTextCtrlKeyDown2(wxKeyEvent& evt);
    void FocusTextbox();
    void ExpandCheckbox();
    void OnChar(wxKeyEvent& evt);
    void Theme1(wxCommandEvent& evt);
    void Theme2(wxCommandEvent& evt);
    void Theme3(wxCommandEvent& evt);
    void Help(wxCommandEvent& evt);
    void Font1(wxCommandEvent& evt);
    void Font2(wxCommandEvent& evt);
    void Font3(wxCommandEvent& evt);

    wxString m_filePath;
    wxString filePath;
    wxStyledTextCtrl* m_list;
    wxTextCtrl* m_add;
    wxChoice* m_problemType;
    std::map<wxString, wxString> choiceMap;
    std::map<wxString, wxColour> colorMap;

    enum {
        ID_PROBLEM_TYPE = 1001,
        ID_SAVE = 1002,
        ID_NEW = 1003,
        ID_OPEN = 1004,
        ID_CLOSE = 1005,
        ID_MINIMIZE = 1006,
        ID_MAXIMIZE = 1007,
        ID_TIME_CAPTURE = 1008,
        ID_SAVE_RTF = 1009,
        ID_SAVEAS = 1010,
        ID_CHECK = 1011,
        ID_THEME1 = 1012,
        ID_THEME2 = 1013,
        ID_THEME3 = 1014,
        ID_HELP = 1015,
        ID_STYLE = 1016,
        ID_UPLOAD = 1017,
        ID_FONT1 = 1018,
        ID_FONT2 = 1019,
        ID_FONT3 = 1020
    };

    bool isExpanded = false;
};

#endif // MAINFRAME_H
