cmake_minimum_required(VERSION 3.20)

project(GamepadCoreHost VERSION 1.0.6)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(GCL_DEBUG "Enable Gamepad-Core host logging" ON)

add_subdirectory(3rdParty/Gamepad-Core)

file(GLOB_RECURSE GAMEPADCORE_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/*.cpp"
)

if(EMSCRIPTEN)
    add_executable(GamepadCoreHost
            ${GAMEPADCORE_SOURCES}
    )

    set_target_properties(GamepadCoreHost PROPERTIES SUFFIX ".js")

    target_link_options(GamepadCoreHost PRIVATE
            "-sALLOW_TABLE_GROWTH=1"
            "-sMODULARIZE=1"
            "-sEXPORT_NAME='InitGamepadCoreHost'"
            "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap','getValue','setValue','addFunction','removeFunction']"
            "-sEXPORTED_FUNCTIONS=['_malloc','_free','_GCH_Shutdown','_GCH_GetVersion','_GCH_SetLogCallback','_GCH_DiscoverDevices','_GCH_CreateDevice','_GCH_UpdateInput','_GCH_UpdateOutput','_GCH_GetInputState','_GCH_GetDeviceDescriptor','_GCH_DeviceIsConnected','_GCH_GetDeviceType','_GCH_GetConnectionType','_GCH_BatteryLevelDevice','_GCH_Lightbar','_GCH_LightbarFlash','_GCH_PlayerLed','_GCH_ResetLights','_GCH_DualSenseSettings','_GCH_ResetGyroOrientation','_GCH_EnableGyroscopeValues','_GCH_EnableTouch','_GCH_CustomTrigger','_GCH_StopTrigger','_GCH_SetVibration','_GCH_InitializeDeviceRegistryPolicy','_GCH_InitializePlatformBridge','_GCH_InitializePlatformBridgeWasm','_GCH_InitializeDeviceRegistryPolicyWasm']"
    )
else()
    add_library(GamepadCoreHost SHARED
            ${GAMEPADCORE_SOURCES}
    )
endif()

target_include_directories(GamepadCoreHost
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Public"
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(GamepadCoreHost PRIVATE GamepadCore)

target_compile_definitions(GamepadCoreHost PRIVATE
        GCH_VERSION_STRING="${PROJECT_VERSION}"
        GCL_DEBUG=$<BOOL:${GCL_DEBUG}>
        $<$<BOOL:${EMSCRIPTEN}>:GCH_WEBASSEMBLY=1>
)
