# CPM - package manager
include(FetchContent)

FetchContent_Declare(
        CPM
        GIT_REPOSITORY https://github.com/TheLartians/CPM.cmake.git
        GIT_TAG v0.32.3
)

FetchContent_MakeAvailable(CPM)

# based on your ide set to different location
set(deps ${CMAKE_CURRENT_BINARY_DIR}/../_deps)
set(vendor ${CMAKE_CURRENT_LIST_DIR})

include(${deps}/cpm-src/cmake/CPM.cmake)

find_package(OpenGL REQUIRED)

# set(GLEW_LIBRARY ${CMAKE_CURRENT_LIST_DIR}/glew/lib/Release/Win32/glew32.lib)
# set(GLEW_INCLUDE_PATH  ${CMAKE_CURRENT_LIST_DIR}/glew/include)
CPMAddPackage("gh:assimp/assimp@5.3.1")
CPMAddPackage("gh:glfw/glfw#3.4")
CPMAddPackage("gh:g-truc/glm#1.0.1")
CPMAddPackage("gh:gabime/spdlog@1.13.0")
CPMAddPackage("gh:jbeder/yaml-cpp#0.8.0")
CPMAddPackage("gh:nlohmann/json@3.11.3")
CPMAddPackage("gh:freetype/freetype#VER-2-13-2")
CPMAddPackage("gh:libsndfile/libsndfile#1.2.2")

# if one of thirdparty libraries is not building you maybe have to do smth like this below
# and remember to add source dir to exe in code
set(miniaudio_DIR ${CMAKE_CURRENT_LIST_DIR}/miniaudio)
set(miniaudio_SOURCE_DIR ${miniaudio_DIR} CACHE INTERNAL "")
add_library(miniaudio STATIC ${miniaudio_DIR}/miniaudio.c)
target_include_directories(miniaudio PRIVATE ${miniaudio_DIR})

set(GLAD_DIR ${CMAKE_CURRENT_LIST_DIR}/glad)
set(glad_SOURCE_DIR ${GLAD_DIR}/include CACHE INTERNAL "")
add_library(glad STATIC ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)

set(STB_IMAGE_DIR ${CMAKE_CURRENT_LIST_DIR}/stb_image)
set(stb_image_SOURCE_DIR ${STB_IMAGE_DIR} CACHE INTERNAL "")
add_library(stb_image STATIC ${STB_IMAGE_DIR}/stb_image.cpp)
target_include_directories(stb_image PRIVATE ${STB_IMAGE_DIR})


set(fftw_dir ${CMAKE_CURRENT_LIST_DIR}/fftw CACHE INTERNAL "")


add_subdirectory(joyshock)


set_target_properties(glad
        stb_image
        assimp
        glfw
        glm
        spdlog
        yaml-cpp
        nlohmann_json
        freetype
        miniaudio
        sndfile
        JoyShockLibrary
        PROPERTIES FOLDER "vendor"
)

if(TARGET zlibstatic)
        set_target_properties(zlibstatic PROPERTIES FOLDER "vendor")
endif()

function(copy_dll_dependencies DESTINATION_DIR DEPENDENCIES)

        if (NOT DESTINATION_DIR)
                message(FATAL_ERROR "copy_dll_dependencies: DESTINATION_DIR not specified")
        endif()

        if (NOT DEPENDENCIES)
                message(STATUS "copy_dll_dependencies: DEPENDENCIES not specified or nothing to copy")
        endif()

        foreach (DLL_PATH ${DEPENDENCIES})

                get_filename_component(DLL_NAME ${DLL_PATH} NAME)
                set(DLL_DESTINATION ${DESTINATION_DIR}/${DLL_NAME})

                if (NOT EXISTS ${DLL_DESTINATION})
                        message(STATUS "${DLL_NAME} not found in destination directory, copying...")
                        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DLL_PATH}" "${DLL_DESTINATION}")
                else()
                        message(STATUS "${DLL_NAME} has been found in destination directory, no need for copying")
                endif()

        endforeach()

endfunction()

file(GLOB_RECURSE dlls
        ${CMAKE_CURRENT_LIST_DIR}/*.dll
)

copy_dll_dependencies( ${CMAKE_CURRENT_BINARY_DIR}/../code "${dlls}")