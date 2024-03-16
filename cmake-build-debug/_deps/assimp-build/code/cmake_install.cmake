# Install script for directory: D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code

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

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.3.0-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-build/lib/libassimpd.dll.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.3.0" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-build/bin/libassimp-5d.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp-5d.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp-5d.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "D:/CLion 2023.3.4/bin/mingw/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp-5d.dll")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/anim.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/aabb.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ai_assert.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/camera.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/color4.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/color4.inl"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-build/code/../include/assimp/config.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ColladaMetaData.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/commonMetaData.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/defs.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/cfileio.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/light.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/material.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/material.inl"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/matrix3x3.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/matrix3x3.inl"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/matrix4x4.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/matrix4x4.inl"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/mesh.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ObjMaterial.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/pbrmaterial.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/GltfMaterial.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/postprocess.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/quaternion.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/quaternion.inl"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/scene.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/metadata.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/texture.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/types.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/vector2.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/vector2.inl"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/vector3.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/vector3.inl"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/version.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/cimport.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/AssertHandler.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/importerdesc.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Importer.hpp"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/DefaultLogger.hpp"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ProgressHandler.hpp"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/IOStream.hpp"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/IOSystem.hpp"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Logger.hpp"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/LogStream.hpp"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/NullLogger.hpp"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/cexport.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Exporter.hpp"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/DefaultIOStream.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/DefaultIOSystem.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ZipArchiveIOSystem.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SceneCombiner.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/fast_atof.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/qnan.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/BaseImporter.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Hash.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/MemoryIOWrapper.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ParsingUtils.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/StreamReader.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/StreamWriter.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/StringComparison.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/StringUtils.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SGSpatialSort.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/GenericProperty.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SpatialSort.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SkeletonMeshBuilder.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SmallVector.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SmoothingGroups.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SmoothingGroups.inl"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/StandardShapes.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/RemoveComments.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Subdivision.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Vertex.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/LineSplitter.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/TinyFormatter.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Profiler.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/LogAux.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Bitmap.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/XMLTools.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/IOStreamBuffer.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/CreateAnimMesh.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/XmlParser.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/BlobIOSystem.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/MathFunctions.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Exceptional.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ByteSwapper.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Base64.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Compiler/pushpack1.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Compiler/poppack1.h"
    "D:/Projects/C++/Engine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

