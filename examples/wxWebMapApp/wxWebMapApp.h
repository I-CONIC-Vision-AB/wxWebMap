/////////////////////////////////////////////////////////////////////////////
// Name:        wxWebMapApp
// Purpose:     wxWebView with map sample
// Author:      Håkan Wiman
// Adapted from:      webview.cpp by Marianne Gagnon
// Copyright:   (c) 2022 I-CONIC Vision AB, Sweden
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include    <MapHtml.h>

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

#include "wx/artprov.h"
#include "wx/cmdline.h"
#include "wx/notifmsg.h"
#include "wx/settings.h"
//#include "wx/webview.h"
//#if wxUSE_WEBVIEW_IE
//#include "wx/msw/webview_ie.h"
//#endif
//#include "wx/webviewarchivehandler.h"
//#include "wx/webviewfshandler.h"
//#include "wx/numdlg.h"
#include "wx/infobar.h"
#include "wx/filesys.h"
#include "wx/fs_arc.h"
#include "wx/fs_mem.h"

//#ifndef wxHAS_IMAGES_IN_RESOURCES
//#include "../sample.xpm"
//#endif
//
//#if wxUSE_STC
//#include "wx/stc/stc.h"
//#else
//#include "wx/textctrl.h"
//#endif
//
//#if defined(__WXMSW__) || defined(__WXOSX__)
//#include "stop.xpm"
//#include "refresh.xpm"
//#endif
//
//#include "wxlogo.xpm"


namespace iconic {
class wxWebMapApp : public wxApp {
public:
    wxWebMapApp();

    virtual bool OnInit() wxOVERRIDE;

#if wxUSE_CMDLINE_PARSER
    virtual void OnInitCmdLine(wxCmdLineParser& parser) wxOVERRIDE;
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser) wxOVERRIDE;
#endif // wxUSE_CMDLINE_PARSER

private:
    wxString m_url;
    iconic::MapHtmlPtr cpMap;
};

}