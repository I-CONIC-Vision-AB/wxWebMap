/////////////////////////////////////////////////////////////////////////////
// Name:        wxWebMapFrame
// Purpose:     wxWebView with map sample
// Author:      H�kan Wiman
// Adapted from:      webview.cpp by Marianne Gagnon
// Copyright:   (c) 2022 I-CONIC Vision AB, Sweden
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include    <wx/frame.h>
#include    <wx/textctrl.h>
#include    <wx/artprov.h>
#include    <wx/infobar.h>
#include    <wx/toolbar.h>
#include    <wx/log.h>

#include    <wxWebMap.h>

#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "../sample.xpm"
#endif

#if wxUSE_STC
#include "wx/stc/stc.h"
#else
#include "wx/textctrl.h"
#endif

#if defined(__WXMSW__) || defined(__WXOSX__)
#include "stop.xpm"
#include "refresh.xpm"
#endif

#include "wxlogo.xpm"


//We map menu items to their history items
WX_DECLARE_HASH_MAP(int, wxSharedPtr<wxWebViewHistoryItem>,
                    wxIntegerHash, wxIntegerEqual, wxMenuHistoryMap);

/**
 * @brief Main frame for the application.
 *
 * Containts a wxWebView
*/
class WebFrame : public wxFrame {
public:
    WebFrame(const wxString& url);
    virtual ~WebFrame();

    /**
     * @brief Add leaflet marker
     * @param lat Latitude
     * @param lon Longitude
    */
    void AddMarker(double lat, double lon);

    /**
     * @brief Get the web view
     * @return the web view
    */
    wxWebView* GetBrowser();

    void UpdateState();
    void OnIdle(wxIdleEvent& evt);
    void OnUrl(wxCommandEvent& evt);
    void OnBack(wxCommandEvent& evt);
    void OnForward(wxCommandEvent& evt);
    void OnStop(wxCommandEvent& evt);
    void OnReload(wxCommandEvent& evt);
    void OnClearHistory(wxCommandEvent& evt);
    void OnEnableHistory(wxCommandEvent& evt);
    void OnNavigationRequest(wxWebViewEvent& evt);
    void OnNavigationComplete(wxWebViewEvent& evt);
    void OnDocumentLoaded(wxWebViewEvent& evt);
    void OnNewWindow(wxWebViewEvent& evt);
    void OnTitleChanged(wxWebViewEvent& evt);
    void OnSetPage(wxCommandEvent& evt);
    void OnViewSourceRequest(wxCommandEvent& evt);
    void OnViewTextRequest(wxCommandEvent& evt);
    void OnToolsClicked(wxCommandEvent& evt);
    void OnSetZoom(wxCommandEvent& evt);
    void OnError(wxWebViewEvent& evt);
    void OnPrint(wxCommandEvent& evt);
    void OnCut(wxCommandEvent& evt);
    void OnCopy(wxCommandEvent& evt);
    void OnPaste(wxCommandEvent& evt);
    void OnUndo(wxCommandEvent& evt);
    void OnRedo(wxCommandEvent& evt);
    void OnMode(wxCommandEvent& evt);
    void OnZoomLayout(wxCommandEvent& evt);
    void OnZoomCustom(wxCommandEvent& evt);
    void OnHistory(wxCommandEvent& evt);
    void OnScrollLineUp(wxCommandEvent&);
    void OnScrollLineDown(wxCommandEvent&);
    void OnScrollPageUp(wxCommandEvent&);
    void OnScrollPageDown(wxCommandEvent&);

    void RunScript(const wxString& javascript);
    void OnRunScriptStockholm(wxCommandEvent& evt);
    void OnRunScriptString(wxCommandEvent& evt);
    void OnRunScriptInteger(wxCommandEvent& evt);
    void OnRunScriptDouble(wxCommandEvent& evt);
    void OnRunScriptBool(wxCommandEvent& evt);
    void OnRunScriptObject(wxCommandEvent& evt);
    void OnRunScriptArray(wxCommandEvent& evt);
    void OnRunScriptDOM(wxCommandEvent& evt);
    void OnRunScriptUndefined(wxCommandEvent& evt);
    void OnRunScriptNull(wxCommandEvent& evt);
    void OnRunScriptDate(wxCommandEvent& evt);
#if wxUSE_WEBVIEW_IE
    void OnRunScriptObjectWithEmulationLevel(wxCommandEvent& evt);
    void OnRunScriptDateWithEmulationLevel(wxCommandEvent& evt);
    void OnRunScriptArrayWithEmulationLevel(wxCommandEvent& evt);
#endif
    void OnRunScriptCustom(wxCommandEvent& evt);
    void OnClearSelection(wxCommandEvent& evt);
    void OnDeleteSelection(wxCommandEvent& evt);
    void OnSelectAll(wxCommandEvent& evt);
    void OnLoadScheme(wxCommandEvent& evt);
    void OnUseMemoryFS(wxCommandEvent& evt);
    void OnFind(wxCommandEvent& evt);
    void OnFindDone(wxCommandEvent& evt);
    void OnFindText(wxCommandEvent& evt);
    void OnFindOptions(wxCommandEvent& evt);
    void OnEnableContextMenu(wxCommandEvent& evt);
    void OnEnableDevTools(wxCommandEvent& evt);
    //    void OnNavigationLoaded(wxWebViewEvent& evt);

private:
    wxTextCtrl* m_url;
    wxWebView* m_browser;

    wxToolBar* m_toolbar;
    wxToolBarToolBase* m_toolbar_back;
    wxToolBarToolBase* m_toolbar_forward;
    wxToolBarToolBase* m_toolbar_stop;
    wxToolBarToolBase* m_toolbar_reload;
    wxToolBarToolBase* m_toolbar_tools;

    wxToolBarToolBase* m_find_toolbar_done;
    wxToolBarToolBase* m_find_toolbar_next;
    wxToolBarToolBase* m_find_toolbar_previous;
    wxToolBarToolBase* m_find_toolbar_options;
    wxMenuItem* m_find_toolbar_wrap;
    wxMenuItem* m_find_toolbar_highlight;
    wxMenuItem* m_find_toolbar_matchcase;
    wxMenuItem* m_find_toolbar_wholeword;

    wxMenu* m_tools_menu;
    wxMenu* m_tools_history_menu;
    wxMenuItem* m_tools_layout;
    wxMenuItem* m_tools_tiny;
    wxMenuItem* m_tools_small;
    wxMenuItem* m_tools_medium;
    wxMenuItem* m_tools_large;
    wxMenuItem* m_tools_largest;
    wxMenuItem* m_tools_custom;
    wxMenuItem* m_tools_handle_navigation;
    wxMenuItem* m_tools_handle_new_window;
    wxMenuItem* m_tools_enable_history;
    wxMenuItem* m_edit_cut;
    wxMenuItem* m_edit_copy;
    wxMenuItem* m_edit_paste;
    wxMenuItem* m_edit_undo;
    wxMenuItem* m_edit_redo;
    wxMenuItem* m_edit_mode;
    wxMenuItem* m_scroll_line_up;
    wxMenuItem* m_scroll_line_down;
    wxMenuItem* m_scroll_page_up;
    wxMenuItem* m_scroll_page_down;
    wxMenuItem* m_script_stockholm;
    wxMenuItem* m_script_string;
    wxMenuItem* m_script_integer;
    wxMenuItem* m_script_double;
    wxMenuItem* m_script_bool;
    wxMenuItem* m_script_object;
    wxMenuItem* m_script_array;
    wxMenuItem* m_script_dom;
    wxMenuItem* m_script_undefined;
    wxMenuItem* m_script_null;
    wxMenuItem* m_script_date;
#if wxUSE_WEBVIEW_IE
    wxMenuItem* m_script_object_el;
    wxMenuItem* m_script_date_el;
    wxMenuItem* m_script_array_el;
#endif
    wxMenuItem* m_script_custom;
    wxMenuItem* m_selection_clear;
    wxMenuItem* m_selection_delete;
    wxMenuItem* m_find;
    wxMenuItem* m_context_menu;
    wxMenuItem* m_dev_tools;

    wxInfoBar* m_info;
    wxStaticText* m_info_text;
    wxTextCtrl* m_find_ctrl;
    wxToolBar* m_find_toolbar;

    wxMenuHistoryMap m_histMenuItems;
    wxString m_findText;
    int m_findFlags, m_findCount;
    long m_zoomFactor;

    // Last executed JavaScript snippet, for convenience.
    wxString m_javascript;
};