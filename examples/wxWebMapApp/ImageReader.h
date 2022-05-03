#pragma once
#include    <vector>
#include	<wxMapUtil.h>
#include    <wx/textfile.h>

/**
 * @brief Reads text files with polygons and image file names.
 * @todo Hardcoded thumbnail path + prefix are added for this esample application
*/
class ImageReader {
public:
    ImageReader(wxString const& filename, std::vector<std::pair<wxMapPoint, wxMapPoint>>& imagePoints, std::vector<wxString>& imagePaths);
protected:
    wxMapPoint ReadPoint(wxString str);
};
