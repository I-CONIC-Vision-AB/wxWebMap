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
 * @brief A rectangular area.
*/
class ICONIC_WEBMAP_EXPORT wxMapRect {
public:
    /**
     * @brief Empty constructor
    */
    wxMapRect() :
        x(0.0f), y(0.0f), w(0.0f), h(0.0f) {}

    /**
     * @brief Constructor
     * @param lat Latitude
     * @param lon Longitude
     * @param width Width (latitude right minus latitude left, so north-south really)
     * @param height Height (longitude right minus longitude left, so east-west really)
    */
    wxMapRect(float lat, float lon, float width, float height) :
        x(lat), y(lon), w(width), h(height) {}

    float x; //!< Latitude
    float y; //!< Longitude
    float w; //!< Width (lat_max-x)
    float h; //!< Height (lon_max-y)

    /**
     * @brief Enable indexed access.
     *
     * \code
     * wxMapRect r(58.0f, 18.0f, 0.001f, 0.00025f);
     * assert(r[0] == 58.0f);
     * assert(r[1] == 18.0f);
     * assert(r[2] == 0.001f);
     * assert(r[3] == 0.00025f);
     * \endcode
     * @param idx Index
     * @return result
    */
    float operator[](int idx) {
        switch (idx) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return w;
        case 3:
            return h;
        default:
            return 0.0f;
        }
    }
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