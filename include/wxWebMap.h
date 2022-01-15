#pragma once

#include	<wx/webview.h>
#include    <exports.h>
#include    <wxMapMarker.h>

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
    virtual bool AddMapObject(wxMapObject const& o, wxString *result=nullptr) = 0;

    /**
     * @brief Set name of map.
     *
     * This must match the name in the html template map. Currently both template and name use "map" as map name
     * @param name name of map
     * @todo Enable multiple map names
    */
    virtual void SetMapName(wxString const& name) = 0;

protected:
    /**
    * @brief Empty constructor.
    */
    wxWebMap();
};
