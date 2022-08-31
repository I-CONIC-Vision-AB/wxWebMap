# wxWebMap
A C++ library enabling web maps in desktop applications (Windows, Linux, MacOS). 
A wxWebMap is a wxWindow that can be added to your wxWidgets application to show maps, add map objects and edit map objects.

## wxWidgets
The library is based on [```wxWidgets```](http://www.wxwidgets.org) and the web map is displayed using a [```wxWebView```](https://docs.wxwidgets.org/3.0/classwx_web_view.html) window.

## leafletjs
The map functionality is implemented using [```leafletjs```](https://leafletjs.com/). ```Leafletjs javascript``` snippets to e.g. add a marker to a map are executed by calling e.g. ```wxWebMap::AddMapObject```. 

## Sample application
In examples/wxWebMapApplication there is a sample application with GUI. It is based on the wxWidgets wxWebView sample and opens a map in the web view. 
To demonstrate the C++ binding to the map, you can add markers, polygons or images using ```menu->Map->Add marker|polygons|images...```.

![wxWebMapApp](wxWebMapApplication.jpg "wxWebMapApplication")

## Build instructions

### Windows
* Clone the wxWebMap repository at https://github.com/HWiman-ICONIC/wxWebMap
* Install vcpkg by following these instructions: https://vcpkg.io/en/getting-started.html
* Install the wxWidgets and boost libraries: ```.\vcpkg.exe install wxwidgets:x64-windows boost:x64-windows``` in e.g. powershell (Admin mode)
    * If you run into problems when compiling wxWidgets or even if compilation works but you get messages like "wx*.dll not found" when running sample applications. Replace the ```./vcpkg/ports/wxwidgets/portfile.cmake``` with the one in ```wxWebMap/cmake/portfile.cmake``` and rebuild wxWidgets (```vcpkg install wxwidgets:x64-windows```).
    * You only have to build ```boost-smart-ptr``` and ```boost-test```, but you might as well build all of [```boost```](https://www.boost.org/). 
* Open VisualStudio and choose the ```Open a local folder``` option and select your wxWebMap directory.
* This will trigger [CMake](https://cmake.org/) to parse the project. 
    * You may get warnings, but in the Output tab you should see ```CMake generation finished```.
* Now Build->Build all
* Set target to e.g. bin/wxWebMapApp.exe and run the application

### Linux
<vcpkg> is the vcpkg root directory

1. Clone as above
1. Use vcpkg
1. Create configuration:
$ cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake
1. Build
$ cmake --build . --target IconicWebMap --config Debug
