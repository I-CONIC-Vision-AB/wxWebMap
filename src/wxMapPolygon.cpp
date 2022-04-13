#include	<wxMapPolygon.h>
#include	<wxMapObject.h>
#include    <string>
#include    <wx/tokenzr.h>
#include    <wx/log.h>
#include    <wx/intl.h>


wxString wxMapPolygon::GetRemoveString(wxString const& map)
{
    return wxString::Format("mapobject_remove(%d, %s); \n", cLeafletId, map);
}

wxMapPolygon::wxMapPolygon(std::vector<wxMapPoint>& vPoints) :
    coordinates(vPoints)
{
    cType = EMapObjectType::POLYGON;
}

wxString wxMapPolygon::GetJavaScriptAdd(wxString map) const
{
    wxString calls = "";
    for each (wxMapPoint mapPoint in coordinates)
    {
        calls = calls + wxString::Format("polygon_coord_add(%.6lf,%.6lf); \n", mapPoint.x, mapPoint.y);
    }
    calls = calls + ("polygon_add(%s); \n", map);
    return wxString::FromUTF8(calls);
}

bool wxMapPolygon::ParseResult(wxString const& result, EMapObjectType& type, int& id)
{
    wxStringTokenizer parse(result, ",");
    int i = 0;
    bool bOk = true;
    while (parse.HasMoreTokens()) {
        wxString token = parse.GetNextToken();
        switch (i) {
        case 0:
            // Parse type:
            if (token.IsSameAs("POLYGON")) {
                type = EMapObjectType::POLYGON;
                break;
            }
            wxLogError(_("Could not create polygon"));
            bOk = false;
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
        }
        ++i;
    }
    return bOk;
}

pwxMapPolygon wxMapPolygon::Create(std::vector<wxMapPoint>& vPoints)
{
    return boost::make_shared<wxMapPolygon>(vPoints);
}