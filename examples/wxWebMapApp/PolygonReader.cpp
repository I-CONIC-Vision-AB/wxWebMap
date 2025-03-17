#include	<PolygonReader.h>
#include	<wx/textfile.h>
#include    <wx/log.h>
#include    <wx/intl.h>
#include    <wx/tokenzr.h>
#include    <list>

PolygonReader::PolygonReader(wxString const& filename, std::vector<std::vector<wxMapPoint >> &polygons, std::vector<wxString> &metaData, bool isPolygon)
{
    wxTextFile file(filename);
    if (!file.Open()) {
        wxLogError(_("Couldnot open %s"), filename);
        return;
    }
    std::vector<wxMapPoint > polygon;
    wxString meta;
	size_t num_lines = file.GetLineCount(), line = 0;
    wxString s = file.GetFirstLine();
	if (isPolygon) {
		while (ReadPolygon(file, s, polygon, meta, isPolygon)) {
			polygons.push_back(polygon);
			metaData.push_back(meta);
			if (file.Eof()) {
				break;
			}
			line += 7; // Each object frame takes seven lines including empty line
			if (line + 6 > num_lines) {
				// Can´t be more object frames now
				break;
			}
			s = file.GetNextLine(); // Read line after empty line
		}
	} else {
		wxMapPoint pt;
		s = file.GetNextLine(); // Ignore header line 
		while (!file.Eof()) {
			wxArrayString tokens = wxStringTokenize(s, ",");
			if (tokens.size() >= 3) {
				sscanf(tokens[1].char_str(), "%f", &(pt[1])); // First in file is longitude, but we want latitude first
				sscanf(tokens[2].char_str(), "%f", &(pt[0]));
				// Ignore third height/altitude token for now since point is 2D
				polygon.push_back(pt);
			}

			s = file.GetNextLine(); // Read line after empty line
		}
		polygons.push_back(polygon);
		metaData.push_back(filename);
	}

}

bool PolygonReader::ReadPolygon(wxTextFile& file, wxString &s, std::vector<wxMapPoint > &polygon, wxString &meta, bool isPolygon)
{
    polygon.clear(); // Start a new polygon
    meta = s;
    s = file.GetNextLine();
    wxMapPoint pt;
    while (!s.IsEmpty()) {
        wxArrayString tokens = wxStringTokenize(s, ",");
		if (tokens.size() >= 2) {
			sscanf(tokens[0].char_str(), "%f", &(pt[1])); // First in file is longitude, but we want latitude first
			sscanf(tokens[1].char_str(), "%f", &(pt[0]));
			// Ignore third height/altitude token for now since point is 2D
			polygon.push_back(pt);
		}
        s = file.GetNextLine();
    }
    polygon.pop_back(); // The last point is a "center of polygon point", not needed for creating the polygon
    // Empty line found so end of polygon
    return true;
}

