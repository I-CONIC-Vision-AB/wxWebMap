
var polygons = new Array();
var nextPolygon = new Array();
function polygon_coord_add(lat, lon) {
    var pt = new Array();
    pt[0] = lat;
    pt[1] = lon;
    nextPolygon.push(pt);
};

function polygon_add(map, opacity, weight, color) {
    
    var polygonOptions = {
        color: color,
        fillColor: color,
        fillOpacity: opacity,
        weight: weight
    }

    var polygon = new L.polygon(nextPolygon, polygonOptions);
    polygon.addTo(map);
    map.setView([nextPolygon[0][0], nextPolygon[0][1]]);
    polygons[polygon._leaflet_id] = polygon;
    nextPolygon = new Array();
    return "POLYGON," + polygon._leaflet_id;
};

function polyline_add(map, weight, color) {

    var polylineOptions = {
        color: color,
        weight: weight
    }

    var polyline = new L.polyline(nextPolygon, polylineOptions);
    polyline.addTo(map);
    map.setView([nextPolygon[0][0], nextPolygon[0][1]]);
    polygons[polyline._leaflet_id] = polyline;
    nextPolygon = new Array();
    return "POLYLINE," + polyline._leaflet_id;
};

function polygon_remove(id, map) {
    map.removeLayer(polygons[id]);
    delete polygons[id];
};
