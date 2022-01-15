#include	"detail/MapHtmlImpl.h"
#include    <wx/textfile.h>
#include    <wx/filename.h>
#include    <wx/log.h>
#include    <wx/intl.h>
#include    <wx/ffile.h>

using namespace iconic;

wxString LeafletJs::cDataDirectory = wxEmptyString;

bool MapHtml::SetDataDirectory(wxString const& dir)
{
    if (!wxFileName::DirExists(dir)) {
        wxLogError(_("Data directory could not be set. %s does not exist"), dir);
        return false;
    }
    return LeafletJs::SetDataDirectory(dir);
}

MapHtmlPtr MapHtml::Create(wxString sHtmlBaseMapFileName)
{
    return MapHtmlPtr(new LeafletJs(sHtmlBaseMapFileName));
}

bool LeafletJs::SetDataDirectory(wxString const& dir)
{
    cDataDirectory = dir;
    return true;
}

LeafletJs::LeafletJs(wxString sHtmlBaseMapFileName) :
    cCenterLat(0.0),
    cCenterLon(0.0),
    cNum(0.0),
    cWidth(1200),
    cHeight(800)
{
    if (!sHtmlBaseMapFileName.IsEmpty()) {
        if (wxFileName::Exists(sHtmlBaseMapFileName)) {
            // The string contains a readable file, including a path
            wxFFile file(sHtmlBaseMapFileName, "r");
            if (!file.ReadAll(&cHtmlString)) {
                wxLogError(_("Could not read %s"), sHtmlBaseMapFileName);
            }
        } else if (!cDataDirectory.IsEmpty()) {
            // Assume the string is a full name like map.html, but without path.
            // Try to prepend the data directory
            wxFileName fn;
            fn.SetPath(cDataDirectory);
            fn.SetFullName(sHtmlBaseMapFileName);
            if (fn.Exists()) {
                // Yes, we found the file in the data directory
                wxFFile file(fn.GetFullPath(), "r");
                if (!file.ReadAll(&cHtmlString)) {
                    wxLogError(_("Could not read %s"), fn.GetFullPath());
                }
            }
        }
    }
    SetZoom();
}

LeafletJs::~LeafletJs()
{

}

bool LeafletJs::ClearAllMarkers()
{
    cvMarkers.clear();
    csMarkers = wxEmptyString;
    csCenter = wxEmptyString;
    cCenterLat = cCenterLon = 0.0;
    cNum = 0;
    return true;
}

void LeafletJs::AddMarker(Eigen::Vector3d const& Pt)
{
    cvMarkers.push_back(Pt);
    cCenterLon += Pt[0];
    cCenterLat += Pt[1];
    cNum++;
}

void LeafletJs::AddOverlayImage(std::vector<Eigen::Vector3d> const& Pt, wxString sUrl)
{
    //cvMarkers.push_back(Pt);
    //cCenterLon += Pt[0];
    //cCenterLat += Pt[1];
    //cNum++;
}

void LeafletJs::AddMarker(double longitude, double latitude)
{
    Eigen::Vector3d pt(longitude, latitude, 0.0);
    AddMarker(pt);
}

void LeafletJs::SetCenter(double longitude, double latitude)
{
    cCenterLon = longitude;
    cCenterLat = latitude;
    cNum = 1.0;
}

void LeafletJs::SetZoom(int level)
{
    if (level >= 0 && level < 64) {
        cZoom = level;
    }
}

wxString LeafletJs::GetHtml()
{
    return cHtmlString;

    //csMarkers = wxEmptyString;
    //std::list<Eigen::Vector3d>::const_iterator it;
    //for (it = cvMarkers.cbegin(); it != cvMarkers.cend(); ++it) {
    //    csMarkers += wxString::Format("L.marker([%.6lf,%.6lf]).addTo(map);\n", (*it)[1], (*it)[0]);
    //}
    //if (cNum) {
    //    double center_lat = cCenterLat / cNum;
    //    double center_lon = cCenterLon / cNum;
    //    csCenter = wxString::Format("%.6lf,%.6lf", center_lat, center_lon);
    //} else {
    //    csCenter = "59.326180, 18.072263";
    //}

    //return wxString::Format(cHtmlString, cWidth, cHeight, csCenter, cZoom, csMarkers);
}

bool LeafletJs::Save(wxString const& filename)
{
    if (wxFileName::FileExists(filename)) {
        if (!wxRemoveFile(filename)) {
            wxLogError(_("%s already exists and could not be overwritten"), filename);
            return false;
        }
    }
    wxFFile afile(filename, "w");
    if (!afile.Write(GetHtml())) {
        wxLogError(_("Could not write %s"), filename);
        return false;
    }
    if (!afile.Close()) {
        wxLogError(_("Could not close %s"), filename);
        return false;
    }

    return true;
}

void LeafletJs::SetSize(int width, int height)
{
    cWidth = width;
    cHeight = height;
}

wxString LeafletJs::GetMapName()
{
    return wxString("map");
}
