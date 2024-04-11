#include <detail/wxWebMapImpl.h>
#include <wxMapHtml.h>
#include <wx/log.h>
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/filename.h>
#include <wx/fs_mem.h>
#include <wx/tokenzr.h>
#include <sstream>

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
    auto f = &wxWebMapImpl::OnScriptResult;
    Bind(wxEVT_WEBVIEW_SCRIPT_RESULT, f, this);
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

    // Install message handler with the name wx_msg
    p->cpWebView->AddScriptMessageHandler("wx_msg");
    // Bind handler
    p->cpWebView->Bind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED, &wxWebMapImpl::OnScriptResult, p);

    return p;
}

void wxWebMapImpl::OnScriptResult(wxWebViewEvent& evt) {
    ParseRectangleEvent(evt);
    wxMapObject* o = (wxMapObject*)evt.GetClientData();

    if (!o) {
        return;
    }

    bool found = false; // will be called in many cases, make sure its the one we are intrested in (AddMapObject)
    for (auto it = clMapObjects.begin(); it != clMapObjects.end(); ++it) {
        if (it->get() == o) {
            found = true;
            break;
        }
    }

    if (!found) {
        return;
    }

    if (o->GetType() != EMapObjectType::MARKER) {
        return;
    }


    wxString result = evt.GetString();
    long val = 0;
    result.ToLong(&val);
    o->SetLeafletId(val);
}

void wxWebMapImpl::SetEventListener(wxEvtHandler* Listener) {
    this->EventListener = Listener;
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

        // Needs to be run after the wxEVT_WEBVIEW_LOADED event (which triggers SpacetimeView::OnDocumentLoaded)
        //  is received. See documentation for RunScript, which also is true for RunScriptAsync:
        //  https://docs.wxwidgets.org/stable/classwx_web_view.html#a67000a368c45f3684efd460d463ffb98
        cpWebView->RunScriptAsync(o->GetJavaScriptAdd(cMapName), o.get());
    }
    return true;
}

bool wxWebMapImpl::DeleteMapObject(pwxMapObject o)
{
    wxString javascript = o->GetRemoveString(cMapName);
    cpWebView->RunScript(javascript);
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

bool wxWebMapImpl::QueryLastSavedRectangle(roi_rectangle& Out) {
    bool Result = false;

    // Set only if first vertex has a value, consider adding better error checking?
    if (LastSavedRectangle.Rectangle[0].lat != 0.f && LastSavedRectangle.Rectangle[0].lng != 0.f) {
        Out = LastSavedRectangle;
        Result = true;
    }
    return(Result);
}

void wxWebMapImpl::AddPolygonToWebMap(roi_polygon& Polygon, bool UseAsRegionOfInterest) {
    std::stringstream In = {};
    In << "L.polygon([";
    for (int PointIndex = 0; PointIndex < Polygon.Points.size(); ++PointIndex) {
        lat_lng_coords *Point = &Polygon.Points[PointIndex];
        In << "[" << Point->lat << "," << Point->lng << "]";
        if (PointIndex != Polygon.Points.size() - 1) {
            In << ",";
        }
    }
    In << "]).addTo(";
    In << cMapName << ")";
    wxString AddPolygonCommand = In.str();
    cpWebView->RunScriptAsync(AddPolygonCommand);
}

void wxWebMapImpl::AddRectangleToWebMap(float MinX, float MaxX, float MinY, float MaxY, bool UseAsRegionOfInterest) {
    /*
    * Adds a leaflet rectangle to the webmap.
    // define rectangle geographical bounds
    var bounds = [[54.559322, -5.767822], [56.1210604, -3.021240]];
    // create an orange rectangle
    L.rectangle(bounds, {color: "#ff7800", weight: 1}).addTo(map);
    */
    wxString AddRectangleCommand = wxString::Format("L.rectangle([[%f, %f], [%f, %f]], {color: \"#ff7800\", weight: 1}).addTo(%s)", MinX, MinY, MaxX, MaxY, cMapName);
    cpWebView->RunScriptAsync(AddRectangleCommand);

    if (UseAsRegionOfInterest) {
        roi_rectangle ROI = {};
        ROI.Rectangle[0] = { MinX, MinY };
        ROI.Rectangle[1] = { MaxX, MinY };
        ROI.Rectangle[2] = { MaxX, MaxY };
        ROI.Rectangle[3] = { MinX, MaxY };
        LastSavedRectangle = ROI;
    }
}

void wxWebMapImpl::ParseRectangleEvent(wxWebViewEvent& evt) {
    wxString EventString = evt.GetString();
    const wxString CreateRectangleHeader = "Create: Rectangle";
    const wxString RemoveRectangleHeader = "Remove: Rectangle";
    if (EventString.StartsWith(CreateRectangleHeader)) {
        int RectangleVertexIndex = 0;
        wxStringTokenizer Tokenizer(EventString, "(");
        //The string we parse looks like this:
        //"Create: Rectangle 266 LatLng(FloatValueLat, FloatValueLng),LatLng(FloatValueLat, FloatValueLng),LatLng(FloatValueLat, FloatValueLng),LatLng(FloatValueLat, FloatValueLng)"
        wxString InitToken = Tokenizer.GetNextToken();
        int IDStart = CreateRectangleHeader.size() + 1;
        int IDEnd = InitToken.size() - 7;
        wxString LeafletIDString = InitToken.SubString(IDStart, IDEnd);
        int LeafletID = std::stoi(LeafletIDString.ToStdString());
        LastSavedRectangle.LeafletID = LeafletID;

        while (Tokenizer.HasMoreTokens()) {
            wxString Token = Tokenizer.GetNextToken();
            int LngStartIndex = Token.Find(",") + 1;
            wxString LatString = Token.substr(0, LngStartIndex - 1);
            LastSavedRectangle.Rectangle[RectangleVertexIndex].lat = std::stof(LatString.ToStdString());
            wxString LngString = Token.substr(LngStartIndex);
            LastSavedRectangle.Rectangle[RectangleVertexIndex].lng = std::stof(LngString.ToStdString());
            ++RectangleVertexIndex;
        }
        BroadcastROIChange(1);
    } else if (EventString.StartsWith(RemoveRectangleHeader)) {
        //wxStringTokenizer Tokenizer(EventString, "(");
        //wxString InitToken = Tokenizer.GetNextToken();
        //int IDStart = RemoveRectangleHeader.size() + 1;
        //int IDEnd = InitToken.size() - 7;
        //wxString LeafletIDString = InitToken.SubString(IDStart, IDEnd);
        //int LeafletID = std::stoi(LeafletIDString.ToStdString());
        //if (LastSavedRectangle.LeafletID == LeafletID) {
        //    LastSavedRectangle = {};
        //}

        LastSavedRectangle = {};
        BroadcastROIChange(0);
    }
}

void wxWebMapImpl::BroadcastROIChange(int ID) {
    if (EventListener) {
        wxCommandEvent Event = { };
        Event.SetEventType((int)WebMapEventIDS::ID_ROI_WAS_UPDATED);
        Event.SetId(ID);
        wxPostEvent(EventListener, Event);
    }
}