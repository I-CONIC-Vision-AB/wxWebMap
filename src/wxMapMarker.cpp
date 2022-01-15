#include	<wxMapMarker.h>

wxMapMarker::wxMapMarker(double lat, double lon, bool bDraggable, bool bRemovable) :
    cLat(lat),
    cLon(lon),
    cbDraggable(bDraggable),
    cbRemovable(bRemovable)
{

}

wxString wxMapMarker::GetJavaScriptAdd(wxString map) const
{
    if (!cbDraggable) {
        if (!cbRemovable) {
            return wxString::Format("L.marker([%.6lf, %.6lf]).addTo(%s);\n", cLat, cLon, map);
        }
    } else {
        if (!cbRemovable) {
            return wxString::Format("L.marker([%.6lf, %.6lf], {draggable:true}).addTo(%s);\n", cLat, cLon, map);
        } else {
            return wxString::Format("function f() {var marker = new L.Marker([%.9lf, %.9lf], {draggable:true});\
        marker.addTo(%s);\
    markers[marker._leaflet_id] = marker;\
    $('#overlay > ul').append('<li>Marker ' + marker._leaflet_id + ' - <a href=\"#\" class=\"remove\" id=\"' + marker._leaflet_id + '\">remove</a></li>'); return marker._leaflet_id;}f();", cLat, cLon, map);
        }
    }
    return wxEmptyString;
    //wxString marker("var marker = new L.Marker([%.9lf, %.9lf]");
    //return wxString::Format("var marker = new L.Marker([%.9lf, %.9lf], {draggable:true});\
    //    marker.addTo(map);\
    //markers[marker._leaflet_id] = marker;\
    //%s;", cLat, cLon, cbDraggable ? "true" : "false", cbRemovable ? "$('#overlay > ul').append('<li>Marker ' + marker._leaflet_id + ' - <a href=\"#\" class=\"remove\" id=\"' + marker._leaflet_id + '\">remove</a></li>')" : wxEmptyString);
    //wxString sLeafletMarker("var marker = new L.Marker([%.9lf, %.9lf], {draggable:true});\
    //    marker.addTo(map);\
    //markers[marker._leaflet_id] = marker;\
    //$('#overlay > ul').append('<li>Marker ' + marker._leaflet_id + ' - <a href=\"#\" class=\"remove\" id=\"' + marker._leaflet_id + '\">remove</a></li>');");

}
