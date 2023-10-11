/////////////////////////////////////////////////////////////////////////////
// Name:        wxMapMarker
// Purpose:     Point marker on map
// Author:      H�kan Wiman
// Copyright:   (c) 2022 I-CONIC Vision AB, Sweden
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <wxMapObject.h>
#include <memory>

/**
 * @brief A marker object
*/
class ICONIC_WEBMAP_EXPORT wxMapMarker : public wxMapObject {
public:
    /**
     * @brief Constructor
     * @param lat Latitude
     * @param lon Longitude
     * @param bDraggable Draggable
    */
    wxMapMarker(double lat, double lon, bool bDraggable = false);

    /**
     * @brief Returns script to run to add this object.
     *
     * See \c js/wxMapMarker.js.
     * @param map Map name
     * @return script to run to add this object
    */
    virtual wxString GetJavaScriptAdd(wxString map) const;

    /**
     * @brief Create a marker
     * @param lat Latitude
     * @param lon Longitude
     * @param bDraggable Draggable
     * @return a marker
    */
    static std::shared_ptr<wxMapMarker> Create(double lat, double lon, bool bDraggable = false);

    bool ParseResult(wxString const& result, EMapObjectType& type, int& id, double lat, double lon);

    virtual bool operator==(const wxString& result);

    wxString label;

    double cLat;
    double cLon;
    bool cbDraggable;
};

typedef std::shared_ptr<wxMapMarker> pwxMapMarker; //!< Smart pointer to a map marker.
