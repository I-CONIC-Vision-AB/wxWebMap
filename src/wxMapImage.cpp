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

wxMapImage::wxMapImage(double lat, double lon, wxString url):
    cLat(lat),
    cLon(lon),
    cull(url)
{

    cType = EMapObjectType::IMAGE;
}

wxString wxMapImage::GetJavaScriptAdd(wxString map) const
{
   
    wxLogMessage(_("Added  %s"), cull);
    wxLogMessage(_("Added  %s"), map);
     
    
    return "image_add('" +cull+ wxString::Format("',%.6lf,%.6lf, %s", cLat, cLon, map) + "); \n"; 
    //return wxString::Format("image_add(%.6lf,%.6lf,%s,%s); \n", cLat, cLon,cull, map);
    //return wxString::Format("image_add(59.326180, 18.072263,59.326180, 18.072263,'cull',map); \n");
    
    //return wxEmptyString;
}

pwxMapImage wxMapImage::Create(double lat, double lon, wxString url)
{
    return boost::make_shared<wxMapImage>(lat, lon, url);
}

