# Install script for directory: C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "c:/libjpeg-turbo64")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/simd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/src/md5/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "lib" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/Debug/turbojpeg-static.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/Release/turbojpeg-static.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/MinSizeRel/turbojpeg-static.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/RelWithDebInfo/turbojpeg-static.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "bin" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM RENAME "tjbench.exe" FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/${CMAKE_INSTALL_CONFIG_NAME}/tjbench-static.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "include" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/src/turbojpeg.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "lib" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/Debug/jpeg-static.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/Release/jpeg-static.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/MinSizeRel/jpeg-static.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/RelWithDebInfo/jpeg-static.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "bin" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM RENAME "cjpeg.exe" FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/${CMAKE_INSTALL_CONFIG_NAME}/cjpeg-static.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "bin" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM RENAME "djpeg.exe" FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/${CMAKE_INSTALL_CONFIG_NAME}/djpeg-static.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "bin" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM RENAME "jpegtran.exe" FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/${CMAKE_INSTALL_CONFIG_NAME}/jpegtran-static.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "bin" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/Debug/rdjpgcom.exe")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/Release/rdjpgcom.exe")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/MinSizeRel/rdjpgcom.exe")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/RelWithDebInfo/rdjpgcom.exe")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "bin" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/Debug/wrjpgcom.exe")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/Release/wrjpgcom.exe")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/MinSizeRel/wrjpgcom.exe")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/RelWithDebInfo/wrjpgcom.exe")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "doc" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/doc" TYPE FILE FILES
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/README.ijg"
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/README.md"
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/src/example.c"
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/src/tjcomp.c"
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/src/tjdecomp.c"
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/src/tjtran.c"
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/doc/libjpeg.txt"
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/doc/structure.txt"
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/doc/usage.txt"
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/doc/wizard.txt"
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/LICENSE.md"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "lib" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/pkgscripts/libjpeg.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "lib" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/pkgscripts/libturbojpeg.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "lib" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libjpeg-turbo" TYPE FILE FILES
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/pkgscripts/libjpeg-turboConfig.cmake"
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/pkgscripts/libjpeg-turboConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "lib" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libjpeg-turbo/libjpeg-turboTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libjpeg-turbo/libjpeg-turboTargets.cmake"
         "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/CMakeFiles/Export/f0d506f335508d6549928070f26fb787/libjpeg-turboTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libjpeg-turbo/libjpeg-turboTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libjpeg-turbo/libjpeg-turboTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libjpeg-turbo" TYPE FILE FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/CMakeFiles/Export/f0d506f335508d6549928070f26fb787/libjpeg-turboTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libjpeg-turbo" TYPE FILE FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/CMakeFiles/Export/f0d506f335508d6549928070f26fb787/libjpeg-turboTargets-debug.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libjpeg-turbo" TYPE FILE FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/CMakeFiles/Export/f0d506f335508d6549928070f26fb787/libjpeg-turboTargets-minsizerel.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libjpeg-turbo" TYPE FILE FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/CMakeFiles/Export/f0d506f335508d6549928070f26fb787/libjpeg-turboTargets-relwithdebinfo.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libjpeg-turbo" TYPE FILE FILES "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/CMakeFiles/Export/f0d506f335508d6549928070f26fb787/libjpeg-turboTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "include" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/jconfig.h"
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/src/jerror.h"
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/src/jmorecfg.h"
    "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/src/jpeglib.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/libjpeg-turbo/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
