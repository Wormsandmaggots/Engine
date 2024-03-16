# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Projects/C++/Engine/cmake-build-debug/_deps/imguizmo-src"
  "D:/Projects/C++/Engine/cmake-build-debug/_deps/imguizmo-build"
  "D:/Projects/C++/Engine/cmake-build-debug/_deps/imguizmo-subbuild/imguizmo-populate-prefix"
  "D:/Projects/C++/Engine/cmake-build-debug/_deps/imguizmo-subbuild/imguizmo-populate-prefix/tmp"
  "D:/Projects/C++/Engine/cmake-build-debug/_deps/imguizmo-subbuild/imguizmo-populate-prefix/src/imguizmo-populate-stamp"
  "D:/Projects/C++/Engine/cmake-build-debug/_deps/imguizmo-subbuild/imguizmo-populate-prefix/src"
  "D:/Projects/C++/Engine/cmake-build-debug/_deps/imguizmo-subbuild/imguizmo-populate-prefix/src/imguizmo-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Projects/C++/Engine/cmake-build-debug/_deps/imguizmo-subbuild/imguizmo-populate-prefix/src/imguizmo-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Projects/C++/Engine/cmake-build-debug/_deps/imguizmo-subbuild/imguizmo-populate-prefix/src/imguizmo-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
