# CPM - package manager
include(FetchContent)


FetchContent_Declare(
        CPM
        GIT_REPOSITORY https://github.com/TheLartians/CPM.cmake.git
        GIT_TAG        v0.32.3
)


FetchContent_MakeAvailable(CPM)

#based on your ide set to different location
set(deps ${CMAKE_CURRENT_BINARY_DIR}/../_deps)


include(${deps}/cpm-src/cmake/CPM.cmake)


find_package(OpenGL REQUIRED)


set(GLAD_DIR ${CMAKE_CURRENT_LIST_DIR}/glad)
set(glad_SOURCE_DIR ${GLAD_DIR}/include CACHE INTERNAL "")
add_library(glad STATIC ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)


set(STB_IMAGE_DIR ${CMAKE_CURRENT_LIST_DIR}/stb_image)
set(stb_image_SOURCE_DIR ${STB_IMAGE_DIR} CACHE INTERNAL "")
add_library(stb_image STATIC ${STB_IMAGE_DIR}/stb_image.cpp)
target_include_directories(stb_image PRIVATE ${STB_IMAGE_DIR})


CPMAddPackage("gh:assimp/assimp@5.3.1")
CPMAddPackage("gh:glfw/glfw#3.4")
CPMAddPackage("gh:g-truc/glm#1.0.1")
CPMAddPackage("gh:ocornut/imgui@1.90.4")
CPMAddPackage("gh:gabime/spdlog@1.13.0")
CPMAddPackage("gh:jbeder/yaml-cpp#0.8.0")
CPMAddPackage("gh:nlohmann/json@3.11.3")
CPMAddPackage("gh:effolkronium/random#v1.5.0")
CPMAddPackage("gh:CedricGuillemet/ImGuizmo#1.83")
CPMAddPackage("gh:freetype/freetype#VER-2-13-2")
CPMAddPackage("gh:mackron/miniaudio#0.11.21")
CPMAddPackage("gh:iauns/cpm-glm-aabb#1.0.4")


add_library(miniaudio INTERFACE)
target_include_directories(miniaudio INTERFACE ${deps}/miniaudio-src)


set(imgui_SOURCE_DIR ${imgui_SOURCE_DIR} CACHE INTERNAL "")
add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp)


set(ImGuizmo_SOURCE_DIR ${ImGuizmo_SOURCE_DIR} CACHE INTERNAL "")
add_library(gizmos STATIC ${ImGuizmo_SOURCE_DIR}/ImGuizmo.cpp
        ${ImGuizmo_SOURCE_DIR}/GraphEditor.cpp
        ${ImGuizmo_SOURCE_DIR}/ImCurveEdit.cpp
        ${ImGuizmo_SOURCE_DIR}/ImGradient.cpp
        ${ImGuizmo_SOURCE_DIR}/ImSequencer.cpp)


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
        glm_aabb
        miniaudio
        effolkronium_random
        PROPERTIES FOLDER "vendor")


if (TARGET zlibstatic)
    set_target_properties(zlibstatic PROPERTIES FOLDER "vendor")
endif()