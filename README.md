# wxWebMap
A C++ library enabling web maps in desktop applications (Windows, Linux, MacOS). 
A wxWebMap is a wxWindow that can be added to your wxWidgets application to show maps, add map objects and edit map objects.

## wxWidgets
The library is based on [```wxWidgets```](http://www.wxwidgets.org) and the web map is displayed using a [```wxWebView```](https://docs.wxwidgets.org/3.0/classwx_web_view.html) window.

## leafletjs
The map functionality is implemented using [```leafletjs```](https://leafletjs.com/). ```Leafletjs javascript``` snippets to e.g. add a marker to a map are executed by calling e.g. ```wxWebMap::AddMapObject```. 

## Sample application
In examples/wxWebMapApplication there is a sample application with GUI. It is based on the wxWidgets wxWebView sample and opens a map in the web view. 
To demonstrate the C++ binding to the map, you can add markers, polygons or images using ```Menu->Map->Add marker|polygons|images...```.

![wxWebMapApp](wxWebMapApplication.jpg "wxWebMapApplication")

## Build instructions

### Install required packages

1. Install *vcpkg* by following these instructions: https://vcpkg.io/en/getting-started.html
1. Set environment variable CMAKE_TOOLCHAIN_FILE to **[path to vcpkg]/scripts/buildsystems/vcpkg.cmake**
1. Install wxWidgets
    ```
    Windows: $ .\vcpkg.exe install wxwidgets:x64-windows
    Linux:   $ .\vcpkg.exe install wxwidgets:x64-linux
    ```
1. Clone the wxWebMap repository from https://github.com/I-CONIC-Vision-AB/wxWebMap

### Build with Visual Studio

Open the wxWebMap folder in Visual Studio. CMake configuration should start automatically. Choose your preset (debug/release) and build.

### Build with CMake command

```
$ cmake -S . --preset=[preset name]
$ cmake --build out/build/[preset name]
```

Available presets are:

- linux-x64-debug
- linux-x64-release
- windows-x64-debug
- windows-x64-release

### Known issues

If you run into problems when compiling wxWidgets or even if compilation works but you get messages like "wx*.dll not found" when running sample applications. Replace the ```./vcpkg/ports/wxwidgets/portfile.cmake``` with the one in ```wxWebMap/cmake/portfile.cmake``` and rebuild wxWidgets (```vcpkg install wxwidgets:x64-windows```).