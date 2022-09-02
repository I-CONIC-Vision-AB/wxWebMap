/////////////////////////////////////////////////////////////////////////////
// Name:        wxMapPolygon
// Purpose:     Polygon on map
// Author:
// Copyright:   (c) 2022 I-CONIC Vision AB, Sweden
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include    <wxMapObject.h>
#include	<wxMapUtil.h>
#include    <vector>

/**
 * @brief Polygon
 *
 * ToDo: Make templated like the following code with \c T a 2D point with \c operator[]
 * \code
 * template <typename T>
 * class wxMapPolygon{
 * wxMapPolygon(std::vector<T> &vPoints);
 * };
 * \endcode
 * @todo Implement
 * @todo Make wxMapPolygon a template class so that any 2D point with \c operator[] can be used.
*/
class ICONIC_WEBMAP_EXPORT wxMapPolygon : public wxMapObject {
public:
    /**
     * @brief A polygon or a polyline.
     * 
     * Last point of a polygon does not have to be same as first point - it will connect to start point anyway.
     * @param vPoints Vector with points
     * @param bIsPolygon Polygon=true, polyline=false
     * @param opacity Controls transparency
     * @param weight Thickness of border lines
     * @param color Color of border and interior
    */
    wxMapPolygon(std::vector<wxMapPoint> const &vPoints, bool bIsPolygon = true, float opacity = 0.5, float weight = 0.5, wxString color = wxString("blue"));

    /**
     * @brief Returns the script to add this object to a map
     * @param map Map to add to
     * @return Java script to execute
     * @sa wxWebMap::AddMapObject
    */
    virtual wxString GetJavaScriptAdd(wxString map) const;

    /**
    * @brief Create a polygon
    * @return a polygon
    */
    static boost::shared_ptr<wxMapPolygon> Create(std::vector<wxMapPoint> const &vPoints, bool bIsPolygon = true, float opacity = 0.5, float weight = 0.5, wxString color = wxString("blue"));
    float cOpacity;
    float cWeight;
    wxString cColor;
    bool cbIsPolygon;

    virtual wxString GetRemoveString(wxString const& map);

    virtual bool operator==(const wxString& result) { return wxMapObject::operator==(result); }

    std::vector<wxMapPoint> coordinates;
};

typedef boost::shared_ptr<wxMapPolygon> pwxMapPolygon; //!< Smart pointer to a map polygon.
