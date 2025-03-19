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
	wxString prevName = {};
	if (isPolygon) {
		while (ReadPolygon(file, s, polygon, meta)) {
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
		unsigned long prevNum = 0;
		wxMapPoint pt;
		s = file.GetNextLine(); // Ignore header line 
		const int numDigits = 4; // We assume image files end with at least four digits. If not, trajectories will simply not be split
		while (!file.Eof()) {
			wxArrayString tokens = wxStringTokenize(s, ",");
			if (tokens.size() >= 3) {
				// Get the last four characters and convert to integers
				size_t len = tokens[0].length();
				if (len >= numDigits) {
					wxString sNum = tokens[0].SubString(len - numDigits, len - 1);
					unsigned long thisNum;
					if (sNum.ToULong(&thisNum)) {
						// Name ends with (at least) four digits
						if (thisNum != prevNum + 1) {
							// This one does not follow immediatelly after previous one so 
							// finalize trajectory and start a new one
							if (polygon.size()) {
								if (polygon.size() == 1) {
									// Draw a tiny line to symbolize a point 
									pt = polygon[0];
									pt.x += 0.000001f;
									polygon.push_back(pt);
								}
								polygons.push_back(polygon);
								metaData.push_back(tokens[0]);
							}
							polygon.clear();
						}
						prevNum = thisNum;
					}
				}
				sscanf(tokens[1].char_str(), "%f", &(pt[1])); // First in file is longitude, but we want latitude first
				sscanf(tokens[2].char_str(), "%f", &(pt[0]));
				polygon.push_back(pt);
			}

			s = file.GetNextLine(); // Read line after empty line
		}
		if (polygon.size()) {
			polygons.push_back(polygon);
			metaData.push_back(filename);
		}
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

