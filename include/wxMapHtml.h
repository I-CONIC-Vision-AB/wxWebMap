#pragma once

#include	<exports.h>
#include    <wx/string.h>
#include    <wx/xml/xml.h>

/**
 * @brief Utility to add e.g. javascripts to an HTML page.
*/
class ICONIC_WEBMAP_EXPORT wxMapHtml : public wxXmlDocument {
public:
    /**
     * @brief Constructor.
     *
     * wxMemoryFileSystem must have been added to file system handlers before call!
     * @param baseMapHtmlFileName HTML template file to edit
    */
    wxMapHtml(wxString const& baseMapHtmlFileName);

    /**
     * @brief Get the name of the file in the memory file system
     * @return the name of the file in the memory file system
    */
    wxString GetMemoryFileName() const;
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
     * @brief Get \c <body>
     * @return \c <body> or null
    */
    wxXmlNode* GetBodyNode();

    /**
     * @brief Add Leaflet javascripts to the HTML file.
     * @todo Provide a directory (data/js) and add all *.js in that directory
    */
    bool AddLeafletJavaScripts();

    wxString cFileName;
    wxString cMemoryFileName;
};