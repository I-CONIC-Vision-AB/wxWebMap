#include	<wxMapImage.h>
#include    <wx/tokenzr.h>
#include    <wx/log.h>
#include    <wx/intl.h>
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
    return boost::make_shared<wxMapImage>(upperLeftLat, upperLeftlon, lowerRightLat, lowerRightLon, imageUrl);
}
