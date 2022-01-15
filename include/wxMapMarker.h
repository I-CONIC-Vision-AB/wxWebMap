#pragma once
#include    <wxMapObject.h>

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
     * @param bRemovable Removable (Adds marker to a visible html list with a remove link)
    */
    wxMapMarker(double lat, double lon, bool bDraggable = false, bool bRemovable = false);

    /**
     * @brief Returns script to run to add this object
     * @param map Map name
     * @return script to run to add this object
    */
    virtual wxString GetJavaScriptAdd(wxString map) const;

    double cLat;
    double cLon;
    bool cbDraggable;
    bool cbRemovable;
};