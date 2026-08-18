// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.

#ifndef GAMEPADCORE_GAMEPAD_TYPES_API_H
#define GAMEPADCORE_GAMEPAD_TYPES_API_H

#include <cstddef>
#include <cstdint>

#ifndef GCL_DEBUG
#define GCL_DEBUG 0
#endif

#ifndef GCH_VERSION_STRING
#define GCH_VERSION_STRING "v1.0.6"
#endif

constexpr int MaxDetectedDevices = 64;
constexpr char GCH_VERSION[] = GCH_VERSION_STRING;

/**
 * @file GCHTypes.h
 * @brief Host integration API for Gamepad-Core library.
 *
 * This header defines the C/C++ API for integrating the Gamepad-Core library with Host.
 * It provides callback types and external variables for device policy management,
 * allowing Host to control device allocation, dispatch, and disconnection events.
 */


/**
 * @brief Represents a descriptor for a game controller device.
 *
 * This structure provides detailed information about a game controller device,
 * including its unique handle, type, connection status, and path.
 *
 * @struct GamepadDeviceDescriptor
 */
struct GamepadDeviceDescriptor {
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

static_assert(offsetof(GamepadDeviceDescriptor, Handle) == 0);
static_assert(offsetof(GamepadDeviceDescriptor, DeviceType) == 8);
static_assert(offsetof(GamepadDeviceDescriptor, ConnectionType) == 12);
static_assert(offsetof(GamepadDeviceDescriptor, IsConnected) == 16);
static_assert(offsetof(GamepadDeviceDescriptor, Path) == 20);

/**
 * @brief Callback type for logging messages from the library.
 *
 * This callback is invoked by the library to send log messages to Host.
 * It allows Host to handle logging through its own logging system,
 * enabling proper integration with Host's console and debugging tools.
 *
 * @param Level The severity level of the log message (e.g., Info, Warning, Error).
 * @param Message Null-terminated string containing the log message text.
 */
typedef void (*GamepadLogCallback)(int Level, const char *Message);

/**
 * @brief Callback type for allocating a new engine device.
 *
 * This callback is invoked by the library when a new gamepad device is detected
 * and needs to be allocated in the Host engine. The callback should create
 * a new device instance in Host and return its unique identifier.
 *
 * @return The unique identifier of the newly allocated device in Host.
 */
typedef int (*AllocEngineDeviceCallback)();

/**
 * @brief Callback type for dispatching a new gamepad device to Host.
 *
 * This callback is invoked after a gamepad device has been allocated and
 * initialized. It notifies Host that the device is ready to be used.
 *
 * @param deviceId The unique identifier of the gamepad device to dispatch.
 */
typedef void (*DispatchNewGamepadCallback)(int);

/**
 * @brief Callback type for handling device disconnection.
 *
 * This callback is invoked when a gamepad device is disconnected.
 * Host should clean up any resources associated with the device.
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
 * @return bool Number of bytes actually read.
 */
typedef bool (*PlatformReadCallback)(std::uint64_t Handle, std::uint8_t *Buffer, std::int32_t Length,
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
 * @return bool Number of bytes actually written.
 */
typedef bool (*PlatformWriteCallback)(std::uint64_t Handle, std::uint8_t *Buffer, std::int32_t Length,
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
typedef std::int32_t (*PlatformDetectCallback)(GamepadDeviceDescriptor *Devices, int MaxDevices);

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
 * @return True if the handle was successfully created, false otherwise.
 */
typedef bool (*PlatformCreateHandleCallback)(GamepadDeviceDescriptor *Device);

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
 * @brief Global variable storing the Host engine type identifier.
 *
 * This variable holds the type identifier used by Host to classify
 * gamepad devices. It is set during initialization via
 * GCH_InitializeDeviceRegistryPolicy.
 */
extern int g_EngineTypeId;
/**
 * @brief Global callback pointer for device allocation.
 *
 * This callback is invoked when the library needs to allocate a new
 * device in Host. Set during initialization via
 * GCH_InitializeDeviceRegistryPolicy.
 */
extern AllocEngineDeviceCallback g_AllocDeviceCallback;
/**
 * @brief Global callback pointer for device dispatch.
 *
 * This callback is invoked when a new gamepad device is ready to be
 * dispatched to Host. Set during initialization via
 * GCH_InitializeDeviceRegistryPolicy.
 */
extern DispatchNewGamepadCallback g_DispatchDeviceCallback;
/**
 * @brief Global callback pointer for device disconnection.
 *
 * This callback is invoked when a gamepad device is disconnected.
 * Set during initialization via GCH_InitializeDeviceRegistryPolicy.
 */
extern DisconnectDeviceCallback g_DisconnectDeviceCallback;

/**
 * @brief Global callback pointer for platform read operations.
 *
 * This callback is invoked to read the current state from platform-specific devices.
 * Set during initialization via GCH_InitializePlatformBridge.
 */
extern PlatformReadCallback g_PlatformReadCallback;

/**
 * @brief Global callback pointer for platform write operations.
 *
 * This callback is invoked to write output data to platform-specific devices.
 * Set during initialization via GCH_InitializePlatformBridge.
 */
extern PlatformWriteCallback g_PlatformWriteCallback;

/**
 * @brief Global callback pointer for platform device detection.
 *
 * This callback is invoked to detect connected platform-specific devices.
 * Set during initialization via GCH_InitializePlatformBridge.
 */
extern PlatformDetectCallback g_PlatformDetectCallback;

/**
 * @brief Global callback pointer for platform device handle creation.
 *
 * This callback is invoked to create platform-specific device handles.
 * Set during initialization via GCH_InitializePlatformBridge.
 */
extern PlatformCreateHandleCallback g_PlatformCreateHandleCallback;

/**
 * @brief Global callback pointer for platform device handle invalidation.
 *
 * This callback is invoked to invalidate and clean up platform-specific device handles.
 * Set during initialization via GCH_InitializePlatformBridge.
 */
extern PlatformInvalidateHandleCallback g_PlatformInvalidateHandleCallback;


/**
 * @brief Global callback pointer for platform device feature configuration.
 *
 * This callback is invoked to configure specific features or settings on platform-specific devices.
 * Set during initialization via GCH_InitializePlatformBridge.
 */
extern PlatformConfigureFeaturesCallback g_PlatformConfigureFeaturesCallback;

/**
 * @brief Global callback pointer for platform audio haptic processing.
 *
 * This callback is invoked to process audio-based haptic feedback on platform-specific devices.
 * Set during initialization via GCH_InitializePlatformBridge.
 */
extern PlatformProcessAudioHapticsCallback g_PlatformProcessAudioHapticsCallback;

/**
 * @namespace GCL
 * @brief Gamepad-Core-Log namespace for logging functionality.
 *
 * This namespace encapsulates logging-related functionality for the Gamepad-Core library.
 * It provides a centralized mechanism for the library to communicate log messages to the host
 * application through the configured log callback.
 */
namespace GCL {
    /**
     * @brief Global callback pointer for logging messages from the library.
     *
     * This callback is invoked by the library to send log messages to the host application.
     * It should be set during initialization to enable proper logging integration with the
     * host's logging system. If not set (nullptr), logging will be disabled.
     */
    inline GamepadLogCallback LogCallback = nullptr;

    inline void Log(const int Level, const char *Message) {
#if GCL_DEBUG
        if (LogCallback) {
            LogCallback(Level, Message);
        }
#else
        (void)Level;
        (void)Message;
#endif
    }

    inline void Error(const int Level, const char *Message) {
        if (LogCallback) {
            LogCallback(Level, Message);
        }
    }
}

/**
 * @brief Inline global variable definitions for platform and engine callbacks.
 *
 * These inline global variables provide the actual storage for callback pointers
 * and configuration values declared as extern in the header. They are initialized
 * to nullptr or default values and should be set during library initialization
 * via the appropriate initialization functions (GCH_InitializePlatformBridge and
 * GCH_InitializeDeviceRegistryPolicy).
 */
inline PlatformReadCallback g_PlatformReadCallback = nullptr;
inline PlatformWriteCallback g_PlatformWriteCallback = nullptr;
inline PlatformDetectCallback g_PlatformDetectCallback = nullptr;
inline PlatformCreateHandleCallback g_PlatformCreateHandleCallback = nullptr;
inline PlatformInvalidateHandleCallback g_PlatformInvalidateHandleCallback = nullptr;
inline PlatformConfigureFeaturesCallback g_PlatformConfigureFeaturesCallback = nullptr;
inline PlatformProcessAudioHapticsCallback g_PlatformProcessAudioHapticsCallback = nullptr;


/**
 * @brief Inline global variable storing the Host engine type identifier.
 *
 * This variable holds the type identifier used by the Host engine to classify
 * and manage gamepad devices. It is set during initialization via the
 * GCH_InitializeDeviceRegistryPolicy function and is used internally by the
 * library to associate devices with the correct engine type.
 *
 * The engine type identifier is typically used to distinguish between different
 * engine implementations or device handling strategies within the Host application.
 */
inline int g_EngineTypeId;

/**
 * @brief Inline global callback pointer for allocating new engine devices.
 *
 * This callback is invoked by the library when a new gamepad device is detected
 * and needs to be allocated in the Host engine. The Host application should
 * implement this callback to create a new device instance and return its unique
 * identifier. This identifier will be used in subsequent operations to reference
 * the device.
 *
 * Set during initialization via GCH_InitializeDeviceRegistryPolicy.
 * If not set (nullptr), device allocation will fail and new devices cannot be registered.
 *
 * @see AllocEngineDeviceCallback
 */
inline AllocEngineDeviceCallback g_AllocDeviceCallback = nullptr;

/**
 * @brief Inline global callback pointer for dispatching new gamepad devices to Host.
 *
 * This callback is invoked after a gamepad device has been successfully allocated
 * and initialized by the library. It notifies the Host application that the device
 * is fully configured and ready to be used for input processing. The Host should
 * update its internal device registry and begin accepting input from the device.
 *
 * Set during initialization via GCH_InitializeDeviceRegistryPolicy.
 * If not set (nullptr), device dispatch notifications will be skipped, but the
 * device will still be functional within the library.
 *
 * @see DispatchNewGamepadCallback
 */
inline DispatchNewGamepadCallback g_DispatchDeviceCallback = nullptr;

/**
 * @brief Inline global callback pointer for handling device disconnection events.
 *
 * This callback is invoked when a gamepad device is physically disconnected or
 * becomes unavailable. The Host application should implement this callback to
 * clean up any resources associated with the device, update UI elements, and
 * notify users of the disconnection. The device identifier passed to this callback
 * will no longer be valid after the callback returns.
 *
 * Set during initialization via GCH_InitializeDeviceRegistryPolicy.
 * If not set (nullptr), disconnection events will not be reported to the Host,
 * potentially leading to stale device references.
 *
 * @see DisconnectDeviceCallback
 */
inline DisconnectDeviceCallback g_DisconnectDeviceCallback = nullptr;

#endif //GAMEPADCORE_GAMEPAD_TYPES_API_H
