/////////////////////////////////////////////////////////////////////////////
// Name:        wxMapImage
// Purpose:     Point image on map
// Author:      Sandra Reinecke ;)
// Copyright:   (c) 2022 I-CONIC Vision AB, Sweden
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include    <wxMapObject.h>

/**
 * @brief A marker object
*/
class ICONIC_WEBMAP_EXPORT wxMapImageS : public wxMapObject {
public:
    /**
     * @brief Constructor
     * @param lat Latitude
     * @param lon Longitude
     * @param bDraggable Draggable
    */
    wxMapImageS(double lat, double lon, bool bDraggable = false);

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
    static boost::shared_ptr<wxMapImageS> Create(double lat, double lon, bool bDraggable = false);

    double cLat;
    double cLon;
    bool cbDraggable;
};

typedef boost::shared_ptr<wxMapImageS> pwxMapImageS; //!< Smart pointer to a map marker.
