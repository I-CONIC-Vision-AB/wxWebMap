#pragma once

#include <wxWebMap.h>
#include <wxMapHtml.h>
#include <list>
#include <memory>

class wxWebMapImpl : public wxWebMap {
public:
    static wxWebMap* Create(wxWindow* parent, wxWindowID id, wxString const& basemapHtmlFileName, const wxPoint& pos, const wxSize& size, 
        const wxString& backend, long style, const wxString& name);

    void OnScriptResult(wxWebViewEvent& evt);

protected:
    wxWebMapImpl();

    virtual wxWebView* GetWebView();

    virtual bool AddMapObject(pwxMapObject o, wxString* result);
    virtual bool DeleteMapObject(pwxMapObject o);
    virtual void SetMapName(wxString const& name);
    virtual std::list<pwxMapObject>& GetMapObjects();
    virtual pwxMapObject Find(wxString const& result);

    bool QueryLastSavedRectangle(roi_rectangle& Out);
    void AddRectangleToWebMap(float MinX, float MaxX, float MinY, float MaxY, bool UseAsRegionOfInterest);
    void ParseRectangleEvent(wxWebViewEvent& evt);

    wxString cMapName;
    wxWebView* cpWebView;
    std::list<pwxMapObject> clMapObjects;
    std::shared_ptr<wxMapHtml> cpMapHtml;
    //May be saved to the project file as a region of interest
    roi_rectangle LastSavedRectangle = {};
};
