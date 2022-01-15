#pragma once

//#include	<IconicSensor/Defines.h>
#include    <exports.h>
#include    <boost/shared_ptr.hpp>
#include    <Eigen/Core>
#include    <wx/string.h>

namespace iconic {
/**
 * @brief Basic class for showing a web map with features.
 *
 * @todo Extend with more features than markers
*/
class ICONIC_WEBMAP_EXPORT MapHtml {
public:
    /**
     * @brief Create an instance of a web map
     * @return instance of a web map
    */
    static boost::shared_ptr<MapHtml> Create(wxString sHtmlBaseMapFileName = wxEmptyString);
    static bool SetDataDirectory(wxString const& dir);

    /**
     * @brief Reset to enable new map
     * @return True on success
    */
    virtual bool ClearAllMarkers() = 0;

    /**
     * @brief Add a markler at the given position
     * @param Pt (lon,lat,(alt))
    */
    virtual void AddMarker(Eigen::Vector3d const& Pt) = 0;

    /**
     * @brief Add a markler at the given position
     * @param latitude Latitude
     * @param longitude Longitude
     * @return
    */
    virtual void AddMarker(double longitude, double latitude) = 0;

    /**
     * @brief Explicitly set center coorinate.
     *
     * If not set, the center will be the average of the added markers
     * @param latitude Latitude
     * @param longitude Longiotude
     * @return True on success
    */
    virtual void SetCenter(double longitude, double latirude) = 0;

    /**
     * @brief Set zoom level
     * @param level Zoom level (14 default is very large scale)
    */
    virtual void SetZoom(int level = 14) = 0;

    /**
     * @brief Get result as HTML string
     * @return HTML string
    */
    virtual wxString GetHtml() = 0;

    /**
     * @brief Save web map to HTML file.
     * @param filename File name
     * @return True on success
     * @todo Save to stream
    */
    virtual bool Save(wxString const& filename) = 0;

    /**
     * @brief
     * @param width
     * @param height
    */
    virtual void SetSize(int width, int height) = 0;
};
typedef boost::shared_ptr<MapHtml> MapHtmlPtr; //!< Smart pointer to a web map
}