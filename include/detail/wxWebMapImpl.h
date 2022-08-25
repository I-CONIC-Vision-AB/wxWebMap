#pragma once

#include	<wxWebMap.h>
#include    <wxMapHtml.h>
#include    <list>

class wxWebMapImpl : public wxWebMap {
public:
    static wxWebMap* Create(wxWindow* parent, wxWindowID id, wxString const& basemapHtmlFileName, const wxPoint& pos, const wxSize& size, 
        const wxString& backend, long style, const wxString& name);

protected:
    wxWebMapImpl();

    virtual wxWebView* GetWebView();

    virtual bool AddMapObject(pwxMapObject o, wxString* result);
    virtual bool DeleteMapObject(pwxMapObject o);
    virtual void SetMapName(wxString const& name);
    virtual std::list<pwxMapObject>& GetMapObjects();
    virtual pwxMapObject Find(wxString const& result);

    wxWebView* cpWebView;
    wxString cMapName;
    std::list<pwxMapObject> clMapObjects;
    boost::shared_ptr<wxMapHtml> cpMapHtml;
};
