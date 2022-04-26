#include	<wxMapPolygon.h>
#include	<wxMapObject.h>
#include    <wx/tokenzr.h>
#include    <wx/log.h>
#include    <wx/intl.h>
#include <iostream>
using namespace std;




wxMapPolygon::wxMapPolygon(std::vector<wxMapPoint> const& vPoints) :
    coordinates(vPoints)
{
    cType = EMapObjectType::POLYGON;
}

wxString wxMapPolygon::GetJavaScriptAdd(wxString map) const
{
    string calls = "polygon_add(\"" + std::to_string(coordinates.size());
    for each (wxMapPoint mapPoint in coordinates)
    {
        calls = calls + ("," + std::to_string(mapPoint.x) + "," + std::to_string(mapPoint.y));
    }
    calls = calls + ("\" ," + std::string(map.mb_str()) + ");");
    return wxString::FromUTF8(calls);
}

pwxMapPolygon wxMapPolygon::Create(std::vector<wxMapPoint>const& vPoints)
{
    return boost::make_shared<wxMapPolygon>(vPoints);
}
