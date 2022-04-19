#include	<wxMapImage.h>
#include    <wx/tokenzr.h>
#include    <wx/log.h>
#include    <wx/intl.h>

wxString wxMapObject::GetRemoveStringS(wxString const& map)
{
    return wxString::Format("mapobject_remove(%d, %s); \n", cLeafletId, map);
}

bool wxMapObject::ParseResultS(wxString const& result, EMapObjectType& type, int& id)
{
    wxStringTokenizer parse(result, ",");
    int i = 0;
    bool bOk = true;
    while (parse.HasMoreTokens()) {
        wxString token = parse.GetNextToken();
        switch (i) {
        case 0:
            // Parse type:
            if (token.IsSameAs("IMAGE")) {
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

wxMapImage::wxMapImage(double lat, double lon):
    cLat(lat),
    cLon(lon)
{
    cType = EMapObjectType::IMAGE;
}

wxString wxMapImage::GetJavaScriptAdd(wxString map) const
{
    

 
    return wxString::Format("image_add(%.6lf,%.6lf,%s); \n", cLat, cLon, map);
    
    return wxEmptyString;
}

pwxMapImage wxMapImage::Create(double lat, double lon)
{
    return boost::make_shared<wxMapImage>(lat, lon);
}

