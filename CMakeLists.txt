cmake_minimum_required(VERSION 3.28)

project(linear-algebra-visualisator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(app
    linear-algebra-cpp/linear-algebra-cpp.cpp
    third_party_libs/imgui.cpp
    third_party_libs/imgui_draw.cpp
    third_party_libs/imgui_tables.cpp
    third_party_libs/imgui_widgets.cpp
    third_party_libs/rlImGui-main/rlImGui.cpp
)

target_sources(app
    PUBLIC
    FILE_SET CXX_MODULES FILES
        MathEngine.cppm
)
set_target_properties(app PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

target_include_directories(app PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party_libs
    ${CMAKE_CURRENT_SOURCE_DIR}/packages/raylib.5.5.0/build/native/include
)
target_link_directories(app PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/packages/raylib.5.5.0/build/native/lib/x64
)

target_link_libraries(app PRIVATE
    raylib
    winmm
    gdi32
    opengl32
    shell32
)

add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/config.json"
        "$<TARGET_FILE_DIR:app>/config.json"
    COMMENT "Copying config.json to build directory..."
)