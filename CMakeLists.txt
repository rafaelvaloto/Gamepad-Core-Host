cmake_minimum_required(VERSION 4.2)

project(GamepadCoreUnity VERSION 1.0.0)

set(GAMEPADCOREUNITY_VERSION "${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(3rdParty/Gamepad-Core)

file(GLOB_RECURSE GAMEPADCOREUNITY_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/*.cpp"
)

add_library(GamepadCoreUnity SHARED
        ${GAMEPADCOREUNITY_SOURCES}
)

target_include_directories(GamepadCoreUnity
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Public"
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(GamepadCoreUnity PRIVATE GamepadCore)

target_compile_definitions(GamepadCoreUnity PRIVATE
        GCU_VERSION_STRING="${GAMEPADCOREUNITY_VERSION}"
)
