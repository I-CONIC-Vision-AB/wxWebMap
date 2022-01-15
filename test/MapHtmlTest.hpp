#pragma once

#include    <boost/test/unit_test.hpp>
namespace buf = boost::unit_test::framework;
#include	<MapHtml.h>
#include    <wxWebMap.h>
#include	<wx/filename.h>
#include	<wx/utils.h>
#include    <wx/log.h>
#include    <wx/frame.h>

BOOST_AUTO_TEST_CASE(photo_plan)
{
    std::cerr << "Running test case: " << buf::current_test_case().p_name << std::endl;
    wxLog::SetActiveTarget(new wxLogStderr);
    wxLog::SetVerbose(true);

    wxString dir = wxFileName::GetCwd(); // The install_dir/bin directory
    wxFileName fn;
    fn.SetPath(dir);
    fn.RemoveLastDir(); // The install_dir
    dir = fn.GetPath();
    BOOST_TEST(iconic::sensor::MapHtml::SetDataDirectory(dir));

    iconic::sensor::MapHtmlPtr pMap = iconic::sensor::MapHtml::Create(wxString("map.html"));
    wxString filename("C:/Data/marker.html");
    if (wxFileExists(filename)) {
        wxRemoveFile(filename);
    }

    std::vector<Eigen::Vector2d> vPoints = {
        Eigen::Vector2d(12.443549,57.539421),
        Eigen::Vector2d(12.443545,57.539322),
        Eigen::Vector2d(12.443549,57.539219),
        Eigen::Vector2d(12.443548,57.539112),
        Eigen::Vector2d(12.443551,57.539005),
        Eigen::Vector2d(12.443551,57.538906),
        Eigen::Vector2d(12.443550,57.538799),
        Eigen::Vector2d(12.443550,57.538692),
        Eigen::Vector2d(12.443551,57.538589),
        Eigen::Vector2d(12.443549,57.538486),
        Eigen::Vector2d(12.443548,57.538380),
        Eigen::Vector2d(12.443552,57.538273),
        Eigen::Vector2d(12.443555,57.538170),
        Eigen::Vector2d(12.443462,57.538162),
        Eigen::Vector2d(12.443262,57.538162),
        Eigen::Vector2d(12.443213,57.538189),
        Eigen::Vector2d(12.443215,57.538292),
        Eigen::Vector2d(12.443216,57.538399),
        Eigen::Vector2d(12.443216,57.538502),
        Eigen::Vector2d(12.443212,57.538609),
        Eigen::Vector2d(12.443212,57.538712),
        Eigen::Vector2d(12.443213,57.538818),
        Eigen::Vector2d(12.443218,57.538929),
        Eigen::Vector2d(12.443216,57.539028),
        Eigen::Vector2d(12.443216,57.539135),
        Eigen::Vector2d(12.443219,57.539238),
        Eigen::Vector2d(12.443217,57.539345),
        Eigen::Vector2d(12.443205,57.539425),
        Eigen::Vector2d(12.443073,57.539421),
        Eigen::Vector2d(12.442884,57.539421),
        Eigen::Vector2d(12.442884,57.539364),
        Eigen::Vector2d(12.442888,57.539261),
        Eigen::Vector2d(12.442886,57.539154),
        Eigen::Vector2d(12.442881,57.539051)
    };

    for (int i = 0; i < vPoints.size(); ++i) {
        pMap->AddMarker(vPoints[i][0], vPoints[i][1]);
    }

    // Save leaflet to file
    filename = wxString("C:/Data/marker.html");
    BOOST_TEST(pMap->Save(filename));
    wxLogVerbose(_("Markers saved to %s"), filename);

    // Show in web browser
    filename = wxString("file:///") + filename;
    wxLaunchDefaultBrowser(filename);

    // Cleanup
    pMap = iconic::sensor::MapHtmlPtr();
    delete wxLog::SetActiveTarget(nullptr);
}

BOOST_AUTO_TEST_CASE(webmap)
{
    std::cerr << "Running test case: " << buf::current_test_case().p_name << std::endl;
    wxLog::SetActiveTarget(new wxLogStderr);
    wxLog::SetVerbose(true);
    wxFrame *f = new wxFrame(nullptr, wxID_ANY, "Hej");

    iconic::wxWebMap* pMap = iconic::wxWebMap::Create(f, wxID_ANY);
    BOOST_TEST(pMap);
    f->Destroy();
}

BOOST_AUTO_TEST_CASE(pinpoint_image)
{
    std::cerr << "Running test case: " << buf::current_test_case().p_name << std::endl;
    wxLog::SetActiveTarget(new wxLogStderr);
    wxLog::SetVerbose(true);

    iconic::sensor::MapHtmlPtr pMap = iconic::sensor::MapHtml::Create();

}