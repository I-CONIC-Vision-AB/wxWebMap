
function marker_add(lat, lon, map) {
    var marker = new L.marker([lat, lon]); marker.addTo(map); return "MARKER," + marker._leaflet_id + "," + lat + "," + lon;
};
function marker_add_drag(lat, lon, map) {
    var marker = new L.marker([lat, lon], { draggable: true }); marker.addTo(map); return "MARKER," + marker._leaflet_id + "," + lat + "," + lon;
};
