#include	<PolygonReader.h>
#include	<wx/textfile.h>
#include    <wx/log.h>
#include    <wx/intl.h>
#include    <wx/tokenzr.h>
#include    <list>

PolygonReader::PolygonReader(wxString const& filename, std::vector<std::vector<wxMapPoint >> &polygons, std::vector<wxString> &metaData)
{
    wxTextFile file(filename);
    if (!file.Open()) {
        wxLogError(_("Couldnot open %s"), filename);
        return;
    }
    std::vector<wxMapPoint > polygon;
    wxString meta;
    wxString s = file.GetFirstLine();
    while (ReadPolygon(file, s, polygon, meta)) {
        polygons.push_back(polygon);
        metaData.push_back(meta);
        if (file.Eof()) {
            break;
        }
        s = file.GetNextLine(); // Read line after empty line
    }

}

bool PolygonReader::ReadPolygon(wxTextFile& file, wxString &s, std::vector<wxMapPoint > &polygon, wxString &meta)
{
    polygon.clear(); // Start a new polygon
    meta = s;
    s = file.GetNextLine();
    wxMapPoint pt;
    while (!s.IsEmpty()) {
        wxArrayString tokens = wxStringTokenize(s, ",");
        sscanf(tokens[0].char_str(), "%f", &(pt[1])); // First in file is longitude, but we want latitude first
        sscanf(tokens[1].char_str(), "%f", &(pt[0]));
        // Ignore third height/altitude token for now since point is 2D
        polygon.push_back(pt);
        s = file.GetNextLine();
    }
    polygon.pop_back(); // The last point is a "center of polygon point", not needed for creating the polygon
    // Empty line found so end of polygon
    return true;
}

