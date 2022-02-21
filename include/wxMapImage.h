/////////////////////////////////////////////////////////////////////////////
// Name:        wxMapImage
// Purpose:     Image on map
// Author:
// Copyright:   (c) 2022 I-CONIC Vision AB, Sweden
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include    <wxMapObject.h>
#include	<wxMapUtil.h>
#include    <wx/image.h>

/**
 * @brief Image
 * @todo Implement
*/
class ICONIC_WEBMAP_EXPORT wxMapImage : public wxMapObject {
public:
    /**
     * @brief Use case 2.2
     * @param imageUrl
     * @param latLon
     * @todo Implement
    */
    wxMapImage(wxString const& imageUrl, wxMapPoint const &latLon);

    /**
     * @brief Use case 2.3
     * @param image
     * @param latitude
     * @param longitude
     * @todo Implement with lower prio
    */
    wxMapImage(wxImage const& image, float latitude, float longitude);

    /**
     * @brief Use case 2.4 (low prio)
     * @param imageUrl
    */
    wxMapImage(wxString const& imageUrl);

    /**
     * @brief Use case 2.5 (low prio)
     * @param imageUrl
     * @param polygon
    */
    wxMapImage(wxString const& imageUrl, std::vector<wxMapPoint> const& polygon);

    /**
     * @brief Returns the script to add this object to a map
     * @param map Map to add to
     * @return Java script to execute
     * @sa wxWebMap::AddMapObject
     * @todo Implement
    */
    virtual wxString GetJavaScriptAdd(wxString map) const = 0;

    /**
    * @brief Create an image
    * @return an image
    * @todo Implement
    */
    static boost::shared_ptr<wxMapImage> Create(wxString const& imageUrl, wxMapPoint const& latLon);

    /**
     * @brief Use case 2.3
     * @param image
     * @param latitude
     * @param longitude
     * @return Image
     * @todo Implement
    */
    static boost::shared_ptr<wxMapImage> Create(wxImage const& image, float latitude, float longitude);


};

typedef boost::shared_ptr<wxMapImage> pwxMapImage; //!< Smart pointer to an image.
