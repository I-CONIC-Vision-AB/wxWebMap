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

/**
 * @brief Shows web page text source.
 *
 * Used in sample wxWebMapApp application only. In styled format if wxWidgets STC library has been compiled
*/
class SourceViewDialog : public wxDialog {
public:
    /**
     * @brief Constructor
     * @param parent Parent
     * @param source Source to show
    */
    SourceViewDialog(wxWindow* parent, wxString source);
};
