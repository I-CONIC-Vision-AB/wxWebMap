# wxWebMap
A C++ library enabling web maps in desktop applications (Windows, Linux, MacOS). 

## wxWidgets
The library is based on ```wxWidgets``` and the web map is displayed using a ```wxWebView``` window.

## leafletjs
The map functionality is implemented using ```leafletjs```. ```Leafletjs javascript``` snippets to e.g. add a marker to a map are executed by calling e.g. ```wxWebMap::AddMapObject```. 

## Sample application
In examples/wxWebMapApplication there is a sample application with GUI. It is based on the wxWidgets wxWebView sample. However, it opens a map in the web view. 
You can click in the map to add markers. These markers can be dragged on the map. A list with markers is seen to the left and a \c 'remove' link enables deleting a marker.
You can progammatically add a marker from ```menu->Run Script->Put marker on Stockholm```.

![wxWebMapApp](wxWebMapApplication.jpg "wxWebMapApplication")