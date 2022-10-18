#include	<wxMapMarker.h>
#include    <wx/tokenzr.h>
#include    <wx/log.h>
#include    <wx/intl.h>

wxMapMarker::wxMapMarker(double lat, double lon, bool bDraggable) :
    cLat(lat),
    cLon(lon),
    cbDraggable(bDraggable)
{
    cType = EMapObjectType::MARKER;
}

wxString wxMapMarker::GetJavaScriptAdd(wxString map) const
{
    if (!cbDraggable) {
        return wxString::Format("marker_add(%.6lf,%.6lf,%s); \n", cLat, cLon, map);
    } else {
        return wxString::Format("marker_add_drag(%.6lf,%.6lf,%s); \n", cLat, cLon, map);
    }
    return wxEmptyString;
}

pwxMapMarker wxMapMarker::Create(double lat, double lon, bool bDraggable)
{
    return std::make_shared<wxMapMarker>(lat, lon, bDraggable);
}

bool wxMapMarker::ParseResult(wxString const& result, EMapObjectType& type, int& id, double lat, double lon)
{
    wxStringTokenizer parse(result, ",");
    int i = 0;
    bool bOk = true;
    while (parse.HasMoreTokens()) {
        wxString token = parse.GetNextToken();
        switch (i) {
        case 0:
            // Parse type:
            if (token.IsSameAs("MARKER")) {
                type = EMapObjectType::MARKER;
            }
            else if (token.IsSameAs("POLYGON")) {
                type = EMapObjectType::POLYGON;
            }
            else if (token.IsSameAs("IMAGE")) {
                type = EMapObjectType::IMAGE;
            }
            break;
        case 1:

        {
            // Parse id
            long lid;
            if (!token.ToLong(&lid)) {
                wxLogError(_("Could not parse result as id: %s"), token);
                bOk = false;
            }
            id = (int)lid;
        }
        break;
        case 2: {
            if (!token.ToDouble(&lat)) {
                wxLogError(_("Could not parse result as lat: %s"), token);
                bOk = false;
            }
        }
              break;
        case 3: {
            if (!token.ToDouble(&lon)) {
                wxLogError(_("Could not parse result as lon: %s"), token);
                bOk = false;
            }
        }
              break;
        }
        ++i;
    }
    return bOk;

}

bool wxMapMarker::operator==(const wxString& result) {
    EMapObjectType type;
    int id;
    double lat=0.0, lon=0.0;
    if (!ParseResult(result, type, id, lat, lon)) return false;
    if (type != cType) return false;
    if (std::fabs(lat - cLat) < 1.0e-9 && std::fabs(lon - cLon) < 1.0e-9) return true;
    else return false;
}

