

var polygons = [];
var nextPolygon = [];
function polygon_coord_add(lat, lon){
    nextPolygon.push[lat, lon];
};

function polygon_add(map) {
    if (nextPolygon.length > 2) {
        var polygon = new L.polygon(nextPolygon);
        polygon.addTo(map);
        polygons[polygon._leaflet_id] = polygon;
        nextPolygon = [];
        return "POLYGON," + polygon._leaflet_id;
    }
    return "FALSE";
};

function mapobject_remove(id, map) {
    map.removeLayer(polygons[id]);
    delete polygons[id];
};