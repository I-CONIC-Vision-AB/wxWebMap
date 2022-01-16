#include	<wxMapHtml.h>
#include    <wx/html/htmlpars.h>
#include    <wx/xml/xml.h>
#include    <wx/log.h>
#include    <wx/FFile.h>
#include    <wx/sstream.h>
#include    <wx/fs_mem.h>

wxMapHtml::wxMapHtml(wxString const& baseMapHtmlFileName) :
    wxXmlDocument(baseMapHtmlFileName),
    cFileName(baseMapHtmlFileName)
{
    if (!IsOk()) {
        wxLogError(_("Could not parse %s"), baseMapHtmlFileName);
        return;
    }

    AddLeafletJavaScripts();
}

wxXmlNode* wxMapHtml::GetBodyNode()
{
    bool bRecurse = false; // html and body are top nodes
    wxXmlNode* htmlNode = Find(wxString("html"), GetRoot(), bRecurse); // html is root
    return Find(wxString("body"), htmlNode->GetChildren(), bRecurse); // body is immediate child of html
}

bool wxMapHtml::AddLeafletJavaScripts()
{
    wxXmlNode* p = GetDocumentNode();
    wxString name = p->GetName();
    wxXmlNode* bodyNode = GetBodyNode();
    if (!bodyNode) {
        wxLogError(_("Could not find <body>"));
        return false;
    }

    // Check if there already is a javascript node
    wxXmlAttribute *attr = new wxXmlAttribute("type","text/javascript");
    wxXmlNode* scriptNode = Find(wxString("script"), bodyNode->GetChildren(), false, attr);
    if (!scriptNode) {
        // No script node so create and add one
        scriptNode = new wxXmlNode(bodyNode, wxXmlNodeType::wxXML_ELEMENT_NODE, wxString("script"), wxEmptyString, attr);
    } else {
        delete attr;
    }

    wxFileName fn(cFileName);
    wxString htmlName = fn.GetFullName();
    fn.AppendDir("js");
    fn.SetFullName("wxMapMarker.js"); // One example, ore to be added
    wxString filename = fn.GetFullPath();
    wxFFile jsFile(filename, "r");
    if (!jsFile.IsOpened()) {
        return false;
    }
    wxString jsCodeString;
    jsFile.ReadAll(&jsCodeString);
    // Although we add a new text node, this text is prepended to the start of the existing text
    wxXmlNode* addMarkerNode = new wxXmlNode(scriptNode, wxXmlNodeType::wxXML_TEXT_NODE, wxString("script"), jsCodeString);

    wxStringOutputStream os;
    Save(os);
    wxString s = os.GetString();
    if (s.StartsWith("<?xml")) {
        s = s.AfterFirst('\n'); // Delete the <?xml> tag - not desired for HTML
    }
    if (s.StartsWith("<!DOC")) {
        s = s.AfterFirst('\n'); // Delete the <!DOCUMENT html> tag - not desired for HTML
    }
    wxMemoryFSHandler::AddFileWithMimeType(htmlName, s, "text/html");

#if 0
    // Save to file for debugging
    Save("C:/Data/test.htm");
    wxFFile f("C:/Data/test2.htm", "w");
    f.Write(s);
    f.Close();
#endif

    cMemoryFileName = htmlName;

    return true;
}

wxString wxMapHtml::GetMemoryFileName() const
{
    return cMemoryFileName;
}

wxXmlNode* wxMapHtml::Find(wxString const& s, wxXmlNode* startNode, bool bIncludeChildren, wxXmlAttribute* attr)
{
    while (startNode) {
        if (startNode->GetName().IsSameAs(s, false)) {
            if (attr && startNode->GetAttribute(attr->GetName()).IsSameAs(attr->GetValue())) {
                return startNode;
            } else {
                return startNode;
            }
        }
        if (bIncludeChildren) {
            wxXmlNode* pChild = Find(s, startNode->GetChildren());
            if (pChild) {
                return pChild;
            }
        }
        startNode = startNode->GetNext();
    }
    return startNode;
}