/////////////////////////////////////////////////////////////////////////////
// Name:        wxMapObject
// Purpose:     Base class for map objects
// Author:      H�kan Wiman
// Copyright:   (c) 2022 I-CONIC Vision AB, Sweden
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <webmap_exports.h>
#include <wx/string.h>
#include <wx/event.h>
#include <memory>

/**
 * @brief Map object type.
 * @sa wxMapObject
*/
enum class ICONIC_WEBMAP_EXPORT EMapObjectType {
    MARKER,     //!< wxMapMarker
    POLYGON,    //!< wxMapPolygon
    IMAGE       //!< wxMapImage
};

/**
 * @brief A generic map object.
 *
 * Corresponds to a leaflet object, such as a L.marker
*/
class ICONIC_WEBMAP_EXPORT wxMapObject {
public:
    wxMapObject();

    /**
     * @brief Returns the script to add this object to a map
     * @param map Map to add to
     * @return Java script to execute
     * @sa wxWebMap::AddMapObject
    */
    virtual wxString GetJavaScriptAdd(wxString map) const = 0;

    /**
     * @brief Returns type of map object
     * @return type of map object
    */
    virtual EMapObjectType GetType() const {
        return cType;
    }

    virtual void SetLeafletId(int id) {
        cLeafletId = id;
    }

    virtual int GetLeafletId() const {
        return cLeafletId;
    }

    virtual wxString GetRemoveString(wxString const& map);

    static bool ParseResult(wxString const& result, EMapObjectType& type, int& id);

    /**
     * @brief Compare this object with a javascript result string.
     * 
     * Used to identify which object has been added.
     * @param result javascript result string
     * @return True if same type. (more criteria in subclasses)
    */
    virtual bool operator==(const wxString& result);

protected:
    EMapObjectType cType;
    int cLeafletId;
};

typedef std::shared_ptr<wxMapObject> pwxMapObject; //!< Smart pointer to a map object.

/**
 * @brief Specialized event for wxWebMap
 *
 * @todo Implement. Not sure if wxMapEvent is needed?
 * @sa wxWebMap::OnMapObjectSelected
*/
class ICONIC_WEBMAP_EXPORT wxMapEvent : public wxEvent {
public:
    /**
     * @brief Constructor
     * @param winid Window id
     * @param commandType Command type
     * @param pObject wxMapObject
    */
    wxMapEvent(int winid = 0, wxEventType commandType = wxEVT_NULL, pwxMapObject pObject = pwxMapObject()) : wxEvent(winid, commandType), cpObject(pObject) {}
    virtual ~wxMapEvent() {}

    pwxMapObject cpObject;
};



