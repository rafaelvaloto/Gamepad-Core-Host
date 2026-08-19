// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.
#pragma once
#include "application.h"

extern "C"
{
/**
 * Initializes the device registry policy by setting the engine type ID and callback functions
 * for device allocation, gamepad dispatch, and device disconnection.
 *
 * @param TypeId An integer representing the engine type ID.
 * @param AllocCallback A callback function for allocating a new engine device.
 * @param DispatchCallback A callback function for dispatching new gamepad devices.
 * @param DisconnectCallback A callback function for handling device disconnection events.
 */
GCH_API void GCH_InitializeDeviceRegistryPolicy(
	int TypeId,
	AllocEngineDeviceCallback AllocCallback,
	DispatchNewGamepadCallback DispatchCallback,
	DisconnectDeviceCallback DisconnectCallback);


/**
 * Initializes the platform bridge by assigning callback functions for platform-level
 * operations and setting up the platform hardware instance.
 *
 * @param ReadCallback A callback function for reading data from a platform handle.
 * @param WriteCallback A callback function for writing data to a platform handle.
 * @param DetectCallback A callback function for detecting connected devices.
 * @param CreateHandleCallback A callback function for creating a platform-specific handle for a device.
 * @param InvalidateHandleCallback A callback function for invalidating a platform device handle.
 * @param ConfigureFeaturesCallback A callback function for configuring platform-specific features.
 * @param ProcessAudioHapticsCallback A callback function for processing audio-based haptics data.
 */
GCH_API void GCH_InitializePlatformBridge(
	PlatformReadCallback ReadCallback,
	PlatformWriteCallback WriteCallback,
	PlatformDetectCallback DetectCallback,
	PlatformCreateHandleCallback CreateHandleCallback,
	PlatformInvalidateHandleCallback InvalidateHandleCallback,
	PlatformConfigureFeaturesCallback ConfigureFeaturesCallback,
	PlatformProcessAudioHapticsCallback ProcessAudioHapticsCallback);

}
