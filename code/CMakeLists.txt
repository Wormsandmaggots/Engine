file(GLOB_RECURSE SOURCE_FILES
        *.c
        *.cpp)


file(GLOB_RECURSE HEADER_FILES
        *.h
        *.hpp)

#based on your ide set to different location
file(GLOB_RECURSE dlls
        ${CMAKE_CURRENT_BINARY_DIR}/../_deps/*.dll)


file(GLOB_RECURSE ASSETS_FILES
        ${CMAKE_SOURCE_DIR}/res/*.*)


set_source_files_properties(${ASSETS_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_FILES})
source_group(TREE ${CMAKE_SOURCE_DIR}         FILES ${ASSETS_FILES})


add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${ASSETS_FILES}
        Engine/src/Text/Text.cpp
        Engine/inc/Text/Text.h
        Engine/inc/Text/Text.h
        Engine/inc/Debug/Profiler.h
        Engine/src/Debug/Profiler.cpp
        Engine/inc/ThirdPersonCamera.h
        Engine/src/ThirdPersonCamera.cpp
        Engine/inc/Input/Input.h
        Engine/inc/Input/Input.h
        Engine/inc/Input/Input.h
        Engine/inc/Input/DebugInput.h
        Engine/inc/Input/DebugInput.h
        Engine/src/HUD/Image.cpp
        Engine/inc/HUD/Image.h
        Engine/inc/HUD/Image.h
        Engine/src/HUD/Image.cpp
        Engine/inc/HUD/BackgroundImage.h
        Engine/inc/HUD/BackgroundImage.h
        Engine/inc/HUD/ProgressBar.h
        Engine/inc/HUD/ProgressBar.h
        Engine/inc/assimp_glm_helpers.h
        Engine/inc/RigPrep.h
        Engine/inc/Animation/Bone.h
        Engine/inc/Animation/AnimBone.h
        Engine/inc/Animation/InverseKinematics.h
        Engine/inc/Animation/Animation.h
        Engine/inc/Animation/Animator.h
)


target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
target_compile_definitions(${PROJECT_NAME} PRIVATE LIBRARY_SUFFIX="")


target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${glad_SOURCE_DIR}
        ${stb_image_SOURCE_DIR}
        ${imgui_SOURCE_DIR}
        ${ImGuizmo_SOURCE_DIR}
        ${miniaudio_SOURCE_DIR}
        ${aabb_SOURCE_DIR}
        ${ftgl_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine/inc
)


target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        glad
        stb_image
        assimp
        glfw
        imgui
        spdlog
        glm::glm
        nlohmann_json::nlohmann_json
        yaml-cpp
        freetype
        miniaudio
        #aabb
        effolkronium_random
        gizmos
        ftgl
        TracyClient
)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/res
        ${CMAKE_CURRENT_BINARY_DIR}/res)


if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC NOMINMAX)
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


copy_dll_dependencies( ${CMAKE_CURRENT_BINARY_DIR} "${dlls}")