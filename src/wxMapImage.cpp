#include	<wxMapImage.h>
#include    <wx/tokenzr.h>
#include    <wx/log.h>
#include    <wx/intl.h>

wxString wxMapObject::GetRemoveStringImage(wxString const& map)
{
    return wxString::Format("mapobject_remove(%d, %s); \n", cLeafletId, map);
}

bool wxMapObject::ParseResultImage(wxString const& result, EMapObjectType& type, int& id)
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

wxMapImage::wxMapImage(double upperLeftLat, double upperLeftlon, double lowerRightLat, double lowerRightLon, wxString imageUrl) :
    cLeftLat(upperLeftLat),
    cLeftLon(upperLeftlon),
    cRightLat(lowerRightLat),
    cRightLon(lowerRightLon),
    cUrl(imageUrl)
{
    cType = EMapObjectType::IMAGE;
}

wxString wxMapImage::GetJavaScriptAdd(wxString map) const
{
    return "image_add('" + cUrl + wxString::Format("',%.6lf,%.6lf,%.6lf,%.6lf,%s", cLeftLat, cLeftLon, cRightLat, cRightLon, map) + "); \n";
}

pwxMapImage wxMapImage::Create(double upperLeftLat, double upperLeftlon, double lowerRightLat, double lowerRightLon, wxString imageUrl)
{
    return boost::make_shared<wxMapImage>(upperLeftLat, upperLeftlon, lowerRightLat, lowerRightLon, imageUrl);
}
