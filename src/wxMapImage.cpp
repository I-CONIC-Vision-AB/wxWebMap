#include <wxMapImage.h>
#include <wx/tokenzr.h>
#include <wx/log.h>
#include <wx/intl.h>
#include <wxMapObject.h>

wxMapImage::wxMapImage(double upperLeftLat, double upperLeftlon, double lowerRightLat, double lowerRightLon, wxString imageUrl) :
    cLeftLat(upperLeftLat),
    cLeftLon(upperLeftlon),
    cRightLat(lowerRightLat),
    cRightLon(lowerRightLon),
    cUrl(imageUrl)
{
    cUrl.Replace('\\', '/');
    cType = EMapObjectType::IMAGE;
}

wxString wxMapImage::GetJavaScriptAdd(wxString map) const
{
    return "image_add('" + cUrl + wxString::Format("',%.6lf,%.6lf,%.6lf,%.6lf,%s", cLeftLat, cLeftLon, cRightLat, cRightLon, map) + "); \n";
}

pwxMapImage wxMapImage::Create(double upperLeftLat, double upperLeftlon, double lowerRightLat, double lowerRightLon, wxString imageUrl)
{
    return std::make_shared<wxMapImage>(upperLeftLat, upperLeftlon, lowerRightLat, lowerRightLon, imageUrl);
}

bool wxMapImage::ParseResult(const wxString& result, EMapObjectType& type, int& id, wxString& url) {
    wxStringTokenizer parse(result, ",");
    int i = 0;
    bool bOk = true;
    double l[4];
    int j = 0;
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
        case 2:
        case 3:
        case 4:
        case 5: {
            if (!token.ToDouble(&(l[j++]))) {
                wxLogError(_("Could not parse result as coordinate: %s"), token);
                bOk = false;
            }
        }
              break;
        case 6: {
            url = token;
        }
              break;
        }
        ++i;
    }
    return bOk;
}

bool wxMapImage::operator==(const wxString& result) {
    EMapObjectType type;
    int id;
//    double latmin, lonmin, latmax, lonmax;
    wxString url;
    if (!ParseResult(result, type, id, url)) return false;
    if (type != cType) return false;
    if (cLeafletId > -1 && cLeafletId != id) {
        // There is an id set and it differs from the result id
        return false;
    }
    if (!cUrl.IsSameAs(url)) return false;
    return true;
}

