#include	<ImageReader.h>
#include	<wx/textfile.h>
#include    <wx/log.h>
#include    <wx/intl.h>
#include    <wx/tokenzr.h>
#include    <list>
#include    <wx/filename.h>

ImageReader::ImageReader(wxString const& filename, std::vector<std::pair<wxMapPoint, wxMapPoint>>& imagePoints, std::vector<wxString>& imagePaths)
{
    // wxFileName txtPath(filename);
    wxTextFile file(filename);
    if (!file.Open()) {
        wxLogError(_("Could not open %s"), filename);
        return;
    }

    wxString str;
    int c = 0;
    wxFileName imgPath(filename);
    wxString vExt[] = { "png","jpg","tif" };
    std::pair<wxMapPoint, wxMapPoint> corners;
    wxMapPoint pt;

    bool bEof = false;
    for (str = file.GetFirstLine(); !bEof; str = file.GetNextLine()) {
        switch (c) {
        case 0:
            imgPath.SetName(str);
            break;
        case 1:
            corners.first = corners.second = ReadPoint(str);
            break;
        case 2:
        case 3:
        case 4:
            pt = ReadPoint(str);
            if (pt.x < corners.first.x) {
                corners.first.x = pt.x;
            }
            else if (pt.x > corners.second.x) {
                corners.second.x = pt.x;
            }
            if (pt.y < corners.first.y) {
                corners.first.y = pt.y;
            }
            else if (pt.y > corners.second.y) {
                corners.second.y = pt.y;
            }
            break;
        case 6:
        {
            imagePoints.push_back(corners);
            bool bFound = false;
            for (int j = 0; j < 3; ++j) {
                imgPath.SetExt(vExt[j]);
                wxString filename = imgPath.GetFullPath();
                if (imgPath.Exists()) {
                    bFound = true;
                    imagePaths.push_back(imgPath.GetFullPath());
                    break;
                }
            }
            if (!bFound) {
                wxLogWarning(_("The image %s does not exist"), imgPath.GetFullPath());
            }
        }
            break;
        default:
            break;
        }
        c = (c + 1) % 7;
        if (file.Eof()) {
            bEof = true;
            break;
        }
    }
}

wxMapPoint ImageReader::ReadPoint(wxString str)
{
    wxMapPoint pt;
    wxArrayString tokens = wxStringTokenize(str, ",");
    sscanf(tokens[0].char_str(), "%f", &(pt[1])); // First in file is longitude, but we want latitude first
    sscanf(tokens[1].char_str(), "%f", &(pt[0]));
    return pt;
}

