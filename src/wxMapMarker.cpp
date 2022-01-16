#include	<wxMapMarker.h>

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
    return boost::make_shared<wxMapMarker>(lat, lon, bDraggable);
}
