cmake_minimum_required(VERSION 4.2)

project(GamepadCoreHost VERSION 1.0.6)

set(CMAKE_CXX_STANDARD 20)
option(GCL_DEBUG "Enable Gamepad-Core host logging" OFF)

add_subdirectory(3rdParty/Gamepad-Core)

file(GLOB_RECURSE GAMEPADCORE_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/*.cpp"
)

add_library(GamepadCoreHost SHARED
        ${GAMEPADCORE_SOURCES}
)

target_include_directories(GamepadCoreHost
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Public"
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(GamepadCoreHost PRIVATE GamepadCore)

target_compile_definitions(GamepadCoreHost PRIVATE
        GCH_VERSION_STRING="${PROJECT_VERSION}"
        GCL_DEBUG=$<BOOL:${GCL_DEBUG}>
)
