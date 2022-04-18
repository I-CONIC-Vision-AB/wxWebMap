
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
//Test
function image_add(lat, lon, map) {
    // image add! !!!!
    imageUrl = "https://upload.wikimedia.org/wikipedia/commons/2/20/Stockholm-Drone-010_%2828675114140%29.jpg";
    var center = [59.426180, 18.472263]; // fixa
    imageBounds = [center, [lat, lon]];
    var image = L.imageOverlay(imageUrl, imageBounds);
    image.addTo(map);
    //markers[marker._leaflet_id] = marker;
    return "IMAGE," + image._leaflet_id + "," + lat + "," + lon;
};
