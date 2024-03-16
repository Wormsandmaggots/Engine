# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Projects/C++/Engine/cmake-build-debug/_deps/glew-cmake-src"
  "D:/Projects/C++/Engine/cmake-build-debug/_deps/glew-cmake-build"
  "D:/Projects/C++/Engine/cmake-build-debug/_deps/glew-cmake-subbuild/glew-cmake-populate-prefix"
  "D:/Projects/C++/Engine/cmake-build-debug/_deps/glew-cmake-subbuild/glew-cmake-populate-prefix/tmp"
  "D:/Projects/C++/Engine/cmake-build-debug/_deps/glew-cmake-subbuild/glew-cmake-populate-prefix/src/glew-cmake-populate-stamp"
  "D:/Projects/C++/Engine/cmake-build-debug/_deps/glew-cmake-subbuild/glew-cmake-populate-prefix/src"
  "D:/Projects/C++/Engine/cmake-build-debug/_deps/glew-cmake-subbuild/glew-cmake-populate-prefix/src/glew-cmake-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Projects/C++/Engine/cmake-build-debug/_deps/glew-cmake-subbuild/glew-cmake-populate-prefix/src/glew-cmake-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Projects/C++/Engine/cmake-build-debug/_deps/glew-cmake-subbuild/glew-cmake-populate-prefix/src/glew-cmake-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
