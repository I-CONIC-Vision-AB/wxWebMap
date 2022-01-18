The portfile*.cmake files are only substitues for wxWidgets portfiles in vcpkg.
vcpkg refuses to accept them, but wxWidgets debug dll:s are not handled correctly with the official portfile.
portfile.cmake fixes that
portfile_for_IE_EDGE.cmake is not tested but should compile an Edge backend of webview instead of internet explorer on Windows (provided that webview2 has been installed first!!)