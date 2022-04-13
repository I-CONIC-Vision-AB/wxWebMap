/////////////////////////////////////////////////////////////////////////////
// Name:        wxWebMap
// Purpose:     Show and manipulate web maps in Windows, Linux and Mac applications
// Author:      Håkan Wiman
// Copyright:   (c) 2022 I-CONIC Vision AB, Sweden
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include    <webmap_exports.h>
#include	<wx/webview.h>
#include    <wxMapMarker.h>
#include    <wxMapPolygon.h>
#include    <wxMapUtil.h>
#include    <list>


/**
 * @brief A window displaying a map from a map source, such as WMS.
 *
 * \c wxWebMap uses a wxWebView to display map and execute javascripts. \c Leafletjs is used for handling the maps internally.
*/
class ICONIC_WEBMAP_EXPORT wxWebMap : public wxWindow {
public:

    /**
     * @brief Create a wxWebMap.
     *
     * wxWebMap does not inherit from wxWebView since there are multiple possible backends for the wxWebView.
     * Instead we use the same mechanism as when creating wxWebView.
     * @param parent Parent window
     * @param id Window ID
     * @param basemapHtmlFileName Template for html map including path (e.g. map.html)
     * @param pos Window position
     * @param size Window size
     * @param backend wxWebView backend
     * @param style Window style
     * @param name Window name
     * @return a wxWebMap
    */
    static wxWebMap* Create(wxWindow* parent,
                            wxWindowID id,
                            wxString const& basemapHtmlFileName = wxEmptyString,
                            const wxPoint& pos = wxDefaultPosition,
                            const wxSize& size = wxDefaultSize,
                            const wxString& backend = wxASCII_STR(wxWebViewBackendDefault),
                            long style = 0,
                            const wxString& name = wxASCII_STR(wxWebViewNameStr));

    /**
     * @brief Get the aggregated wxWebView.
     *
     * Can be used to e.g. run scripts with wxWebView::RunScript
     * @return the aggregated wxWebView.
    */
    virtual wxWebView* GetWebView() = 0;

    /**
     * @brief Add a map object to the map
     * @param o Map object
     * @param result If not null, it will contain the Leafletjs identity number as a astring
     * @return True on success
    */
    virtual bool AddMapObject(pwxMapObject o, wxString *result=nullptr) = 0;

    virtual bool DeleteMapObject(pwxMapObject o) = 0;

    /**
     * @brief Set name of map.
     *
     * This must match the name in the html template map. Currently both template and name use "map" as map name
     * @param name name of map
     * @todo Enable multiple map names
    */
    virtual void SetMapName(wxString const& name) = 0;

    /**
     * @brief Return list with map objects
     * @return list with map objects
    */
    virtual std::list<pwxMapObject>& GetMapObjects() = 0;

    /**
     * @brief Use case 3.1
     * Handle mouse clicks to draw polygon in when in draw mode.
     * @param e Mouse event
     * @todo Bind() OnLeftMouseClick with this event handler. Not likely to work directly since clicks are in aggregated web view, not in this window. Use wxOverlay?
    */
    virtual void OnLeftMouseClick(wxMouseEvent &e) {}

    /**
     * @brief Use case 4.2. Handle map object selected.
     * @param e Map event
     * @todo Implement
     * @sa wxMapEvent
    */
    virtual void OnMapObjectSelected(wxMapEvent &e) {}

    /**
     * @brief Use case 4.1. Set action on mouse events
     * @param mode Action mode
     * @todo Implement
    */
    virtual void SetAction(EActionMode mode) {}

    /**
     * @brief Use case 4.1. Add action. Probably multiple actions can be active simultaneously, e.g. draw and select
     * @param mode Action mode
     * @todo Implement. Perhaps add argument for which kind of event (e.g. wxEVT_LEFT_DOWN) the action is connected to.
    */
    virtual void AddAction(EActionMode mode) {}

protected:
    /**
    * @brief Empty constructor.
    */
    wxWebMap();
};
