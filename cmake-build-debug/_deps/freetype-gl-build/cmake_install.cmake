# Install script for directory: D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/ENGINE")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "D:/CLion 2023.3.4/bin/mingw/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/ENGINE/share/pkgconfig/freetype-gl.pc")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Program Files (x86)/ENGINE/share/pkgconfig" TYPE FILE FILES "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-build/freetype-gl.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-build/libfreetype-gld.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freetype-gl" TYPE FILE PERMISSIONS OWNER_READ GROUP_READ WORLD_READ FILES
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/distance-field.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/edtaa3func.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/font-manager.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/freetype-gl.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/markup.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/opengl.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/platform.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/text-buffer.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/texture-atlas.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/texture-font.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/utf8-utils.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/ftgl-utils.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/vec234.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/vector.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/vertex-attribute.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/vertex-buffer.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/freetype-gl-errdef.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-build/config.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-build/libfreetype-gld.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/distance-field.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/edtaa3func.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/font-manager.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/freetype-gl.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/markup.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/opengl.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/platform.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/text-buffer.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/texture-atlas.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/texture-font.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/utf8-utils.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/ftgl-utils.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/vec234.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/vector.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/vertex-attribute.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/vertex-buffer.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-src/freetype-gl-errdef.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-build/config.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-build/doc/cmake_install.cmake")
  include("D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-build/demos/cmake_install.cmake")
  include("D:/Projects/C++/Engine/cmake-build-debug/_deps/freetype-gl-build/tests/cmake_install.cmake")

endif()

