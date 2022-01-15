#include	"../MapHtml.h"
#include    <list>
#include    <vector>

namespace iconic {
namespace sensor {
class LeafletJs : public MapHtml {
public:
    LeafletJs(wxString sHtmlBaseMapFileName);
    virtual ~LeafletJs();

    static bool SetDataDirectory(wxString const& dir);

    virtual bool ClearAllMarkers();
    virtual void AddMarker(Eigen::Vector3d const& Pt);
    virtual void AddMarker(double longitude, double latitude);
    virtual void AddOverlayImage(std::vector<Eigen::Vector3d> const& Pt, wxString sUrl);
    virtual void SetCenter(double longitude, double latitude);
    virtual bool Save(wxString const& filename);
    virtual void SetSize(int width, int height);
    virtual void SetZoom(int level = 14);
    virtual wxString GetHtml();

    static wxString cDataDirectory;
    wxString cHtmlString;
    wxString csMarkers;
    wxString csCenter;
    double cCenterLat, cCenterLon;
    double cNum;
    std::list<Eigen::Vector3d> cvMarkers;
    int cWidth, cHeight;
    int cZoom;
};
}
}