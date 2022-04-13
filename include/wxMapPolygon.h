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

    std::vector<wxMapPoint> coordinates;

    /**
     * @brief Use case 1.2
     * @param vPoints
    */
    wxMapPolygon(std::vector<wxMapPoint>& vPoints);
    bool wxMapPolygon::ParseResult(wxString const& result, EMapObjectType& type, int& id);

    wxString wxMapPolygon::GetRemoveString(wxString const& map);
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
    static boost::shared_ptr<wxMapPolygon> Create(std::vector<wxMapPoint> &vPoints);

};

typedef boost::shared_ptr<wxMapPolygon> pwxMapPolygon; //!< Smart pointer to a map polygon.
