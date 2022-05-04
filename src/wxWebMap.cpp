#include	<detail/wxWebMapImpl.h>
#include    <wxMapHtml.h>
#include    <wx/log.h>
#include    <wx/intl.h>
#include    <wx/sizer.h>
#include    <wx/filename.h>
#include    <wx/fs_mem.h>
#include    <boost/algorithm/string.hpp>

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
    bool bUseMemoryFS = (backend != wxWebViewBackendEdge);
    wxString url = basemapHtmlFileName;
    wxWebMapImpl* p = new wxWebMapImpl();
    p->cpMapHtml = boost::make_shared<wxMapHtml>(basemapHtmlFileName, bUseMemoryFS);
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

    // Install message handler with the name wx_msg
    p->cpWebView->AddScriptMessageHandler("wx_msg");
    // Bind handler
    p->cpWebView->Bind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED, [p](wxWebViewEvent& evt) {
        wxString s = evt.GetString();
        wxLogMessage(s);
        std::string delimiter = ": ";
        size_t pos = 0;
        std::string function_token;
        std::string id_token;

        pos = s.find(delimiter);
        function_token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());

        pos = s.find(delimiter);
        id_token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());

        pos = s.find(delimiter);
        if (function_token == "Create") {
            p->StorePolygon(stoi(id_token), s);
        }
        else if (function_token == "Update") {
            p->RemovePolygon(stoi(id_token), s);
            p->StorePolygon(stoi(id_token), s);
        }
        else
        {
            p->RemovePolygon(stoi(id_token), s);
        }
        });


    return p;
}

void wxWebMap::RemovePolygon(int id, wxString newpolygon)
{
    for (int i = 0; i < polygons.size(); i++) {
        if (polygons.at(i).GetLeafletId() == id) {
            polygons.erase(polygons.begin() + i);
            break;
        }
    }
}

void wxWebMap::StorePolygon(int id, wxString newpolygon)
{
    std::string string = newpolygon.ToStdString();
    boost::erase_all(string, "LatLng(");
    boost::erase_all(string, " ");
    boost::erase_all(string, ")");

    std::string delimiter = ",";

    bool b = false;
    size_t pos = 0;
    std::string token;
    std::vector<wxMapPoint> points;
    while ((pos = string.find(delimiter)) != std::string::npos) {

        if (b) {
            float lat = ::atof(token.c_str());
            b = false;
            token = string.substr(0, pos);
            float lng = ::atof(token.c_str());
            wxMapPoint* point = new wxMapPoint(lat, lng);
            wxMapPoint notpointer = *point;
            points.push_back(notpointer);
        }
        else {
            b = true;
            token = string.substr(0, pos);

        }
        string.erase(0, pos + delimiter.length());
    }
    wxMapPolygon* polygon = new wxMapPolygon(points);
    polygon->SetLeafletId(id);
    polygons.push_back(*polygon);
}

wxWebView* wxWebMapImpl::GetWebView()
{
    return cpWebView;
}

bool wxWebMapImpl::AddMapObject(pwxMapObject o, wxString* result)
{
    cpWebView->RunScript(o->GetJavaScriptAdd(cMapName), result);
    if (result && !result->IsEmpty()) {
        int id;
        EMapObjectType type;
        wxMapObject::ParseResult(*result, type, id);
        o->SetLeafletId(id);
    }
    clMapObjects.push_back(o);

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
//
//std::list<pwxMapObject>& wxWebMapImpl::GetMapPolygon()
//{
//    return clMapObjects;
//}