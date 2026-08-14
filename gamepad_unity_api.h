// Project: Gamepad-Core-Unity
// Copyright (c) 2026 valoto.games
// All rights reserved.
#pragma once

#include "gamepad_types_api.h"
#include "GCore/Types/Structs/Context/InputContext.h"

/** Callback used by the bridge to forward native log messages to the host. */
typedef void (*GCU_LogCallback)(int Level, const char* Message);

extern "C" {
    /** Shuts down the bridge and releases all registered devices and platform resources. */
    __declspec(dllexport) void GCU_Shutdown();

    /** Returns the bridge version as a null-terminated string owned by the DLL. */
    __declspec(dllexport) const char* GCU_GetVersion();

    /** Registers the callback that receives native log messages. Pass nullptr to disable it. */
    __declspec(dllexport) void GCU_SetLogCallback(GCU_LogCallback Callback);

    /**
     * @brief Initializes the device registry policy with Unity callbacks.
     *
     * This function must be called from Unity before any gamepad devices
     * can be detected or managed. It registers the necessary callbacks
     * for device allocation, dispatch, and disconnection, along with the
     * Unity engine type identifier.
     *
     * @param TypeId The Unity engine type identifier for gamepad devices.
     * @param allocCallback Callback function for allocating new devices in Unity.
     * @param dispatchCallback Callback function for dispatching initialized devices to Unity.
     * @param disconnectCallback Callback function for handling device disconnection in Unity.
     */
    __declspec(dllexport) void GCU_InitializeDeviceRegistryPolicy(
        int TypeId,
        AllocEngineDeviceCallback allocCallback,
        DispatchNewGamepadCallback dispatchCallback,
        DisconnectDeviceCallback disconnectCallback
    );

    /**
     * @brief Initializes the platform bridge with specified platform callback functions.
     *
     * This function sets up the platform bridge by assigning the provided platform callback
     * functions to their respective global variables. These callbacks are used to interact
     * with platform-specific functionality, such as reading data, writing data, detecting
     * devices, handling device creation, invalidation, and processing audio haptic feedback.
     * Additionally, it establishes the underlying platform hardware instance.
     *
     * @param readCallback                  Callback invoked when a platform read operation is required.
     * @param writeCallback                 Callback invoked when a platform write operation is required.
     * @param detectCallback                Callback invoked for detecting connected platform devices.
     * @param createHandleCallback          Callback invoked to create a handle for a platform device.
     * @param invalidateHandleCallback      Callback invoked to invalidate a handle for a platform device.
     * @param processAudioHapticCallback    Callback invoked to process audio-based haptic feedback.
     */
    __declspec(dllexport) void GCU_InitializePlatformBridge(
        PlatformReadCallback readCallback,
        PlatformWriteCallback writeCallback,
        PlatformDetectCallback detectCallback,
        PlatformCreateHandleCallback createHandleCallback,
        PlatformInvalidateHandleCallback invalidateHandleCallback,
        PlatformProcessAudioHapticCallback processAudioHapticCallback
    );

    /**
     * @brief Performs plug-and-play device discovery and management.
     *
     * This function should be called periodically from Unity's update loop to detect
     * newly connected gamepad devices and handle disconnections. It accumulates delta
     * time internally and performs actual device detection at fixed intervals to
     * optimize performance. When new devices are detected, the registered allocation
     * and dispatch callbacks will be invoked. When devices are disconnected, the
     * disconnection callback will be triggered.
     *
     * @param DeltaTime The time elapsed since the last frame in seconds.
     */
    __declspec(dllexport) void GCU_DiscoverDevices(float DeltaTime);

    /**
     * @brief Updates the input state of a registered gamepad.
     *
     * @param DeviceId The Unity/device identifier assigned by the allocation callback.
     * @param DeltaTime The time elapsed since the last frame in seconds.
     */
    __declspec(dllexport) void GCU_UpdateInput(int DeviceId, float DeltaTime);

    /**
     * @brief Copies the current input state of a registered gamepad.
     *
     * The destination must point to storage for an FInputContext. This form is
     * suitable for P/Invoke because the caller owns the destination buffer.
     *
     * @param DeviceId The Unity/device identifier assigned by the allocation callback.
     * @param InputState Destination buffer that receives the input state.
     * @return True when the device and destination are valid; false otherwise.
     */
    __declspec(dllexport) bool GCU_GetInputState(int DeviceId, FInputContext* InputState);
}
