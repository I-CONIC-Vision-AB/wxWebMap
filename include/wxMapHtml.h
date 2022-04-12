#pragma once

#include	<webmap_exports.h>
#include    <wx/string.h>
#include    <wx/xml/xml.h>
#include    <wx/filename.h>

/**
 * @brief Utility to add e.g. javascripts to an 

 page.
 *
 * This is typically only done once to create an HTML file with desired javascripts so that a wxMapObject can be added and edited.
 * Called internally in wxWebMap::Create using the \c basemapHtmlFileName argument to identify HTML template.
*/
class ICONIC_WEBMAP_EXPORT wxMapHtml : public wxXmlDocument {
public:
    /**
     * @brief Constructor.
     *
     * wxMemoryFileSystem must have been added to file system handlers before call!
     * @param baseMapHtmlFileName HTML template file to edit. This is typically an HTML file in the \c wxWebMap/data directory, e.g. wxWebMap/data/map.html (which is copied to binary directory).
     * @todo Remove more javascript code from data/map.html and add programatically from javscripts instead so that we can use C++ to create a map, change tile layer etcetera.
    */
    wxMapHtml(wxString const& baseMapHtmlFileName, bool bUseMemoryFS = true);

    virtual ~wxMapHtml();

    /**
     * @brief Get the name of the file in the memory file system.
     *
     * This file can be loaded like this:
     * \code
     * wxString memoryFileName = wxString("memory:")+html.GetMemoryFileName();
     * wxWebView* pWebView = pWebMap->GetWebView()->LoadURL(memoryFileName);
     * \endcode
     * @return the name of the file in the memory file system
    */
    wxString GetMemoryFileName() const;

    wxFileName GetLocalFileName() const;
protected:
    /**
     * @brief Find node in XML/HTML tree
     * @param s Name of node
     * @param startNode Node to start search in
     * @param bIncludeChildren If true, children are traversed, otherwise only nodes at the \c startNode level
     * @param attr If not null, the node must have this attribute and with this value
     * @return Found node or null
    */
    wxXmlNode* Find(wxString const& s, wxXmlNode* startNode, bool bIncludeChildren = true, wxXmlAttribute* attr = nullptr);

    /**
     * @brief Get \c "body" node
     * @return \c "body" or null
    */
    wxXmlNode* GetBodyNode();

    /**
     * @brief Add Leaflet javascripts to the HTML file.
     * @todo Provide a directory (data/js) and add all *.js in that directory
    */
    bool AddLeafletJavaScripts();

    wxString cFileName;
    wxString cMemoryFileName;
    bool cbUseMemoryFS;
    wxFileName cOutputFileName;
};