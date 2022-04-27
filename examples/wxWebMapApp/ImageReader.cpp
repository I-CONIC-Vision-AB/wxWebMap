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
        wxLogError(_("Couldnot open %s"), filename);
        return;
    }

    wxString str;
    int c = 0;
    wxFileName imgPath(filename);
    imgPath.AppendDir("images");
    imgPath.SetExt("jpg");
    std::pair<wxMapPoint, wxMapPoint> corners;

    for (str = file.GetFirstLine(); !file.Eof(); str = file.GetNextLine()) {
        switch (c) {
        case 0:
            imgPath.SetName(str);
            break;
        case 2:
            corners.first = ReadPoint(str);
            break;
        case 4:
            corners.second = ReadPoint(str);
            break;
        case 6:
            imagePoints.push_back(corners);
            imagePaths.push_back(imgPath.GetFullPath());
            break;
        default:
            break;
        }
        c = (c + 1) % 7;
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

