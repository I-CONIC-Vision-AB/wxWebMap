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
     * @brief Use case 1.2
     * @param vPoints
    */
    wxMapPolygon(std::vector<wxMapPoint> const &vPoints, float opacity = 0.5, wxString color = wxString("blue"));

    /**
     * @brief Returns the script to add this object to a map
     * @param map Map to add to
     * @return Java script to execute
     * @sa wxWebMap::AddMapObject
    */
    virtual wxString GetJavaScriptAdd(wxString map) const;

    /**
    * @brief Create a polygon
    * @return a marker
    */
    static boost::shared_ptr<wxMapPolygon> Create(std::vector<wxMapPoint> const &vPoints, float opacity = 0.5, wxString color = wxString("blue"));
    float cOpacity;
    wxString cColor;

    virtual wxString GetRemoveString(wxString const& map);

    std::vector<wxMapPoint> coordinates;
};

typedef boost::shared_ptr<wxMapPolygon> pwxMapPolygon; //!< Smart pointer to a map polygon.
