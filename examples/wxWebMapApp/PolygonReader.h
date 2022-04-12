#pragma once
#include    <vector>
#include	<wxMapUtil.h>
#include    <wx/textfile.h>

class PolygonReader {
public:
    PolygonReader(wxString const& filename, std::vector<std::vector<wxMapPoint>>& polygons, std::vector<wxString>& metaData);
protected:
    bool ReadPolygon(wxTextFile& file, wxString& s, std::vector<wxMapPoint >& polygon, wxString& meta);
};
