#pragma once

#include	<wx/webview.h>
#include    <exports.h>

namespace iconic {
class ICONIC_WEBMAP_EXPORT wxWebMap : public wxWindow {
public:
    wxWebMap();

    static wxWebMap* Create(wxWindow* parent,
                            wxWindowID id,
                            wxString const& basemapHtmlFileName = wxEmptyString,
                            wxString const& dataDirectory = wxEmptyString,
                            const wxString& url = wxASCII_STR(wxWebViewDefaultURLStr),
                            const wxPoint& pos = wxDefaultPosition,
                            const wxSize& size = wxDefaultSize,
                            const wxString& backend = wxASCII_STR(wxWebViewBackendDefault),
                            long style = 0,
                            const wxString& name = wxASCII_STR(wxWebViewNameStr));

};
}