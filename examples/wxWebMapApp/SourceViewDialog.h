/////////////////////////////////////////////////////////////////////////////
// Name:        SourceViewDialog
// Purpose:     wxWebView with map sample
// Author:      Håkan Wiman
// Adapted from:      webview.cpp by Marianne Gagnon
// Copyright:   (c) 2022 I-CONIC Vision AB, Sweden
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include    <wx/dialog.h>

class SourceViewDialog : public wxDialog {
public:
    SourceViewDialog(wxWindow* parent, wxString source);
};
