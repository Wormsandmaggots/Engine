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

include(${deps}/cpm-src/cmake/CPM.cmake)

find_package(OpenGL REQUIRED)

# set(GLEW_LIBRARY ${CMAKE_CURRENT_LIST_DIR}/glew/lib/Release/Win32/glew32.lib)
# set(GLEW_INCLUDE_PATH  ${CMAKE_CURRENT_LIST_DIR}/glew/include)
CPMAddPackage("gh:assimp/assimp@5.3.1")
CPMAddPackage("gh:glfw/glfw#3.4")
CPMAddPackage("gh:g-truc/glm#1.0.1")
CPMAddPackage("gh:ocornut/imgui@1.88")
CPMAddPackage("gh:gabime/spdlog@1.13.0")
CPMAddPackage("gh:jbeder/yaml-cpp#0.8.0")
CPMAddPackage("gh:nlohmann/json@3.11.3")
CPMAddPackage("gh:effolkronium/random#v1.5.0")
CPMAddPackage("gh:CedricGuillemet/ImGuizmo#1.83")
CPMAddPackage("gh:freetype/freetype#VER-2-13-2")
CPMAddPackage("gh:wolfpld/tracy#v0.10")
#CPMAddPackage("gh:FFTW/fftw3#fftw-3.3.7")
CPMAddPackage("gh:fftw/fftw3#fftw-3.3.10")
CPMAddPackage("gh:libsndfile/libsndfile#1.2.2")
CPMAddPackage("gh:libsndfile/libsndfile#v1.2.2")


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

set(imgui_SOURCE_DIR ${imgui_SOURCE_DIR} CACHE INTERNAL "")
add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp)

set(ImGuizmo_SOURCE_DIR ${ImGuizmo_SOURCE_DIR} CACHE INTERNAL "")
add_library(gizmos STATIC
        imguizmo/ImGuizmo.cpp
        ${ImGuizmo_SOURCE_DIR}/GraphEditor.cpp
        ${ImGuizmo_SOURCE_DIR}/ImCurveEdit.cpp
        ${ImGuizmo_SOURCE_DIR}/ImGradient.cpp
        ${ImGuizmo_SOURCE_DIR}/ImSequencer.cpp)
target_include_directories(gizmos PRIVATE ${imgui_SOURCE_DIR})

set(aabb_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/glm-aabb CACHE INTERNAL "")
add_library(aabb STATIC
        ${aabb_SOURCE_DIR}/AABB.cpp)
target_include_directories(aabb PRIVATE ${aabb_SOURCE_DIR})
target_link_libraries(aabb glm)

set(fftw ${CMAKE_CURRENT_LIST_DIR}/fftw CACHE INTERNAL "")
add_library(fftw_lib INTERFACE)
target_include_directories(fftw_lib INTERFACE ${fftw})

set(ftgl_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/freetype-gl CACHE INTERNAL "")
add_library(ftgl STATIC
        ${ftgl_SOURCE_DIR}/distance-field.c
        ${ftgl_SOURCE_DIR}/edtaa3func.c
        ${ftgl_SOURCE_DIR}/font-manager.c
        ${ftgl_SOURCE_DIR}/ftgl-utils.c
        ${ftgl_SOURCE_DIR}/makefont.c
        ${ftgl_SOURCE_DIR}/platform.c
        ${ftgl_SOURCE_DIR}/text-buffer.c
        ${ftgl_SOURCE_DIR}/texture-font.c
        ${ftgl_SOURCE_DIR}/utf8-utils.c
        ${ftgl_SOURCE_DIR}/vector.c
        ${ftgl_SOURCE_DIR}/vertex-attribute.c
        ${ftgl_SOURCE_DIR}/vertex-buffer.c)
target_include_directories(ftgl PRIVATE
        ${ftgl_SOURCE_DIR}
        ${glad_SOURCE_DIR})
target_link_libraries(ftgl
        glad
        freetype)

set_target_properties(glad
        stb_image
        assimp
        glfw
        glm
        spdlog
        imgui
        gizmos
        yaml-cpp
        nlohmann_json
        freetype
        #aabb
        miniaudio
        effolkronium_random
        ftgl
        TracyClient
        sndfile
        fftw_lib
        PROPERTIES FOLDER "vendor")

if(TARGET zlibstatic)
        set_target_properties(zlibstatic PROPERTIES FOLDER "vendor")
endif()