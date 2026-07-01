#include "SearchDialog.h"

SearchDialog::SearchDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Find Text", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE) {

    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Search text input
    m_searchText = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    sizer->Add(m_searchText, 0, wxEXPAND | wxALL, 5);

    // Case sensitivity checkbox
    m_caseSensitive = new wxCheckBox(this, wxID_ANY, "Case Sensitive");
    sizer->Add(m_caseSensitive, 0, wxALIGN_LEFT | wxALL, 5);

    // Add standard buttons (OK and Cancel)
    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    sizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5);

    SetSizerAndFit(sizer);
}

SearchDialog::~SearchDialog() {
    // Cleanup
}
