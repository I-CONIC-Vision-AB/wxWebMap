#include	<SourceViewDialog.h>
#include    <wx/textctrl.h>
#include    <wx/sizer.h>

SourceViewDialog::SourceViewDialog(wxWindow* parent, wxString source) :
    wxDialog(parent, wxID_ANY, "Source Code",
             wxDefaultPosition, wxSize(700, 500),
             wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
#if 0 //wxUSE_STC
    wxStyledTextCtrl* text = new wxStyledTextCtrl(this, wxID_ANY);
    text->SetMarginWidth(1, 30);
    text->SetMarginType(1, wxSTC_MARGIN_NUMBER);
    text->SetText(source);

    text->StyleClearAll();
    text->SetLexer(wxSTC_LEX_HTML);
    text->StyleSetForeground(wxSTC_H_DOUBLESTRING, wxColour(255, 0, 0));
    text->StyleSetForeground(wxSTC_H_SINGLESTRING, wxColour(255, 0, 0));
    text->StyleSetForeground(wxSTC_H_ENTITY, wxColour(255, 0, 0));
    text->StyleSetForeground(wxSTC_H_TAG, wxColour(0, 150, 0));
    text->StyleSetForeground(wxSTC_H_TAGUNKNOWN, wxColour(0, 150, 0));
    text->StyleSetForeground(wxSTC_H_ATTRIBUTE, wxColour(0, 0, 150));
    text->StyleSetForeground(wxSTC_H_ATTRIBUTEUNKNOWN, wxColour(0, 0, 150));
    text->StyleSetForeground(wxSTC_H_COMMENT, wxColour(150, 150, 150));
#else // !wxUSE_STC
    wxTextCtrl* text = new wxTextCtrl(this, wxID_ANY, source,
                                      wxDefaultPosition, wxDefaultSize,
                                      wxTE_MULTILINE |
                                      wxTE_RICH |
                                      wxTE_READONLY);
#endif // wxUSE_STC/!wxUSE_STC

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text, 1, wxEXPAND);
    SetSizer(sizer);
}