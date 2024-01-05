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

    void AddRectangleToWebMap(float MinX, float MaxX, float MinY, float MaxY, bool UseAsRegionOfInterest) {
        /*
        * Adds a leaflet rectangle to the webmap.
        // define rectangle geographical bounds
        var bounds = [[54.559322, -5.767822], [56.1210604, -3.021240]];
        // create an orange rectangle
        L.rectangle(bounds, {color: "#ff7800", weight: 1}).addTo(map);
        */
        wxString AddRectangleCommand = wxString::Format("L.rectangle([[%f, %f], [%f, %f]]).addTo(%s)", MinX, MinY, MaxX, MaxY, cMapName);
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

    wxString cMapName;
    wxWebView* cpWebView;
    std::list<pwxMapObject> clMapObjects;
    std::shared_ptr<wxMapHtml> cpMapHtml;
};
