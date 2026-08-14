// Project: Gamepad-Core-Unity
// Copyright (c) 2026 valoto.games
// All rights reserved.

#ifndef GAMEPADCOREUNITY_GAMEPAD_TYPES_API_H
#define GAMEPADCOREUNITY_GAMEPAD_TYPES_API_H

#include <cstdint>

/**
 * @file gamepad_types_api.h
 * @brief Unity integration API for Gamepad-Core library.
 *
 * This header defines the C/C++ API for integrating the Gamepad-Core library with Unity.
 * It provides callback types and external variables for device policy management,
 * allowing Unity to control device allocation, dispatch, and disconnection events.
 */


/**
 * @brief Represents a descriptor for a game controller device.
 *
 * This structure provides detailed information about a game controller device,
 * including its unique handle, type, connection status, and path.
 *
 * @struct GCUDeviceDescriptor
 */
struct GCUDeviceDescriptor {
    /// Unique platform-specific handle identifying the device hardware
    std::uint64_t Handle;
    /// Type identifier for the device (corresponds to EDSDeviceType: DualSense, DualSenseEdge, DualShock4, NotFound) 1,2,3,4
    std::int32_t DeviceType;
    /// Connection method type (corresponds to EDSDeviceConnection: Usb, Bluetooth, Unrecognized) 1,2,3
    std::int32_t ConnectionType;
    /// Connection status flag: non-zero if device is currently connected, zero otherwise
    std::int32_t IsConnected;
    /// Platform-specific device path or identifier string (null-terminated, max 512 bytes)
    char Path[512];
};


/**
 * @brief Callback type for logging messages from the library.
 *
 * This callback is invoked by the library to send log messages to Unity.
 * It allows Unity to handle logging through its own logging system,
 * enabling proper integration with Unity's console and debugging tools.
 *
 * @param Level The severity level of the log message (e.g., Info, Warning, Error).
 * @param Message Null-terminated string containing the log message text.
 */
typedef void (*GCU_LogCallback)(int Level, const char* Message);

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
 * @brief Callback type for reading platform device state.
 *
 * This callback is invoked to read the current state from a platform-specific device.
 * It should populate the device context with the latest input data from the hardware.
 *
 * @param Handle Handle to the device to read data from.
 * @param Buffer Pointer to the buffer to read data into.
 * @param Length Maximum number of bytes that can be read into the buffer.
 * @param BytesRead Pointer to an integer that will receive the number of bytes actually read.
 * @return std::int32_t Number of bytes actually read.
 */
typedef std::int32_t (*PlatformReadCallback)(std::uint64_t Handle, std::uint8_t *Buffer, std::int32_t Length,
                                             std::int32_t *BytesRead);

/**
 * @brief Callback type for writing output data to platform device.
 *
 * This callback is invoked to write output data (such as rumble, LED colors, etc.)
 * to a platform-specific device. It should send the output data stored in the
 * device context to the hardware.
 *
 * @param Handle Handle to the device to read data from.
 * @param Buffer Pointer to the buffer to read data into.
 * @param Length Maximum number of bytes that can be read into the buffer.
 * @param BytesWritten Pointer to the device context containing output data to write.
 * @return std::int32_t Number of bytes actually written.
 */
typedef std::int32_t (*PlatformWriteCallback)(std::uint64_t Handle, const std::uint8_t *Buffer, std::int32_t Length,
                                              std::int32_t *BytesWritten);

/**
 * @brief Callback type for detecting connected platform devices.
 *
 * This callback is invoked to detect all currently connected platform-specific devices.
 * It should populate the provided array of device contexts with information about
 * detected devices and return the number of devices found.
 *
 * @param Devices Array of device contexts to populate with detected devices.
 * @param MaxDevices Maximum number of devices that can be stored in the array.
 * @return std::int32_t The number of devices detected and populated in the array.
 */
typedef std::int32_t (*PlatformDetectCallback)(GCUDeviceDescriptor *Devices, int MaxDevices);

/**
 * @brief Callback type for configuring platform device features.
 *
 * This callback is invoked to configure specific features or settings on a platform-specific device.
 * It should send the configuration data to the device and report the number of bytes processed.
 *
 * @param Handle Handle to the device to configure.
 * @param Buffer Pointer to the buffer containing the configuration data to send.
 * @param Length Number of bytes in the configuration buffer.
 * @param Bytes Pointer to an integer that will receive the number of bytes processed.
 */
typedef void (*PlatformConfigureFeaturesCallback)(std::uint64_t Handle, const std::uint8_t *Buffer, std::int32_t Length,
                                                  std::int32_t *Bytes);

/**
 * @brief Callback type for creating platform device handle.
 *
 * This callback is invoked to create and initialize a platform-specific handle
 * for a device. It should establish the connection to the hardware and prepare
 * the device for reading and writing operations.
 *
 * @param Device Pointer to the device descriptor for which to create a handle.
 * @return True if the handle was successfully created, false otherwise.
 */
typedef int (*PlatformCreateHandleCallback)(GCUDeviceDescriptor *Device);

/**
 * @brief Callback type for invalidating platform device handle.
 *
 * This callback is invoked to invalidate and clean up a platform-specific device handle.
 * It should close the connection to the hardware and release any associated resources.
 *
 * @param Handle The handle to be invalidated.
 */
typedef void (*PlatformInvalidateHandleCallback)(std::uint64_t Handle);

/**
 * @brief Callback type for processing audio-based haptic feedback.
 *
 * This callback is invoked to process and apply audio-based haptic feedback to a device.
 * It should handle the conversion and transmission of audio data to the device's
 * haptic actuators.
 *
 * @param Handle The handle of the device for which to process haptic feedback.
 * @param Buffer Pointer to the audio data buffer containing the haptic feedback.
 * @param Length The length of the audio data buffer in bytes.
 * @param BytesWritten Pointer to an integer that will receive the number of bytes written to the device.
 */
typedef void (*PlatformProcessAudioHapticsCallback)(std::uint64_t Handle, const std::uint8_t *Buffer,
                                                   std::int32_t Length,
                                                   std::int32_t *BytesWritten);

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
 * @brief Global callback pointer for platform device feature configuration.
 *
 * This callback is invoked to configure specific features or settings on platform-specific devices.
 * Set during initialization via GCU_InitializePlatformBridge.
 */
extern PlatformConfigureFeaturesCallback g_UnityPlatformConfigureFeaturesCallback;

/**
 * @brief Global callback pointer for platform audio haptic processing.
 *
 * This callback is invoked to process audio-based haptic feedback on platform-specific devices.
 * Set during initialization via GCU_InitializePlatformBridge.
 */
extern PlatformProcessAudioHapticsCallback g_UnityPlatformProcessAudioHapticsCallback;

#endif //GAMEPADCOREUNITY_GAMEPAD_TYPES_API_H
