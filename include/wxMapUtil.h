#pragma once

#include    <webmap_exports.h>
#include    <wx/event.h>

/**
 * @brief 2D point
*/
class ICONIC_WEBMAP_EXPORT wxMapPoint {
public:
    wxMapPoint() : x(0.0f), y(0.0f) {}
    wxMapPoint(float lat, float lon) : x(lat), y(lon) {}

    float operator[](int idx) {
        switch (idx) {
        case 0:
            return x;
            break;
        case 1:
            return y;
        default:
            return 0.0f;
        }
    }

    float x;
    float y;
};


/**
 * @brief Use case 4.1. Defines action on mouse events.
 *
 * Probably multiple modes may be active simultaneously; e.g. pan when holding left down and moving in map while DRAW on left mouse click.
 * @todo Implement
*/
enum class ICONIC_WEBMAP_EXPORT EActionMode {
    PAN=1,        //!< Default. Move around in the map. Move in map when mouse is moving and left is down.
    DRAW=2,       //!< E.g. add a point on left mouse click. Currently also default in sample application (but shouldn´t really be default)
    SELECT=4      //!< Select map object under mouse cursor on left click
};