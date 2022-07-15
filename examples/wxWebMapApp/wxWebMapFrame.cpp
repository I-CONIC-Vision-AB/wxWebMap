#include	<wxWebMapFrame.h>
#include    <wxMapMarker.h>
#include    <wxMapPolygon.h>
#include    <SourceViewDialog.h>
#include    <PolygonReader.h>
#include    <ImageReader.h>
#include    <wx/sizer.h>
#include    <wx/panel.h>
#include    <wx/menu.h>
#include    <wx/filename.h>
#include    <wx/numdlg.h>
#include    <wx/textdlg.h>
#include    <wx/webview.h>
#include    <wx/filedlg.h>
#if wxUSE_WEBVIEW_IE
#include    <wx/msw/webview_ie.h>
#endif
#include    <wx/webviewarchivehandler.h>
#include    <wx/webviewfshandler.h>
#include    <boost/algorithm/string.hpp>
#include    <wxMapImage.h>
#include    <wx/colordlg.h>

WebFrame::WebFrame(const wxString& url) :
    wxFrame(NULL, wxID_ANY, "wxWebView Sample"),
    cbDraggable(false)
{
    // set the frame icon
    SetIcon(wxICON(sample));
    SetTitle("wxWebView Sample");

    CreateStatusBar(2);
    wxLogStatus(GetTitle());

    wxBoxSizer* topsizer = new wxBoxSizer(wxVERTICAL);

    // Create the toolbar
    m_toolbar = CreateToolBar(wxTB_TEXT);
    m_toolbar->SetToolBitmapSize(wxSize(32, 32));

    wxBitmap back = wxArtProvider::GetBitmap(wxART_GO_BACK, wxART_TOOLBAR);
    wxBitmap forward = wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_TOOLBAR);
#ifdef __WXGTK__
    wxBitmap stop = wxArtProvider::GetBitmap("gtk-stop", wxART_TOOLBAR);
#else
    wxBitmap stop = wxBitmap(stop_xpm);
#endif
#ifdef __WXGTK__
    wxBitmap refresh = wxArtProvider::GetBitmap("gtk-refresh", wxART_TOOLBAR);
#else
    wxBitmap refresh = wxBitmap(refresh_xpm);
#endif

    m_toolbar_back = m_toolbar->AddTool(wxID_ANY, _("Back"), back);
    m_toolbar_forward = m_toolbar->AddTool(wxID_ANY, _("Forward"), forward);
    m_toolbar_stop = m_toolbar->AddTool(wxID_ANY, _("Stop"), stop);
    m_toolbar_reload = m_toolbar->AddTool(wxID_ANY, _("Reload"), refresh);
    m_url = new wxTextCtrl(m_toolbar, wxID_ANY, "", wxDefaultPosition, wxSize(400, -1), wxTE_PROCESS_ENTER);
    m_toolbar->AddControl(m_url, _("URL"));
    m_toolbar_tools = m_toolbar->AddTool(wxID_ANY, _("Menu"), wxBitmap(wxlogo_xpm));

    m_toolbar->Realize();

    // Create the info panel
    m_info = new wxInfoBar(this);
    topsizer->Add(m_info, wxSizerFlags().Expand());

    // Create a log window
    new wxLogWindow(this, _("Logging"), true, false);

    // Create the webview
    wxString backend = wxWebViewBackendDefault;
// #ifdef __WXMSW__ Don�t have to check for windows
    if (wxWebView::IsBackendAvailable(wxWebViewBackendEdge)) {
        wxLogMessage("Using Edge backend");
        backend = wxWebViewBackendEdge;
    } else {
        wxLogMessage("Edge backend not available");
        // Edge does not support handlers, but the other webviews do
        //We register the wxfs:// protocol for testing purposes
        m_browser->RegisterHandler(wxSharedPtr<wxWebViewHandler>(new wxWebViewArchiveHandler("wxfs")));
        //And the memory: file system
        m_browser->RegisterHandler(wxSharedPtr<wxWebViewHandler>(new wxWebViewFSHandler("memory")));
    }
//#endif

    m_webmap = wxWebMap::Create(this, wxID_ANY, url, wxDefaultPosition, wxDefaultSize, backend);
    m_browser = m_webmap->GetWebView();
    topsizer->Add(m_webmap, wxSizerFlags().Expand().Proportion(1));

    SetSizer(topsizer);

    //Set a more sensible size for web browsing
    SetSize(wxSize(800, 600));

    // Create the Tools menu
    m_tools_menu = new wxMenu();
    wxMenuItem* print = m_tools_menu->Append(wxID_ANY, _("Print"));
    wxMenuItem* setPage = m_tools_menu->Append(wxID_ANY, _("Set page text"));
    wxMenuItem* viewSource = m_tools_menu->Append(wxID_ANY, _("View Source"));
    wxMenuItem* viewText = m_tools_menu->Append(wxID_ANY, _("View Text"));
    m_tools_menu->AppendSeparator();

    m_tools_menu->AppendSubMenu(CreateMapMenu(), "Map");

    m_tools_menu->AppendSeparator();
    m_tools_layout = m_tools_menu->AppendRadioItem(wxID_ANY, _("Use Layout Zoom"));
    m_tools_tiny = m_tools_menu->AppendRadioItem(wxID_ANY, _("Tiny"));
    m_tools_small = m_tools_menu->AppendRadioItem(wxID_ANY, _("Small"));
    m_tools_medium = m_tools_menu->AppendRadioItem(wxID_ANY, _("Medium"));
    m_tools_large = m_tools_menu->AppendRadioItem(wxID_ANY, _("Large"));
    m_tools_largest = m_tools_menu->AppendRadioItem(wxID_ANY, _("Largest"));
    m_tools_custom = m_tools_menu->AppendRadioItem(wxID_ANY, _("Custom Size"));
    m_tools_menu->AppendSeparator();
    m_tools_handle_navigation = m_tools_menu->AppendCheckItem(wxID_ANY, _("Handle Navigation"));
    m_tools_handle_new_window = m_tools_menu->AppendCheckItem(wxID_ANY, _("Handle New Windows"));
    m_tools_menu->AppendSeparator();

    //History menu
    m_tools_history_menu = new wxMenu();
    wxMenuItem* clearhist = m_tools_history_menu->Append(wxID_ANY, _("Clear History"));
    m_tools_enable_history = m_tools_history_menu->AppendCheckItem(wxID_ANY, _("Enable History"));
    m_tools_history_menu->AppendSeparator();

    m_tools_menu->AppendSubMenu(m_tools_history_menu, "History");

    //Create an editing menu
    wxMenu* editmenu = new wxMenu();
    m_edit_cut = editmenu->Append(wxID_ANY, _("Cut"));
    m_edit_copy = editmenu->Append(wxID_ANY, _("Copy"));
    m_edit_paste = editmenu->Append(wxID_ANY, _("Paste"));
    editmenu->AppendSeparator();
    m_edit_undo = editmenu->Append(wxID_ANY, _("Undo"));
    m_edit_redo = editmenu->Append(wxID_ANY, _("Redo"));
    editmenu->AppendSeparator();
    m_edit_mode = editmenu->AppendCheckItem(wxID_ANY, _("Edit Mode"));

    m_tools_menu->AppendSeparator();
    m_tools_menu->AppendSubMenu(editmenu, "Edit");

    wxMenu* scroll_menu = new wxMenu;
    m_scroll_line_up = scroll_menu->Append(wxID_ANY, "Line &up");
    m_scroll_line_down = scroll_menu->Append(wxID_ANY, "Line &down");
    m_scroll_page_up = scroll_menu->Append(wxID_ANY, "Page u&p");
    m_scroll_page_down = scroll_menu->Append(wxID_ANY, "Page d&own");
    m_tools_menu->AppendSubMenu(scroll_menu, "Scroll");

    wxMenu* script_menu = new wxMenu;
    m_script_string = script_menu->Append(wxID_ANY, "Return String");
    m_script_integer = script_menu->Append(wxID_ANY, "Return integer");
    m_script_double = script_menu->Append(wxID_ANY, "Return double");
    m_script_bool = script_menu->Append(wxID_ANY, "Return bool");
    m_script_object = script_menu->Append(wxID_ANY, "Return JSON object");
    m_script_array = script_menu->Append(wxID_ANY, "Return array");
    m_script_dom = script_menu->Append(wxID_ANY, "Modify DOM");
    m_script_undefined = script_menu->Append(wxID_ANY, "Return undefined");
    m_script_null = script_menu->Append(wxID_ANY, "Return null");
    m_script_date = script_menu->Append(wxID_ANY, "Return Date");
#if wxUSE_WEBVIEW_IE
    if (!wxWebView::IsBackendAvailable(wxWebViewBackendEdge)) {
        m_script_object_el = script_menu->Append(wxID_ANY, "Return JSON object changing emulation level");
        m_script_date_el = script_menu->Append(wxID_ANY, "Return Date changing emulation level");
        m_script_array_el = script_menu->Append(wxID_ANY, "Return array changing emulation level");
    }
#endif
    m_script_custom = script_menu->Append(wxID_ANY, "Custom script");
    m_tools_menu->AppendSubMenu(script_menu, _("Run Script"));

    //Selection menu
    wxMenu* selection = new wxMenu();
    m_selection_clear = selection->Append(wxID_ANY, _("Clear Selection"));
    m_selection_delete = selection->Append(wxID_ANY, _("Delete Selection"));
    wxMenuItem* selectall = selection->Append(wxID_ANY, _("Select All"));

    editmenu->AppendSubMenu(selection, "Selection");

    wxMenuItem* loadscheme = m_tools_menu->Append(wxID_ANY, _("Custom Scheme Example"));

    m_context_menu = m_tools_menu->AppendCheckItem(wxID_ANY, _("Enable Context Menu"));
    m_dev_tools = m_tools_menu->AppendCheckItem(wxID_ANY, _("Enable Dev Tools"));

    //By default we want to handle navigation and new windows
    m_tools_handle_navigation->Check();
    m_tools_handle_new_window->Check();
    m_tools_enable_history->Check();

    //Zoom
    m_zoomFactor = 100;
    m_tools_medium->Check();

    if (!m_browser->CanSetZoomType(wxWEBVIEW_ZOOM_TYPE_LAYOUT)) {
        m_tools_layout->Enable(false);
    }

    // Connect the toolbar events
    Bind(wxEVT_TOOL, &WebFrame::OnBack, this, m_toolbar_back->GetId());
    Bind(wxEVT_TOOL, &WebFrame::OnForward, this, m_toolbar_forward->GetId());
    Bind(wxEVT_TOOL, &WebFrame::OnStop, this, m_toolbar_stop->GetId());
    Bind(wxEVT_TOOL, &WebFrame::OnReload, this, m_toolbar_reload->GetId());
    Bind(wxEVT_TOOL, &WebFrame::OnToolsClicked, this, m_toolbar_tools->GetId());

    Bind(wxEVT_TEXT_ENTER, &WebFrame::OnUrl, this, m_url->GetId());

    // Connect the webview events
    Bind(wxEVT_WEBVIEW_NAVIGATING, &WebFrame::OnNavigationRequest, this, m_browser->GetId());
    Bind(wxEVT_WEBVIEW_NAVIGATED, &WebFrame::OnNavigationComplete, this, m_browser->GetId());
    Bind(wxEVT_WEBVIEW_LOADED, &WebFrame::OnDocumentLoaded, this, m_browser->GetId());
    Bind(wxEVT_WEBVIEW_ERROR, &WebFrame::OnError, this, m_browser->GetId());
    Bind(wxEVT_WEBVIEW_NEWWINDOW, &WebFrame::OnNewWindow, this, m_browser->GetId());
    Bind(wxEVT_WEBVIEW_TITLE_CHANGED, &WebFrame::OnTitleChanged, this, m_browser->GetId());

    // Connect the menu events
    Bind(wxEVT_MENU, &WebFrame::OnSetPage, this, setPage->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnViewSourceRequest, this, viewSource->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnViewTextRequest, this, viewText->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnPrint, this, print->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnZoomLayout, this, m_tools_layout->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnSetZoom, this, m_tools_tiny->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnSetZoom, this, m_tools_small->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnSetZoom, this, m_tools_medium->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnSetZoom, this, m_tools_large->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnSetZoom, this, m_tools_largest->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnSetZoom, this, m_tools_custom->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnClearHistory, this, clearhist->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnEnableHistory, this, m_tools_enable_history->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnCut, this, m_edit_cut->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnCopy, this, m_edit_copy->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnPaste, this, m_edit_paste->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnUndo, this, m_edit_undo->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnRedo, this, m_edit_redo->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnMode, this, m_edit_mode->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnScrollLineUp, this, m_scroll_line_up->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnScrollLineDown, this, m_scroll_line_down->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnScrollPageUp, this, m_scroll_page_up->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnScrollPageDown, this, m_scroll_page_down->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnRunScriptString, this, m_script_string->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnRunScriptInteger, this, m_script_integer->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnRunScriptDouble, this, m_script_double->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnRunScriptBool, this, m_script_bool->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnRunScriptObject, this, m_script_object->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnRunScriptArray, this, m_script_array->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnRunScriptDOM, this, m_script_dom->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnRunScriptUndefined, this, m_script_undefined->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnRunScriptNull, this, m_script_null->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnRunScriptDate, this, m_script_date->GetId());

    // Install message handler with the name wx_msg
    m_browser->AddScriptMessageHandler("wx_msg");
    // Bind handler
    m_browser->Bind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED, [&](wxWebViewEvent& evt) {
        wxString s = evt.GetString();
        wxLogMessage(s);
        std::string delimiter = ": ";
        size_t pos = 0;
        std::string function_token;
        std::string id_token;

        pos = s.find(delimiter);
        function_token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
        //wxLogMessage("%s", function_token);

        pos = s.find(delimiter);
        id_token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
        //wxLogMessage("%s", id_token);
 
        pos = s.find(delimiter);
        //wxLogMessage(s);

        if (function_token == "Create") {
            //wxLogMessage("%d", stoi(id_token));
            StorePolygon(stoi(id_token), s);
        }
        else if (function_token == "Update") {
            RemovePolygon(stoi(id_token), s);
            StorePolygon(stoi(id_token), s);
        }
        else
        {
            RemovePolygon(stoi(id_token), s);
        }
        //wxLogMessage("%d", static_cast<int>(polygons.size()));
        });


#if wxUSE_WEBVIEW_IE
    if (!wxWebView::IsBackendAvailable(wxWebViewBackendEdge)) {
        Bind(wxEVT_MENU, &WebFrame::OnRunScriptObjectWithEmulationLevel, this, m_script_object_el->GetId());
        Bind(wxEVT_MENU, &WebFrame::OnRunScriptDateWithEmulationLevel, this, m_script_date_el->GetId());
        Bind(wxEVT_MENU, &WebFrame::OnRunScriptArrayWithEmulationLevel, this, m_script_array_el->GetId());
    }
#endif
    Bind(wxEVT_MENU, &WebFrame::OnRunScriptCustom, this, m_script_custom->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnClearSelection, this, m_selection_clear->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnDeleteSelection, this, m_selection_delete->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnSelectAll, this, selectall->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnLoadScheme, this, loadscheme->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnEnableContextMenu, this, m_context_menu->GetId());
    Bind(wxEVT_MENU, &WebFrame::OnEnableDevTools, this, m_dev_tools->GetId());

    //Connect the idle events
    Bind(wxEVT_IDLE, &WebFrame::OnIdle, this);
}

WebFrame::~WebFrame()
{
    delete m_tools_menu;
}



void WebFrame::RemovePolygon(int id, wxString newpolygon)
{
    for (int i = 0; i < polygons.size(); i++) {
        //wxLogMessage("disaniof %d %d", id, polygons.at(i).GetLeafletID());
        if (polygons.at(i).GetLeafletId() == id) {
            polygons.erase(polygons.begin()+i);
            break;
        }
    }
}

void WebFrame::StorePolygon(int id, wxString newpolygon)
{

    std::string string = newpolygon.ToStdString();
    boost::erase_all(string, "LatLng(");
    boost::erase_all(string, " ");
    boost::erase_all(string, ")");
    //wxLogMessage("%s", string);
    
    std::string delimiter = ",";

    bool b = false;
    size_t pos = 0;
    std::string token;
    std::vector<wxMapPoint> points;
    while ((pos = string.find(delimiter)) != std::string::npos) {
        if (b) {
            float lat = ::atof(token.c_str());
            b = false;
            token = string.substr(0, pos);
            float lng = ::atof(token.c_str());
            wxMapPoint* point = new wxMapPoint(lat, lng);
            wxMapPoint notpointer = *point;
            points.push_back(notpointer);
        }
        else {
            b = true;
            token = string.substr(0, pos);

        }
        string.erase(0, pos + delimiter.length());
    }
    wxMapPolygon* polygon = new wxMapPolygon(points);
    polygon->SetLeafletId(id);
    polygons.push_back(*polygon);
}

wxMenu* WebFrame::CreateMapMenu()
{
    // Add map menu
    wxMenu* map_menu = new wxMenu;
    wxMenuItem* pMenuItem;
    pMenuItem = map_menu->AppendCheckItem(wxID_ANY, "Toggle draggable marker", _("Make the marker draggable (set before 'Add marker...')"));
    Bind(wxEVT_MENU, &WebFrame::OnToggleDraggable, this, pMenuItem->GetId());
    Bind(wxEVT_UPDATE_UI, &WebFrame::OnUpdateDraggable, this, pMenuItem->GetId());

    pMenuItem = map_menu->Append(wxID_ANY, "Add marker...", _("Adds a marker to the map"));
    Bind(wxEVT_MENU, &WebFrame::OnAddMarker, this, pMenuItem->GetId());

    pMenuItem = map_menu->Append(wxID_ANY, "Add polygons...", _("Show polygons from file"));
    Bind(wxEVT_MENU, &WebFrame::OnAddPolygons, this, pMenuItem->GetId());

    pMenuItem = map_menu->Append(wxID_ANY, "Add images...", _("Show images from file"));
    Bind(wxEVT_MENU, &WebFrame::OnAddImages, this, pMenuItem->GetId());
    map_menu->AppendSeparator();

    pMenuItem = map_menu->Append(wxID_ANY, _("Remove last map object"));
    Bind(wxEVT_MENU, &WebFrame::OnRemoveLastMarker, this, pMenuItem->GetId());
    Bind(wxEVT_UPDATE_UI, &WebFrame::OnUpdateRemoveLastMarker, this, pMenuItem->GetId());

    return map_menu;
}

wxWebMap* WebFrame::GetWebMap()
{
    return m_webmap;
}

/**
  * Method that retrieves the current state from the web control and updates the GUI
  * the reflect this current state.
  */
void WebFrame::UpdateState()
{
    m_toolbar->EnableTool(m_toolbar_back->GetId(), m_browser->CanGoBack());
    m_toolbar->EnableTool(m_toolbar_forward->GetId(), m_browser->CanGoForward());

    if (m_browser->IsBusy()) {
        m_toolbar->EnableTool(m_toolbar_stop->GetId(), true);
    } else {
        m_toolbar->EnableTool(m_toolbar_stop->GetId(), false);
    }

    SetTitle(m_browser->GetCurrentTitle());
    m_url->SetValue(m_browser->GetCurrentURL());
}

void WebFrame::OnIdle(wxIdleEvent& WXUNUSED(evt))
{
    if (m_browser->IsBusy()) {
        wxSetCursor(wxCURSOR_ARROWWAIT);
        m_toolbar->EnableTool(m_toolbar_stop->GetId(), true);
    } else {
        wxSetCursor(wxNullCursor);
        m_toolbar->EnableTool(m_toolbar_stop->GetId(), false);
    }
}

/**
  * Callback invoked when user entered an URL and pressed enter
  */
void WebFrame::OnUrl(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->LoadURL(m_url->GetValue());
    m_browser->SetFocus();
    UpdateState();
}

/**
    * Callback invoked when user pressed the "back" button
    */
void WebFrame::OnBack(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->GoBack();
    UpdateState();
}

/**
  * Callback invoked when user pressed the "forward" button
  */
void WebFrame::OnForward(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->GoForward();
    UpdateState();
}

/**
  * Callback invoked when user pressed the "stop" button
  */
void WebFrame::OnStop(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->Stop();
    UpdateState();
}

/**
  * Callback invoked when user pressed the "reload" button
  */
void WebFrame::OnReload(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->Reload();
    UpdateState();
}

void WebFrame::OnClearHistory(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->ClearHistory();
    UpdateState();
}

void WebFrame::OnEnableHistory(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->EnableHistory(m_tools_enable_history->IsChecked());
    UpdateState();
}

void WebFrame::OnCut(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->Cut();
}

void WebFrame::OnCopy(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->Copy();
}

void WebFrame::OnPaste(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->Paste();
}

void WebFrame::OnUndo(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->Undo();
}

void WebFrame::OnRedo(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->Redo();
}

void WebFrame::OnMode(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->SetEditable(m_edit_mode->IsChecked());
}

void WebFrame::OnLoadScheme(wxCommandEvent& WXUNUSED(evt))
{
    wxPathList pathlist;
    pathlist.Add(".");
    pathlist.Add("..");
    pathlist.Add("../help");
    pathlist.Add("../../../samples/help");

    wxFileName helpfile(pathlist.FindValidPath("doc.zip"));
    helpfile.MakeAbsolute();
    wxString path = helpfile.GetFullPath();
    //Under MSW we need to flip the slashes
    path.Replace("\\", "/");
    path = "wxfs:///" + path + ";protocol=zip/doc.htm";
    m_browser->LoadURL(path);
}

void WebFrame::OnEnableContextMenu(wxCommandEvent& evt)
{
    m_browser->EnableContextMenu(evt.IsChecked());
}

void WebFrame::OnEnableDevTools(wxCommandEvent& evt)
{
    m_browser->EnableAccessToDevTools(evt.IsChecked());
}

/**
  * Callback invoked when there is a request to load a new page (for instance
  * when the user clicks a link)
  */
void WebFrame::OnNavigationRequest(wxWebViewEvent& evt)
{
    if (m_info->IsShown()) {
        m_info->Dismiss();
    }

    wxLogMessage("%s", "Navigation request to '" + evt.GetURL() + "' (target='" +
                 evt.GetTarget() + "')");

    wxASSERT(m_browser->IsBusy());

    //If we don't want to handle navigation then veto the event and navigation
    //will not take place, we also need to stop the loading animation
    if (!m_tools_handle_navigation->IsChecked()) {
        evt.Veto();
        m_toolbar->EnableTool(m_toolbar_stop->GetId(), false);
    } else {
        UpdateState();
    }
}

/**
  * Callback invoked when a navigation request was accepted
  */
void WebFrame::OnNavigationComplete(wxWebViewEvent& evt)
{
    wxLogMessage("%s", "Navigation complete; url='" + evt.GetURL() + "'");
    UpdateState();
}

/**
  * Callback invoked when a page is finished loading
  */
void WebFrame::OnDocumentLoaded(wxWebViewEvent& evt)
{
    //Only notify if the document is the main frame, not a subframe
    if (evt.GetURL() == m_browser->GetCurrentURL()) {
        wxLogMessage("%s", "Document loaded; url='" + evt.GetURL() + "'");
    }
    UpdateState();
}

/**
  * On new window, we veto to stop extra windows appearing
  */
void WebFrame::OnNewWindow(wxWebViewEvent& evt)
{
    wxString flag = " (other)";

    if (evt.GetNavigationAction() == wxWEBVIEW_NAV_ACTION_USER) {
        flag = " (user)";
    }

    wxLogMessage("%s", "New window; url='" + evt.GetURL() + "'" + flag);

    //If we handle new window events then just load them in this window as we
    //are a single window browser
    if (m_tools_handle_new_window->IsChecked()) {
        m_browser->LoadURL(evt.GetURL());
    }

    UpdateState();
}

void WebFrame::OnTitleChanged(wxWebViewEvent& evt)
{
    SetTitle(evt.GetString());
    wxLogMessage("%s", "Title changed; title='" + evt.GetString() + "'");
}

void WebFrame::OnSetPage(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->SetPage
    (
        "<html><title>New Page</title>"
        "<body>Created using <tt>SetPage()</tt> method.</body></html>",
        wxString()
    );
}

/**
  * Invoked when user selects the "View Source" menu item
  */
void WebFrame::OnViewSourceRequest(wxCommandEvent& WXUNUSED(evt))
{
    SourceViewDialog dlg(this, m_browser->GetPageSource());
    dlg.ShowModal();
}

/**
 * Invoked when user selects the "View Text" menu item
 */
void WebFrame::OnViewTextRequest(wxCommandEvent& WXUNUSED(evt))
{
    wxDialog textViewDialog(this, wxID_ANY, "Page Text",
                            wxDefaultPosition, wxSize(700, 500),
                            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
#if 0 //wxUSE_STC
    wxStyledTextCtrl* text = new wxStyledTextCtrl(&textViewDialog, wxID_ANY);
    text->SetText(m_browser->GetPageText());
#else // !wxUSE_STC
    wxTextCtrl* text = new wxTextCtrl(this, wxID_ANY, m_browser->GetPageText(),
                                      wxDefaultPosition, wxDefaultSize,
                                      wxTE_MULTILINE |
                                      wxTE_RICH |
                                      wxTE_READONLY);
#endif // wxUSE_STC/!wxUSE_STC
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text, 1, wxEXPAND);
    SetSizer(sizer);
    textViewDialog.ShowModal();
}

/**
  * Invoked when user selects the "Menu" item
  */
void WebFrame::OnToolsClicked(wxCommandEvent& WXUNUSED(evt))
{
    if (m_browser->GetCurrentURL() == "") {
        return;
    }

    m_edit_cut->Enable(m_browser->CanCut());
    m_edit_copy->Enable(m_browser->CanCopy());
    m_edit_paste->Enable(m_browser->CanPaste());

    m_edit_undo->Enable(m_browser->CanUndo());
    m_edit_redo->Enable(m_browser->CanRedo());

    m_selection_clear->Enable(m_browser->HasSelection());
    m_selection_delete->Enable(m_browser->HasSelection());

    m_context_menu->Check(m_browser->IsContextMenuEnabled());
    m_dev_tools->Check(m_browser->IsAccessToDevToolsEnabled());

    //Firstly we clear the existing menu items, then we add the current ones
    wxMenuHistoryMap::const_iterator it;
    for (it = m_histMenuItems.begin(); it != m_histMenuItems.end(); ++it) {
        m_tools_history_menu->Destroy(it->first);
    }
    m_histMenuItems.clear();

    wxVector<wxSharedPtr<wxWebViewHistoryItem> > back = m_browser->GetBackwardHistory();
    wxVector<wxSharedPtr<wxWebViewHistoryItem> > forward = m_browser->GetForwardHistory();

    wxMenuItem* item;

    unsigned int i;
    for (i = 0; i < back.size(); i++) {
        item = m_tools_history_menu->AppendRadioItem(wxID_ANY, back[i]->GetTitle());
        m_histMenuItems[item->GetId()] = back[i];
        Bind(wxEVT_MENU, &WebFrame::OnHistory, this, item->GetId());
    }

    wxString title = m_browser->GetCurrentTitle();
    if (title.empty()) {
        title = "(untitled)";
    }
    item = m_tools_history_menu->AppendRadioItem(wxID_ANY, title);
    item->Check();

    //No need to connect the current item
    m_histMenuItems[item->GetId()] = wxSharedPtr<wxWebViewHistoryItem>(new wxWebViewHistoryItem(m_browser->GetCurrentURL(), m_browser->GetCurrentTitle()));

    for (i = 0; i < forward.size(); i++) {
        item = m_tools_history_menu->AppendRadioItem(wxID_ANY, forward[i]->GetTitle());
        m_histMenuItems[item->GetId()] = forward[i];
        Bind(wxEVT_TOOL, &WebFrame::OnHistory, this, item->GetId());
    }

    wxPoint position = ScreenToClient(wxGetMousePosition());
    PopupMenu(m_tools_menu, position.x, position.y);
}

/**
  * Invoked when user selects the zoom size in the menu
  */
void WebFrame::OnSetZoom(wxCommandEvent& evt)
{
    if (evt.GetId() == m_tools_tiny->GetId()) {
        m_browser->SetZoom(wxWEBVIEW_ZOOM_TINY);
    } else if (evt.GetId() == m_tools_small->GetId()) {
        m_browser->SetZoom(wxWEBVIEW_ZOOM_SMALL);
    } else if (evt.GetId() == m_tools_medium->GetId()) {
        m_browser->SetZoom(wxWEBVIEW_ZOOM_MEDIUM);
    } else if (evt.GetId() == m_tools_large->GetId()) {
        m_browser->SetZoom(wxWEBVIEW_ZOOM_LARGE);
    } else if (evt.GetId() == m_tools_largest->GetId()) {
        m_browser->SetZoom(wxWEBVIEW_ZOOM_LARGEST);
    } else if (evt.GetId() == m_tools_custom->GetId()) {
        OnZoomCustom(evt);
    } else {
        wxFAIL;
    }
}

void WebFrame::OnZoomLayout(wxCommandEvent& WXUNUSED(evt))
{
    if (m_tools_layout->IsChecked()) {
        m_browser->SetZoomType(wxWEBVIEW_ZOOM_TYPE_LAYOUT);
    } else {
        m_browser->SetZoomType(wxWEBVIEW_ZOOM_TYPE_TEXT);
    }
}

void WebFrame::OnZoomCustom(wxCommandEvent& WXUNUSED(evt))
{
    wxNumberEntryDialog dialog
    (
        this,
        "Enter zoom factor as a percentage (10-10000)%",
        "Zoom Factor:",
        "Change Zoom Factor",
        m_zoomFactor,
        10, 10000
    );
    if (dialog.ShowModal() != wxID_OK) {
        return;
    }

    m_zoomFactor = dialog.GetValue();
    m_browser->SetZoomFactor((float)m_zoomFactor / 100);
}

void WebFrame::OnHistory(wxCommandEvent& evt)
{
    m_browser->LoadHistoryItem(m_histMenuItems[evt.GetId()]);
}

void WebFrame::RunScript(const wxString& javascript)
{
    // Remember the script we run in any case, so the next time the user opens
    // the "Run Script" dialog box, it is shown there for convenient updating.
    m_javascript = javascript;

    wxLogMessage("Running JavaScript:\n%s\n", javascript);

    wxString result;
    if (m_browser->RunScript(javascript, &result)) {
        wxLogMessage("RunScript() returned \"%s\"", result);
    } else {
        wxLogWarning("RunScript() failed");
    }
}

void WebFrame::OnRemoveLastMarker(wxCommandEvent& e)
{
    if (!m_webmap->GetMapObjects().size()) {
        return;
    }
    std::list<pwxMapObject>::iterator it = m_webmap->GetMapObjects().end();
    --it;
    m_webmap->DeleteMapObject(*it);
}

void WebFrame::OnAddMarker(wxCommandEvent& e)
{
    wxString sLatLon = wxGetTextFromUser(_("Enter latitude longitude"), _("Add marker"), _("59.326180, 18.072263"), this);
    if (sLatLon.IsEmpty()) {
        return;
    }
    double lat, lon;
    if (!sLatLon.BeforeFirst(',').ToDouble(&lat)) {
        return;
    }
    if (!sLatLon.AfterFirst(',').ToDouble(&lon)) {
        return;
    }

    pwxMapMarker marker = wxMapMarker::Create(lat, lon, cbDraggable);
    wxString res;
    m_webmap->AddMapObject(marker, &res);
    wxLogMessage(_("Added leaflet object %s"), res); // Todo, save the leaflet id, so the marker can be referenced later
}


void WebFrame::OnAddPolygons(wxCommandEvent& WXUNUSED(e))
{
    wxFileName fn;
    fn.SetPath(wxFileName::GetCwd());
    wxString filename = wxFileSelector(_("Select polygon file"), fn.GetPath(), wxEmptyString, wxEmptyString, wxString("Footprint file (*.ifp)|*.ifp"));
    if (filename.empty()) {
        return;
    }
    if (!AddPolygons(filename)) {
        wxLogError(_("Could not add polygons to map"));
        return;
    }
}

bool WebFrame::AddPolygons(wxString const &filename)
{
    std::vector<std::vector<wxMapPoint>> vPolygons;
    std::vector<wxString> vPolygonMetaData;
    PolygonReader reader(filename, vPolygons, vPolygonMetaData);
    if (!vPolygons.size()) {
        wxLogError(_("Could not read polygons from %s"), filename);
        return false;
    }
    wxLogStatus(_("%d polygons read from %s"), (int)vPolygons.size(), filename);
    if (!AddPolygons(vPolygons, vPolygonMetaData)) {
        return false;
    }
    return true;
}

bool WebFrame::AddPolygons(std::vector<std::vector<wxMapPoint>> const& vPolygons, std::vector<wxString> const &vPolygonName)
{
    pwxMapPolygon pPolygon;
    wxString result;
    long opacity = 50;
    double weight = 0.5;
    opacity = ::wxGetNumberFromUser(_("Opacity"), _("Percent 0-100"), _("Polygon display"), opacity, 0L, 100L, this);
    float fOpacity = opacity / 100.0f;
    wxString sWeight = ::wxGetTextFromUser(_("Line width"), _("Polygon display"), wxString::Format("%f", weight), this);
    sWeight.ToDouble(&weight);
    wxColourDialog* dlg = new wxColourDialog(this);
    wxColour col("BLUE");
    if (dlg->ShowModal() == wxID_OK) {
        wxColourData colData = dlg->GetColourData();
        col = colData.GetColour();
    }
    dlg->Destroy();
    for (int i = 0; i < vPolygons.size(); ++i) {
        std::vector<wxMapPoint> const& aPolygon = vPolygons[i];
        // TODO: Create a wxMapPolygon instance and assign polygon
        pPolygon = wxMapPolygon::Create(aPolygon, fOpacity,(float)weight, col.GetAsString(wxC2S_HTML_SYNTAX));
        m_webmap->AddMapObject(pPolygon, &result);
        wxLogMessage(_("Added polygon object %s with result %s"), vPolygonName[i], result);

        for (int j = 0; j < aPolygon.size(); ++j) {
            wxLogMessage(_("lat=%.8f, lon=%.8f"), aPolygon[j].x, aPolygon[j].y);
        }
    }
    return true;
}


void WebFrame::OnAddImages(wxCommandEvent& WXUNUSED(e))
{
    wxFileName fn;
    fn.SetPath(wxFileName::GetCwd());
    wxString filename = wxFileSelector(_("Select image txt file"), fn.GetPath(), wxEmptyString, wxEmptyString, wxString("Footprint file (*.ifp)|*.ifp"));
    if (filename.empty()) {
        return;
    }
    std::vector<std::pair<wxMapPoint, wxMapPoint>> vPoints;
    std::vector<wxString> vPaths;
    ImageReader reader(filename, vPoints, vPaths);
    if (!vPoints.size()) {
        wxLogError(_("Could not read images from %s"), filename);
        return;
    }
    wxLogStatus(_("%d images read from %s"), (int)vPoints.size(), filename);
    if (!AddImages(vPoints, vPaths)) {
        wxLogError(_("Could not add polygons to map"));
    }
}

bool WebFrame::AddImages(std::vector<std::pair<wxMapPoint, wxMapPoint>> const& vPoints, std::vector<wxString> const& vPaths)
{
    for (int i = 0; i < vPoints.size(); i++) {
        wxMapPoint upperLeft = vPoints[i].first;
        wxMapPoint lowerRight = vPoints[i].second;
        wxString tFilePath = vPaths[i];

        wxString& filePath = tFilePath;
        filePath.Replace('\\', '/', true);

        pwxMapImage image = wxMapImage::Create(upperLeft[0], upperLeft[1], lowerRight[0], lowerRight[1], filePath);
        wxString res;
        m_webmap->AddMapObject(image, &res);
        wxLogMessage(image->GetJavaScriptAdd(""));
        wxLogMessage(_("- %s"), filePath);
    }
    return true;
}


void WebFrame::OnToggleDraggable(wxCommandEvent& e)
{
    cbDraggable = !cbDraggable;
}

void WebFrame::OnUpdateDraggable(wxUpdateUIEvent& e)
{
    // Mark menu item as checked if draggable is active
    e.Check(cbDraggable);
}

void WebFrame::OnUpdateRemoveLastMarker(wxUpdateUIEvent& e)
{
    // Enable only if there are objects to delete
    e.Enable(m_webmap->GetMapObjects().size() != 0);
}

void WebFrame::OnRunScriptString(wxCommandEvent& WXUNUSED(evt))
{
    RunScript("function f(a){return a;}f('Hello World!');");
}

void WebFrame::OnRunScriptInteger(wxCommandEvent& WXUNUSED(evt))
{
    RunScript("function f(a){return a;}f(123);");
}

void WebFrame::OnRunScriptDouble(wxCommandEvent& WXUNUSED(evt))
{
    RunScript("function f(a){return a;}f(2.34);");
}

void WebFrame::OnRunScriptBool(wxCommandEvent& WXUNUSED(evt))
{
    RunScript("function f(a){return a;}f(false);");
}

void WebFrame::OnRunScriptObject(wxCommandEvent& WXUNUSED(evt))
{
    RunScript("function f(){var person = new Object();person.name = 'Foo'; \
        person.lastName = 'Bar';return person;}f();");
}

void WebFrame::OnRunScriptArray(wxCommandEvent& WXUNUSED(evt))
{
    RunScript("function f(){ return [\"foo\", \"bar\"]; }f();");
}

void WebFrame::OnRunScriptDOM(wxCommandEvent& WXUNUSED(evt))
{
    RunScript("document.write(\"Hello World!\");");
}

void WebFrame::OnRunScriptUndefined(wxCommandEvent& WXUNUSED(evt))
{
    RunScript("function f(){var person = new Object();}f();");
}


void WebFrame::OnRunScriptNull(wxCommandEvent& WXUNUSED(evt))
{
    // Install message handler with the name wx_msg
    m_browser->AddScriptMessageHandler("wx_msg");
    // Bind handler
    m_browser->Bind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED, [](wxWebViewEvent& evt) {
        wxLogMessage("Script message received; value = %s, handler = %s", evt.GetString(), evt.GetMessageHandler());
        });
    RunScript("function f(){setTimeout(() => {window.wx_msg.postMessage('WHOA IS THIS EVEN POSSIBLE');},3000);} f();");
}

void WebFrame::OnRunScriptDate(wxCommandEvent& WXUNUSED(evt))
{
    RunScript("function resolveAfter2Seconds() {return new Promise(resolve => {setTimeout(() => {resolve('resolved');}, 2000)});}async function asyncCall() {document.write('CALLING'); const result = await resolveAfter2Seconds();window.wx_msg.postMessage(result);}asyncCall();");
}

//void WebFrame::OnRunScriptNull(wxCommandEvent& WXUNUSED(evt))
//{
//    RunScript("function f(){return null;}f();");
//}
//
//void WebFrame::OnRunScriptDate(wxCommandEvent& WXUNUSED(evt))
//{
//    RunScript("function f(){var d = new Date('10/08/2017 21:30:40'); \
//        var tzoffset = d.getTimezoneOffset() * 60000; \
//        return new Date(d.getTime() - tzoffset);}f();");
//}

#if wxUSE_WEBVIEW_IE
void WebFrame::OnRunScriptObjectWithEmulationLevel(wxCommandEvent& WXUNUSED(evt))
{
    wxWebViewIE::MSWSetModernEmulationLevel();
    RunScript("function f(){var person = new Object();person.name = 'Foo'; \
        person.lastName = 'Bar';return person;}f();");
    wxWebViewIE::MSWSetModernEmulationLevel(false);
}

void WebFrame::OnRunScriptDateWithEmulationLevel(wxCommandEvent& WXUNUSED(evt))
{
    wxWebViewIE::MSWSetModernEmulationLevel();
    RunScript("function f(){var d = new Date('10/08/2017 21:30:40'); \
        var tzoffset = d.getTimezoneOffset() * 60000; return \
        new Date(d.getTime() - tzoffset);}f();");
    wxWebViewIE::MSWSetModernEmulationLevel(false);
}

void WebFrame::OnRunScriptArrayWithEmulationLevel(wxCommandEvent& WXUNUSED(evt))
{
    wxWebViewIE::MSWSetModernEmulationLevel();
    RunScript("function f(){ return [\"foo\", \"bar\"]; }f();");
    wxWebViewIE::MSWSetModernEmulationLevel(false);
}
#endif

void WebFrame::OnRunScriptCustom(wxCommandEvent& WXUNUSED(evt))
{
    wxTextEntryDialog dialog
    (
        this,
        "Please enter JavaScript code to execute",
        wxGetTextFromUserPromptStr,
        m_javascript,
        wxOK | wxCANCEL | wxCENTRE | wxTE_MULTILINE
    );
    if (dialog.ShowModal() != wxID_OK) {
        return;
    }

    RunScript(dialog.GetValue());
}

void WebFrame::OnClearSelection(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->ClearSelection();
}

void WebFrame::OnDeleteSelection(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->DeleteSelection();
}

void WebFrame::OnSelectAll(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->SelectAll();
}

void WebFrame::OnError(wxWebViewEvent& evt)
{
#define WX_ERROR_CASE(type) \
    case type: \
        category = #type; \
        break;

    wxString category;
    switch (evt.GetInt()) {
        WX_ERROR_CASE(wxWEBVIEW_NAV_ERR_CONNECTION);
        WX_ERROR_CASE(wxWEBVIEW_NAV_ERR_CERTIFICATE);
        WX_ERROR_CASE(wxWEBVIEW_NAV_ERR_AUTH);
        WX_ERROR_CASE(wxWEBVIEW_NAV_ERR_SECURITY);
        WX_ERROR_CASE(wxWEBVIEW_NAV_ERR_NOT_FOUND);
        WX_ERROR_CASE(wxWEBVIEW_NAV_ERR_REQUEST);
        WX_ERROR_CASE(wxWEBVIEW_NAV_ERR_USER_CANCELLED);
        WX_ERROR_CASE(wxWEBVIEW_NAV_ERR_OTHER);
    }

    wxLogMessage("%s", "Error; url='" + evt.GetURL() + "', error='" + category + " (" + evt.GetString() + ")'");

    //Show the info bar with an error
    m_info->ShowMessage(_("An error occurred loading ") + evt.GetURL() + "\n" + "'" + category + "'", wxICON_ERROR);

    UpdateState();
}

/**
  * Invoked when user selects "Print" from the menu
  */
void WebFrame::OnPrint(wxCommandEvent& WXUNUSED(evt))
{
    m_browser->Print();
}

void WebFrame::OnScrollLineUp(wxCommandEvent&)
{
    m_browser->LineUp();
}
void WebFrame::OnScrollLineDown(wxCommandEvent&)
{
    m_browser->LineDown();
}
void WebFrame::OnScrollPageUp(wxCommandEvent&)
{
    m_browser->PageUp();
}
void WebFrame::OnScrollPageDown(wxCommandEvent&)
{
    m_browser->PageDown();
}
