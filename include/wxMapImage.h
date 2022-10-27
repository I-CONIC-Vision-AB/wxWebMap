/////////////////////////////////////////////////////////////////////////////
// Name:        wxMapImage
// Purpose:     Image on map
// Author:
// Copyright:   (c) 2022 I-CONIC Vision AB, Sweden
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <wxMapObject.h>
#include <memory>

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
    wxMapImage(double upperLeftLat, double upperLeftlon, double lowerRightLat, double lowerRightLon, wxString imageUrl);

    /**
     * @brief Use case 2.3
     * @param image
     * @param latitude
     * @param longitude
     * @todo Implement with lower prio
    */
    //wxMapImage(wxImage const& image, float latitude, float longitude);

    /**
     * @brief Use case 2.6
     *
     * Show image inside arbitrary clockwise polygon.
     * - Top left corner of image is shown at position \c vPoints[0].
     * - Top right corner of image is shown at position \c vPoints[1].
     * - Bottom right corner of image is shown at position \c vPoints[2].
     * - Bottom left corner of image is shown at position \c vPoints[3].
     * @param image Image
     * @param vPoints Polygon
    */
    //wxMapImage(wxImage const& image, std::vector<wxMapPoint>& vPoints);

    /**
     * @brief Use case 2.7
     *
     * Show image in a rectangle.
     * - Top left corner of image is shown at position \c (bb[0],bb[1])=(bb.x,bb.y)
     * - Top right corner of image is shown at position \c (bb[0]+bb[2],bb[1])=(bb.x+bb.w,bb.y)
     * - Bottom right corner of image is shown at position \c (bb[0]+bb[2],bb[1]+bb[3])=(bb.x+bb.w,bb.y+bb.h)
     * - Bottom left corner of image is shown at position \c (bb[0],bb[1]+bb[3])=(bb.x,bb.y+bb.h)
     * @param image Image
     * @param bb Bounding box
    */
    //wxMapImage(wxImage const& image, wxMapRect& bb);

    /**
     * @brief Use case 2.4 (low prio)
     * @param imageUrl
    */
    //wxMapImage(wxString const& imageUrl);

    /**
     * @brief Use case 2.5 (low prio)
     * @param imageUrl
     * @param polygon
    */
    //wxMapImage(wxString const& imageUrl, std::vector<wxMapPoint> const& polygon);

    /**
     * @brief Returns the script to add this object to a map
     * @param map Map to add to
     * @return Java script to execute
     * @sa wxWebMap::AddMapObject
     * @todo Implement
    */
    virtual wxString GetJavaScriptAdd(wxString map) const;


    bool ParseResult(const wxString& result, EMapObjectType& type, int& id, wxString& url);
    bool operator==(const wxString& result);

    // Test
    static std::shared_ptr<wxMapImage> Create(double upperLeftLat, double upperLeftLon, double lowerRightLat, double lowerRightLon, wxString imageUrl);
    double cLeftLat;
    double cLeftLon;
    double cRightLat;
    double cRightLon;
    wxString cUrl;


};

typedef std::shared_ptr<wxMapImage> pwxMapImage; //!< Smart pointer to an image.
