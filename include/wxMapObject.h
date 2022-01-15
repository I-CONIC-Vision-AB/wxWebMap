/////////////////////////////////////////////////////////////////////////////
// Name:        wxMapObject
// Purpose:     Base class for map objects
// Author:      Håkan Wiman
// Copyright:   (c) 2022 I-CONIC Vision AB, Sweden
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include    <exports.h>
#include    <wx/string.h>

/**
 * @brief A generic map object.
 *
 * Corresponds to a leaflet object, such as a L.marker
*/
class ICONIC_WEBMAP_EXPORT wxMapObject {
public:
    /**
     * @brief Returns the script to add this object to a map
     * @param map Map to add to
     * @return Java script to execute
     * @sa wxWebMap::AddMapObject
    */
    virtual wxString GetJavaScriptAdd(wxString map) const = 0;
};
