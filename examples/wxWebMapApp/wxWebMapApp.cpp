#include    <wxWebMapApp.h>
#include    <wxWebMapFrame.h>

#include    <wx/filename.h>
#include    <wx/filesys.h>
#include    <wx/fs_arc.h>
#include    <wx/fs_mem.h>

wxIMPLEMENT_APP(wxWebMapApp);

wxWebMapApp::wxWebMapApp()
{
    wxFileName fn;
    fn.SetPath(wxFileName::GetCwd());   // Executable directory
    fn.AppendDir("html");               // html folder in executable directory
    fn.SetFullName("map.html");         // Template html file
    m_url = fn.GetFullPath();
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
    WebFrame *frame = new WebFrame(m_url);
    frame->Show();

    return true;
}

int wxWebMapApp::OnExit()
{
    wxFileSystem::CleanUpHandlers();
    wxImage::CleanUpHandlers();
    int val = wxApp::OnExit();
    return val;
}
