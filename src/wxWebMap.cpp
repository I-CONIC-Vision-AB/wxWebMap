#include <detail/wxWebMapImpl.h>
#include <wxMapHtml.h>
#include <wx/log.h>
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/filename.h>
#include <wx/fs_mem.h>

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
    bool bUseMemoryFS = false;//(backend != wxWebViewBackendEdge);
    wxString url = basemapHtmlFileName;
    wxWebMapImpl* p = new wxWebMapImpl();
    p->cpMapHtml = std::make_shared<wxMapHtml>(basemapHtmlFileName, bUseMemoryFS);
    p->wxWindow::Create(parent, id, pos, size, style, name);

    wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
    p->cpWebView = wxWebView::New(p, wxID_ANY, url, pos, size, backend, style, name);
    if (!p->cpWebView) {
        wxLogError(_("Web view could not be created"));
    } else {
        bs->Add(p->cpWebView, 1, wxEXPAND);
    }
    p->SetSizerAndFit(bs);
    if (bUseMemoryFS) {
        p->cpWebView->LoadURL("memory:" + p->cpMapHtml->GetMemoryFileName()); // It is strange that this url can not be given in the wxWebView construction above... There, LoadURL is called also, so it should work
    } else {
        wxString filename = p->cpMapHtml->GetLocalFileName().GetFullPath();
        p->cpWebView->LoadURL(filename); // It is strange that this url can not be given in the wxWebView construction above... There, LoadURL is called also, so it should work
    }
    return p;
}

wxWebView* wxWebMapImpl::GetWebView()
{
    return cpWebView;
}

bool wxWebMapImpl::AddMapObject(pwxMapObject o, wxString* WXUNUSED(result))
{
    if (std::find(clMapObjects.begin(), clMapObjects.end(), o) == clMapObjects.end()) {
        // We run java script async because otherwise wxYield is called which may trigger unwanted events before we get our result
        clMapObjects.push_back(o);
        cpWebView->RunScriptAsync(o->GetJavaScriptAdd(cMapName), &o);
    }

    return true;
}

bool wxWebMapImpl::DeleteMapObject(pwxMapObject o)
{
    cpWebView->RunScript(o->GetRemoveString(cMapName));
    GetMapObjects().remove(o);

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

pwxMapObject wxWebMapImpl::Find(wxString const& result) {
    int id;
    EMapObjectType type;
    wxMapObject::ParseResult(result, type, id);
    for(auto pMapObject : clMapObjects) {
        if (*pMapObject == result) {
            return pMapObject;
        }
    }
    return pwxMapObject();
}
