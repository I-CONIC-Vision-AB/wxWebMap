#include    <wxWebMapApp.h>
#include    <wxWebMapFrame.h>

#include    <wx/filename.h>
#include    <wx/filesys.h>
#include    <wx/fs_arc.h>
#include    <wx/fs_mem.h>

using namespace iconic;

wxIMPLEMENT_APP(wxWebMapApp);

wxWebMapApp::wxWebMapApp()
{
    wxFileName fn;
    fn.SetPath(wxFileName::GetCwd());
    fn.SetFullName(wxString("map.html"));
    m_url = fn.GetFullPath();
    if (!wxFileName::Exists(m_url)) {
        m_url = wxString("https://www.wxwidgets.org");
    }
    cpMap = MapHtml::Create(m_url);
    cpMap->SetCenter(18.072263, 59.326180);
}

void wxWebMapApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    wxApp::OnInitCmdLine(parser);

    parser.AddParam("URL to open",
                    wxCMD_LINE_VAL_STRING,
                    wxCMD_LINE_PARAM_OPTIONAL);
}

bool wxWebMapApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    if (!wxApp::OnCmdLineParsed(parser)) {
        return false;
    }

    if (parser.GetParamCount()) {
        m_url = parser.GetParam(0);
    }

    return true;
}

bool wxWebMapApp::OnInit()
{
    if ( !wxApp::OnInit() ) {
        return false;
    }

    //Required for virtual file system archive and memory support
    wxFileSystem::AddHandler(new wxArchiveFSHandler);
    wxFileSystem::AddHandler(new wxMemoryFSHandler);

    // Create the memory files
    wxImage::AddHandler(new wxPNGHandler);
    wxMemoryFSHandler::AddFile("logo.png",
                               wxBitmap(wxlogo_xpm), wxBITMAP_TYPE_PNG);
    wxMemoryFSHandler::AddFile("page1.htm",
                               "<html><head><title>File System Example</title>"
                               "<link rel='stylesheet' type='text/css' href='memory:test.css'>"
                               "</head><body><h1>Page 1</h1>"
                               "<p><img src='memory:logo.png'></p>"
                               "<p>Some text about <a href='memory:page2.htm'>Page 2</a>.</p></body>");
    wxMemoryFSHandler::AddFile("page2.htm",
                               "<html><head><title>File System Example</title>"
                               "<link rel='stylesheet' type='text/css' href='memory:test.css'>"
                               "</head><body><h1>Page 2</h1>"
                               "<p><a href='memory:page1.htm'>Page 1</a> was better.</p></body>");
    wxMemoryFSHandler::AddFile("test.css", "h1 {color: red;}");

    WebFrame *frame = new WebFrame(m_url);
    frame->Show();

    return true;
}
