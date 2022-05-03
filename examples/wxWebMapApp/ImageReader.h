#pragma once
#include    <vector>
#include	<wxMapUtil.h>
#include    <wx/textfile.h>

class ImageReader {
public:
    ImageReader(wxString const& filename, std::vector<std::pair<wxMapPoint, wxMapPoint>>& imagePoints, std::vector<wxString>& imagePaths);
protected:
    wxMapPoint ReadPoint(wxString str);
};
