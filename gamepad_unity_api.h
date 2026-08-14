// Project: Gamepad-Core-Unity
// Copyright (c) 2026 valoto.games
// All rights reserved.
#pragma once

/**
 * @file gamepad_unity_api.h
 * @brief Unity integration API for Gamepad-Core library.
 *
 * This header defines the C/C++ API for integrating the Gamepad-Core library with Unity.
 * It provides callback types and external variables for device policy management,
 * allowing Unity to control device allocation, dispatch, and disconnection events.
 */

/**
 * @brief Callback type for allocating a new engine device.
 *
 * This callback is invoked by the library when a new gamepad device is detected
 * and needs to be allocated in the Unity engine. The callback should create
 * a new device instance in Unity and return its unique identifier.
 *
 * @return The unique identifier of the newly allocated device in Unity.
 */
typedef int (*AllocEngineDeviceCallback)();
/**
 * @brief Callback type for dispatching a new gamepad device to Unity.
 *
 * This callback is invoked after a gamepad device has been allocated and
 * initialized. It notifies Unity that the device is ready to be used.
 *
 * @param deviceId The unique identifier of the gamepad device to dispatch.
 */
typedef void (*DispatchNewGamepadCallback)(int);
/**
 * @brief Callback type for handling device disconnection.
 *
 * This callback is invoked when a gamepad device is disconnected.
 * Unity should clean up any resources associated with the device.
 *
 * @param deviceId The unique identifier of the disconnected device.
 */
typedef void (*DisconnectDeviceCallback)(int);

/**
 * @brief Represents the context for a specific input device.
 *
 * This structure encapsulates the state and configuration for a single input device,
 * such as a gamepad. It is primarily managed by the PlatformBridgePolicy class to
 * perform operations such as reading, writing, and detecting device states, as well
 * as creating and invalidating device handles.
 */
struct FDeviceContext;

/**
 * @brief Callback type for reading platform device state.
 *
 * This callback is invoked to read the current state from a platform-specific device.
 * It should populate the device context with the latest input data from the hardware.
 *
 * @param deviceContext Pointer to the device context to read data into.
 */
typedef void (*PlatformReadCallback)(FDeviceContext *);

/**
 * @brief Callback type for writing output data to platform device.
 *
 * This callback is invoked to write output data (such as rumble, LED colors, etc.)
 * to a platform-specific device. It should send the output data stored in the
 * device context to the hardware.
 *
 * @param deviceContext Pointer to the device context containing output data to write.
 */
typedef void (*PlatformWriteCallback)(FDeviceContext *);

/**
 * @brief Callback type for detecting connected platform devices.
 *
 * This callback is invoked to detect all currently connected platform-specific devices.
 * It should populate the provided array of device contexts with information about
 * detected devices and return the number of devices found.
 *
 * @param Devices Array of device contexts to populate with detected devices.
 * @param Capacity Maximum number of devices that can be stored in the array.
 * @return The number of devices detected and populated in the array.
 */
typedef int (*PlatformDetectCallback)(FDeviceContext *Devices, int Capacity);

/**
 * @brief Callback type for creating platform device handle.
 *
 * This callback is invoked to create and initialize a platform-specific handle
 * for a device. It should establish the connection to the hardware and prepare
 * the device for reading and writing operations.
 *
 * @param deviceContext Pointer to the device context for which to create a handle.
 * @return True if the handle was successfully created, false otherwise.
 */
typedef bool (*PlatformCreateHandleCallback)(FDeviceContext *);

/**
 * @brief Callback type for invalidating platform device handle.
 *
 * This callback is invoked to invalidate and clean up a platform-specific device handle.
 * It should close the connection to the hardware and release any associated resources.
 *
 * @param deviceContext Pointer to the device context whose handle should be invalidated.
 */
typedef void (*PlatformInvalidateHandleCallback)(FDeviceContext *);

/**
 * @brief Callback type for processing audio-based haptic feedback.
 *
 * This callback is invoked to process and apply audio-based haptic feedback to a device.
 * It should handle the conversion and transmission of audio data to the device's
 * haptic actuators.
 *
 * @param deviceContext Pointer to the device context for which to process haptic feedback.
 */
typedef void (*PlatformProcessAudioHapticCallback)(FDeviceContext*);

/**
 * @brief Global variable storing the Unity engine type identifier.
 *
 * This variable holds the type identifier used by Unity to classify
 * gamepad devices. It is set during initialization via
 * GCU_InitializeDeviceRegistryPolicy.
 */
extern int g_UnityEngineTypeId;
/**
 * @brief Global callback pointer for device allocation.
 *
 * This callback is invoked when the library needs to allocate a new
 * device in Unity. Set during initialization via
 * GCU_InitializeDeviceRegistryPolicy.
 */
extern AllocEngineDeviceCallback g_UnityAllocDeviceCallback;
/**
 * @brief Global callback pointer for device dispatch.
 *
 * This callback is invoked when a new gamepad device is ready to be
 * dispatched to Unity. Set during initialization via
 * GCU_InitializeDeviceRegistryPolicy.
 */
extern DispatchNewGamepadCallback g_UnityDispatchDeviceCallback;
/**
 * @brief Global callback pointer for device disconnection.
 *
 * This callback is invoked when a gamepad device is disconnected.
 * Set during initialization via GCU_InitializeDeviceRegistryPolicy.
 */
extern DisconnectDeviceCallback g_UnityDisconnectDeviceCallback;

/**
 * @brief Global callback pointer for platform read operations.
 *
 * This callback is invoked to read the current state from platform-specific devices.
 * Set during initialization via GCU_InitializePlatformBridge.
 */
extern PlatformReadCallback g_UnityPlatformReadCallback;

/**
 * @brief Global callback pointer for platform write operations.
 *
 * This callback is invoked to write output data to platform-specific devices.
 * Set during initialization via GCU_InitializePlatformBridge.
 */
extern PlatformWriteCallback g_UnityPlatformWriteCallback;

/**
 * @brief Global callback pointer for platform device detection.
 *
 * This callback is invoked to detect connected platform-specific devices.
 * Set during initialization via GCU_InitializePlatformBridge.
 */
extern PlatformDetectCallback g_UnityPlatformDetectCallback;

/**
 * @brief Global callback pointer for platform device handle creation.
 *
 * This callback is invoked to create platform-specific device handles.
 * Set during initialization via GCU_InitializePlatformBridge.
 */
extern PlatformCreateHandleCallback g_UnityPlatformCreateHandleCallback;

/**
 * @brief Global callback pointer for platform device handle invalidation.
 *
 * This callback is invoked to invalidate and clean up platform-specific device handles.
 * Set during initialization via GCU_InitializePlatformBridge.
 */
extern PlatformInvalidateHandleCallback g_UnityPlatformInvalidateHandleCallback;

/**
 * @brief Global callback pointer for platform audio haptic processing.
 *
 * This callback is invoked to process audio-based haptic feedback on platform-specific devices.
 * Set during initialization via GCU_InitializePlatformBridge.
 */
extern PlatformProcessAudioHapticCallback g_UnityPlatformProcessAudioHapticCallback;

extern "C" {
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
}
