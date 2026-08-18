// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.
#pragma once

#include "application.h"
#include "GCore/Types/Structs/Context/InputContext.h"

extern "C" {

/**
 * Discovers and initializes connected gamepad devices.
 *
 * This method scans for available gamepad devices and prepares them for use.
 * It should be called periodically to detect newly connected devices or update
 * the status of existing ones.
 *
 * @param DeltaTime The time elapsed since the last frame, in seconds. This parameter
 *                  is used for timing-sensitive operations during device discovery.
 */
GCH_API void GCH_DiscoverDevices(float DeltaTime);

/**
 * Updates the input state for a specific gamepad device.
 *
 * This method processes input events and updates the internal state of the specified
 * gamepad based on user interactions or automated inputs. It should be called each
 * frame to ensure input data remains current.
 *
 * @param DeviceId The unique identifier of the gamepad device to update.
 * @param DeltaTime The time elapsed since the last frame, in seconds. This parameter
 *                  is used for timing-sensitive input processing.
 */
GCH_API void GCH_UpdateInput(int DeviceId, float DeltaTime);

/**
 * Retrieves the current input state of a specific gamepad device.
 *
 * This method provides access to the current state of buttons, analog sticks,
 * triggers, and other input elements of the specified gamepad.
 *
 * @param DeviceId The unique identifier of the gamepad device.
 * @param OutInputState A pointer to an FInputContext structure where the current
 *                      input state will be written. Must not be nullptr.
 * @return true if the input state was successfully retrieved, false otherwise
 *         (e.g., if the device ID is invalid or the device is not connected).
 */
GCH_API bool GCH_GetInputState(int DeviceId, FInputContext *OutInputState);

/**
 * Retrieves the device descriptor for a specific gamepad.
 *
 * This method provides information about the gamepad device, such as its type,
 * connection method, capabilities, and other hardware-specific details.
 *
 * @param DeviceId The unique identifier of the gamepad device.
 * @param OutDescriptor A pointer to a GamepadDeviceDescriptor structure where the
 *                      device information will be written. Must not be nullptr.
 * @return true if the device descriptor was successfully retrieved, false otherwise
 *         (e.g., if the device ID is invalid or the device is not connected).
 */
GCH_API bool GCH_GetDeviceDescriptor(int DeviceId, GamepadDeviceDescriptor* OutDescriptor);

}
