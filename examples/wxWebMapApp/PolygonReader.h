#pragma once
#include    <vector>
#include	<wxMapUtil.h>
#include    <wx/textfile.h>

/**
 * @brief Reads text files with footprints.
*/
class PolygonReader {
public:
    /**
     * @brief Constructor reads footprints from file.
     * @param filename I-CONIC footprint fie (*.ifp)
     * @param polygons Read polygons
     * @param metaData Name of each polygon
    */
    PolygonReader(wxString const& filename, std::vector<std::vector<wxMapPoint>>& polygons, std::vector<wxString>& metaData);
protected:
    /**
     * @brief Helper reading a single polygon
     * @param file File
     * @param s String
     * @param polygon Polygon
     * @param meta Name of polygon
     * @return True on success
    */
    bool ReadPolygon(wxTextFile& file, wxString& s, std::vector<wxMapPoint >& polygon, wxString& meta);
};
