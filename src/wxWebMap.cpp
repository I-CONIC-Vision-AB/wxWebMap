#include	<detail/wxWebMapImpl.h>
#include    <wxMapHtml.h>
#include    <wx/log.h>
#include    <wx/intl.h>
#include    <wx/sizer.h>
#include    <wx/filename.h>
#include    <wx/fs_mem.h>

wxWebMap::wxWebMap() :
    wxWindow()
{

}

wxWebMap* wxWebMap::Create(wxWindow* parent, wxWindowID id, wxString const& basemapHtmlFileName, const wxPoint& pos, const wxSize& size, const wxString& backend, long style, const wxString& name)
{
    return wxWebMapImpl::Create(parent, id, basemapHtmlFileName, pos, size, backend, style, name);
}

wxWebMapImpl::wxWebMapImpl() :
    wxWebMap(),
    cMapName("map"),
    cpWebView(nullptr)
{
}

wxWebMap* wxWebMapImpl::Create(wxWindow* parent, wxWindowID id, wxString const& basemapHtmlFileName, const wxPoint& pos, const wxSize& size, const wxString& backend, long style, const wxString& name)
{
    wxString url = basemapHtmlFileName;
    wxMapHtml html(basemapHtmlFileName);
    wxWebMapImpl* p = new wxWebMapImpl();
    p->wxWindow::Create(parent, id, pos, size, style, name);
    wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
    p->cpWebView = wxWebView::New(p, wxID_ANY, url, pos, size, backend, style, name);
    if (!p->cpWebView) {
        wxLogError(_("Web view could not be created"));
    } else {
        bs->Add(p->cpWebView, 1, wxEXPAND);
    }
    p->SetSizerAndFit(bs);
    p->cpWebView->LoadURL("memory:"+ html.GetMemoryFileName()); // It is strange that this url can not be given in the wxWebView construction above... There, LoadURL is called also, so it should work
    return p;
}

wxWebView* wxWebMapImpl::GetWebView()
{
    return cpWebView;
}

bool wxWebMapImpl::AddMapObject(pwxMapObject const o, wxString* result)
{
    cpWebView->RunScript(o->GetJavaScriptAdd(cMapName), result);
    clMapObjects.push_back(o);
    return true;
}

void wxWebMapImpl::SetMapName(wxString const& name)
{
    cMapName = name;
}

std::list<pwxMapObject>& wxWebMapImpl::GetMapObjects()
{
    return clMapObjects;
}
