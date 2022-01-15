#include	<detail/wxWebMapImpl.h>
#include    <wx/log.h>
#include    <wx/intl.h>
#include    <wx/sizer.h>

using namespace iconic;

wxWebMap::wxWebMap() :
    wxWindow()
{

}

wxWebMap* wxWebMap::Create(wxWindow* parent, wxWindowID id, wxString const& basemapHtmlFileName, wxString const& dataDirectory, const wxString& url, const wxPoint& pos, const wxSize& size, const wxString& backend, long style, const wxString& name)
{
    return wxWebMapImpl::Create(parent, id, basemapHtmlFileName, dataDirectory, url, pos, size, backend, style, name);
}

wxWebMapImpl::wxWebMapImpl(wxString const& basemapHtmlFileName, wxString const& dataDirectory) :
    wxWebMap()
{
    if (!dataDirectory.IsEmpty()) {
        MapHtml::SetDataDirectory(dataDirectory);
    }
    cpMap = MapHtml::Create(basemapHtmlFileName);
}

wxWebMap* wxWebMapImpl::Create(wxWindow* parent, wxWindowID id, wxString const& basemapHtmlFileName, wxString const& dataDirectory, const wxString& url, const wxPoint& pos, const wxSize& size, const wxString& backend, long style, const wxString& name)
{
    wxWebMapImpl* p = new wxWebMapImpl(basemapHtmlFileName, dataDirectory);
    p->wxWindow::Create(parent, id, pos, size, style, name);
    p->cpWebView = wxWebView::New(p, wxID_ANY, url, pos, size, backend, style, name);
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
    cpWebView->RunScript(o.GetJavaScriptAdd(cpMap->GetMapName()), result);
    return true;
}

