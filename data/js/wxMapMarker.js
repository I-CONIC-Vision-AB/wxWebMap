
// js code that is used by wxWebMap (executed with wxWebView::RunScript, which also receives returned values). See wxWebMap and wxMapMarker.
// The coded is added to any compatible HTML file using wxMapHtml.
var markers = new Array();
function marker_add(lat, lon, map) {
    var marker = new L.marker([lat, lon]); marker.addTo(map); 
    markers[marker._leaflet_id] = marker;
    return "MARKER," + marker._leaflet_id + "," + lat + "," + lon;
};
function marker_add_drag(lat, lon, map) {
    var marker = new L.marker([lat, lon], { draggable: true }); marker.addTo(map); 
    markers[marker._leaflet_id] = marker;
    return "MARKER," + marker._leaflet_id + "," + lat + "," + lon;
};
function mapobject_remove(id, map) {
    map.removeLayer(markers[id]);
    delete markers[id];
};
function image_add(url, lLat, lLon, rLat, rLon, map) {
    imageBounds = [[lLat, lLon], [rLat, rLon]];
    var image = L.imageOverlay(url, imageBounds);
    image.addTo(map);
    map.setView([lLat, lLon]); // ToDo: Should be center or image or center of all images added in a separate call
    return "IMAGE," + image._leaflet_id + "," + lLat + "," + lLon + "," + rLat + "," + rLon + "," + url;
};