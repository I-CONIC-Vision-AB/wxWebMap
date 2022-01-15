/////////////////////////////////////////////////////////////////////////////
// Name:        wxWebMapApp
// Purpose:     wxWebView with map sample
// Author:      Håkan Wiman
// Adapted from:      webview.cpp by Marianne Gagnon
// Copyright:   (c) 2022 I-CONIC Vision AB, Sweden
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#pragma once

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_WEBVIEW_WEBKIT && !wxUSE_WEBVIEW_WEBKIT2 && !wxUSE_WEBVIEW_IE && !wxUSE_WEBVIEW_EDGE
#error "A wxWebView backend is required by this sample"
#endif

#include "wx/app.h"

#include "wx/cmdline.h"

/**
 * @brief A web map view application
*/
class wxWebMapApp : public wxApp {
public:
    /**
     * @brief Constructor
    */
    wxWebMapApp();

    /**
     * @brief Init application
    */
    virtual bool OnInit() wxOVERRIDE;

#if wxUSE_CMDLINE_PARSER
    /**
     * @brief Set up command line parser
    */
    virtual void OnInitCmdLine(wxCmdLineParser& parser) wxOVERRIDE;

    /**
     * @brief Command line handler
    */
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser) wxOVERRIDE;
#endif // wxUSE_CMDLINE_PARSER

private:
    wxString m_url;
};
