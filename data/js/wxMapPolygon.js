

var polygons = [];
var nextPolygon = [];

function polygon_add(data, map) {

    var input = data.split(",");
    var nextPolygon = [];
    var j = parseFloat(input[0]);
    if (j > 2) {
        for (var i = 1; i <= input.length - 2; i += 2) {
            nextPolygon.push([parseFloat(input[i]), parseFloat(input[i + 1])]);
        }
        var polygon = new L.polygon(nextPolygon);
        polygon.addTo(map);
        polygons[polygon._leaflet_id] = polygon;
        return "POLYGON," + polygon._leaflet_id;
    }
    return "FALSE";
}

function mapobject_remove(id, map) {
    map.removeLayer(polygons[id]);
    delete polygons[id];
};