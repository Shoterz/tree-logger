#ifndef SEARCH_DIALOG_H
#define SEARCH_DIALOG_H

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/sizer.h>

class SearchDialog : public wxDialog {
public:
    SearchDialog(wxWindow* parent);
    ~SearchDialog();

    wxTextCtrl* m_searchText;
    wxCheckBox* m_caseSensitive;
};

#endif // SEARCH_DIALOG_H
