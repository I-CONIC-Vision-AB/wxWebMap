#include	<wxMapObject.h>
#include    <wx/tokenzr.h>
#include    <wx/log.h>
#include    <wx/intl.h>

wxMapObject::wxMapObject() :
    cLeafletId(-1) {}

wxString wxMapObject::GetRemoveString(wxString const& map)
{
    return wxString::Format("mapobject_remove(%d, %s); \n", cLeafletId, map);
}


bool wxMapObject::ParseResult(wxString const& result, EMapObjectType& type, int& id)
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
            else if (token.IsSameAs("POLYGON") || token.IsSameAs("POLYLINE")) {
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
        }
        ++i;
    }
    return bOk;
}

bool wxMapObject::operator==(const wxString& result) {
    EMapObjectType type;
    int id;
    if (!ParseResult(result, type, id)) return false;
    if (type != cType) return false;
    if (cLeafletId > -1 && cLeafletId != id) {
        // There is an id set and it differs from the result id
        return false;
    }
    return true;
}
