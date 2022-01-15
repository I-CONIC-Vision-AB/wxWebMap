#include	<detail/wxWebMapImpl.h>
#include    <wx/log.h>
#include    <wx/intl.h>
#include    <wx/sizer.h>
#include    <wx/filename.h>

wxWebMap::wxWebMap() :
    wxWindow()
{

}

wxWebMap* wxWebMap::Create(wxWindow* parent, wxWindowID id, wxString const& basemapHtmlFileName, /*wxString const& dataDirectory, const wxString& url, */const wxPoint& pos, const wxSize& size, const wxString& backend, long style, const wxString& name)
{
    return wxWebMapImpl::Create(parent, id, basemapHtmlFileName, /*dataDirectory, url, */pos, size, backend, style, name);
}

wxWebMapImpl::wxWebMapImpl(wxString const& basemapHtmlFileName, wxString const& dataDirectory) :
    wxWebMap(),
    cMapName("map")
{
}

wxWebMap* wxWebMapImpl::Create(wxWindow* parent, wxWindowID id, wxString const& basemapHtmlFileName, /*wxString const& dataDirectory, const wxString& url, */const wxPoint& pos, const wxSize& size, const wxString& backend, long style, const wxString& name)
{
    wxFileName fn(basemapHtmlFileName);
    wxWebMapImpl* p = new wxWebMapImpl(fn.GetFullName(), fn.GetPath());
    p->wxWindow::Create(parent, id, pos, size, style, name);
    p->cpWebView = wxWebView::New(p, wxID_ANY, basemapHtmlFileName, pos, size, backend, style, name);
    wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
    bs->Add(p->cpWebView, 1, wxEXPAND);
    p->SetSizerAndFit(bs);
    return p;
}

wxWebView* wxWebMapImpl::GetWebView()
{
    return cpWebView;
}

bool wxWebMapImpl::AddMapObject(wxMapObject const& o, wxString* result)
{
    cpWebView->RunScript(o.GetJavaScriptAdd(cMapName), result);
    return true;
}

void wxWebMapImpl::SetMapName(wxString const& name)
{
    cMapName = name;
}


