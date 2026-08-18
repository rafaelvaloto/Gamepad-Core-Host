// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.
#pragma once

#include "Types/GCHTypes.h"

#if defined(_WIN32) || defined(_WIN64)
    #define GCH_API __declspec(dllexport)
#elif defined(__unix__) || defined(__GNUC__) || defined(__clang__)
    #define GCH_API __attribute__((visibility("default")))
#else
    #define GCH_API
#endif

extern "C" {
/**
 * @brief Shuts down the game controller hardware library and releases associated resources.
 *
 * This function performs a full shutdown of the game controller hardware system, ensuring
 * that all callbacks, platform handles, and internal resources are properly cleared. It
 * sets all global callbacks and configuration pointers to null, invalidates engine type
 * identifiers, and resets the platform hardware instance. This should be called during
 * the application's termination sequence to ensure a clean and consistent shutdown.
 */
GCH_API void GCH_Shutdown();

/**
 * @brief Retrieves the current version of the library.
 *
 * This function returns a string representing the version
 * of the library being used. The version is defined at compile
 * time and remains constant for a given build.
 *
 * @return A constant C-string containing the library version.
 */
GCH_API const char* GCH_GetVersion();

/**
 * @brief Sets the callback function for handling log messages from the library.
 *
 * This function assigns a user-defined callback that the library will invoke
 * to deliver logging messages. This is primarily used for debugging or monitoring
 * internal operations within the library. If the callback is not set, logging messages
 * will not be delivered.
 *
 * @param Callback The callback function to handle log messages. The function should
 * take two parameters: an integer representing the log level, and a C-string containing
 * the log message.
 */
GCH_API void GCH_SetLogCallback(GamepadLogCallback Callback);

}