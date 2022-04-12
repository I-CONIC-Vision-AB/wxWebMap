#pragma once

#include    <boost/test/unit_test.hpp>
namespace buf = boost::unit_test::framework;
#include    <wxWebMap.h>
#include    <wxMapUtil.h>
#include	<wx/filename.h>
#include	<wx/utils.h>
#include    <wx/log.h>
#include    <wx/frame.h>
#include    <wx/fs_mem.h>

BOOST_AUTO_TEST_CASE(maprect)
{
    std::cerr << "Running test case: " << buf::current_test_case().p_name << std::endl;
    float x0 = 58.0f;
    float y0 = 18.0f;
    float x1 = 58.001f;
    float y1 = 18.00025f;
    wxMapRect r(x0, y0, x1-x0, y1-y0);

    BOOST_TEST(r[0] == 58.0f);
    BOOST_TEST(r[1] == 18.0f);
    BOOST_TEST(r[2] == x1-x0);
    BOOST_TEST(r[3] == y1-y0);
    BOOST_TEST(r[2] == r.w);
    BOOST_TEST(r[3] == r.h);
}

BOOST_AUTO_TEST_CASE(webmap)
{
    std::cerr << "Running test case: " << buf::current_test_case().p_name << std::endl;
    wxLog::SetActiveTarget(new wxLogStderr);
    wxLog::SetVerbose(true);
    wxFileSystem::AddHandler(new wxMemoryFSHandler);

    wxFrame *f = new wxFrame(nullptr, wxID_ANY, "Dummy frame");

    // Create the webview
    wxString backend = wxWebViewBackendDefault;
    if (wxWebView::IsBackendAvailable(wxWebViewBackendDefault)) {
    }
#ifdef __WXMSW__
    if (wxWebView::IsBackendAvailable(wxWebViewBackendEdge)) {
        wxLogMessage("Using Edge backend");
        backend = wxWebViewBackendEdge;
    } else {
        wxLogMessage("Edge backend not available");
    }
#endif

    wxWebMap* pMap = wxWebMap::Create(f, wxID_ANY, "map.html", wxDefaultPosition, wxDefaultSize, backend);
    BOOST_TEST(pMap);

    f->Destroy();
    wxFileSystem::CleanUpHandlers();
}
