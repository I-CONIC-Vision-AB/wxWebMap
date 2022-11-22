#include	<wxMapPolygon.h>
#include	<wxMapObject.h>
#include    <string>
#include    <wx/tokenzr.h>
#include    <wx/log.h>
#include    <wx/intl.h>

wxMapPolygon::wxMapPolygon(std::vector<wxMapPoint> const& vPoints, bool bIsPolygon, float opacity, float weight, wxString color) :
    cOpacity(opacity),
    coordinates(vPoints),
    cWeight(weight),
    cColor(color),
    cbIsPolygon(bIsPolygon)
{
    cType = EMapObjectType::POLYGON;
}

wxString wxMapPolygon::GetJavaScriptAdd(wxString map) const
{
    wxString js;
    for (int i=0; i < static_cast<int>(coordinates.size()); ++i)
    {
        js += wxString::Format("polygon_coord_add(%.6f,%.6f);\n", coordinates[i].x, coordinates[i].y);
    }
    if (cbIsPolygon) {
        js += "polygon_add(" + wxString::Format("%s,%f,%f,'", map, cOpacity, cWeight) + cColor + "' ); \n";
    }
    else {
        js += "polyline_add(" + wxString::Format("%s,%f,'", map, cWeight) + cColor + "' ); \n";
    }
    return wxString(js);
}

pwxMapPolygon wxMapPolygon::Create(std::vector<wxMapPoint> const &vPoints, bool bIsPolygon, float opacity, float weight, wxString color)
{
    return std::make_shared<wxMapPolygon>(vPoints,bIsPolygon,opacity,weight,color);
}

wxString wxMapPolygon::GetRemoveString(wxString const& map)
{
    return wxString::Format("polygon_remove(%d, %s); \n", cLeafletId, map);
}