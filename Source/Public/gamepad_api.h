// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.
#pragma once

#include "application.h"
#include "GCore/Types/Structs/Context/InputContext.h"

extern "C"
{

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
 * Updates the output state of a specific gamepad controller.
 *
 * This function retrieves the gamepad associated with the provided controller ID
 * and updates its output state. If the specified gamepad is not found, an error
 * message is logged.
 *
 * @param ControllerId The identifier of the gamepad controller whose output state
 *                     is to be updated.
 */
GCH_API void GCH_UpdateOutput(int ControllerId);

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
GCH_API bool GCH_GetInputState(int DeviceId, FInputContext* OutInputState);

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


/**
 * Checks whether a specific gamepad device is currently connected.
 *
 * This method verifies the connection status of the gamepad identified by
 * the provided controller ID. It can be used to determine if the device
 * is available for input processing and communication.
 *
 * @param ControllerId The unique identifier of the gamepad device to check.
 * @return true if the device is connected, false otherwise.
 */
GCH_API bool GCH_DeviceIsConnected(int ControllerId);

/**
 * Retrieves the device type of a specific gamepad.
 *
 * This method returns an identifier representing the type of gamepad device,
 * such as DualSense, DualShock, Xbox controller, or generic gamepad.
 *
 * @param ControllerId The unique identifier of the gamepad device.
 * @return An integer representing the device type, or an error code if the
 *         device ID is invalid or the device is not connected.
 */
GCH_API int GCH_GetDeviceType(int ControllerId);

/**
 * Retrieves the connection type of a specific gamepad device.
 *
 * This method returns an identifier representing how the gamepad is connected,
 * such as USB, Bluetooth, or wireless dongle.
 *
 * @param ControllerId The unique identifier of the gamepad device.
 * @return An integer representing the connection type, or an error code if the
 *         device ID is invalid or the device is not connected.
 */
GCH_API int GCH_GetConnectionType(int ControllerId);

/**
 * Retrieves the current battery level of a specific gamepad device.
 *
 * This method returns the battery charge level as a percentage value.
 * For devices connected via USB or those without battery monitoring,
 * the return value may indicate a full charge or an invalid state.
 *
 * @param ControllerId The unique identifier of the gamepad device.
 * @return A float value representing the battery level as a percentage (0.0 to 100.0),
 *         or a negative value if the battery level cannot be determined.
 */
GCH_API float GCH_BatteryLevelDevice(int ControllerId);

/**
 * Sets the color of the lightbar on a specific gamepad device.
 *
 * This method configures the RGB color values for the gamepad's lightbar,
 * allowing customization of the visual feedback. The lightbar feature is
 * typically available on controllers like the DualSense or DualShock 4.
 *
 * @param ControllerId The unique identifier of the gamepad device.
 * @param R The red component of the color (0-255).
 * @param G The green component of the color (0-255).
 * @param B The blue component of the color (0-255).
 */
GCH_API void GCH_Lightbar(int ControllerId, std::uint8_t R, std::uint8_t G, std::uint8_t B);

/**
 * Configures a flashing effect for the lightbar on a specific gamepad device.
 *
 * This method sets the lightbar to flash with the specified RGB color and timing
 * parameters. The flashing effect alternates between the specified color and off state.
 *
 * @param ControllerId The unique identifier of the gamepad device.
 * @param R The red component of the color (0-255).
 * @param G The green component of the color (0-255).
 * @param B The blue component of the color (0-255).
 * @param BrightnessTime The duration in seconds that the lightbar remains on during each cycle.
 * @param ToggleTime The duration in seconds that the lightbar remains off during each cycle.
 */
GCH_API void GCH_LightbarFlash(int ControllerId, std::uint8_t R, std::uint8_t G, std::uint8_t B, float BrightnessTime, float ToggleTime);

/**
 * Controls the player indicator LED on a specific gamepad device.
 *
 * This method sets the state and brightness of individual player indicator LEDs,
 * typically used to identify which player is using the controller. The number
 * and behavior of LEDs vary by device type.
 *
 * @param ControllerId The unique identifier of the gamepad device.
 * @param Led The index of the LED to control (device-specific).
 * @param Brightness The brightness level of the LED (0-255, where 0 is off).
 */
GCH_API void GCH_PlayerLed(int ControllerId, int Led, std::uint8_t Brightness);

/**
 * Resets the light settings of the specified game controller.
 *
 * This method retrieves the gamepad associated with the given controller ID,
 * accesses its lightbar component (if available), and resets the lightbar to
 * its default state. It is useful for restoring the light settings of game
 * controllers during gameplay or when reinitializing devices.
 *
 * @param ControllerId The identifier of the game controller whose lightbar
 *                     should be reset. This ID is used to locate the
 *                     corresponding gamepad in the device registry.
 */
GCH_API void GCH_ResetLights(int ControllerId);

/**
 * Configures DualSense-specific settings for a specified gamepad controller.
 *
 * This method updates various audio and haptic feedback settings for
 * a DualSense controller. It applies microphone, headset, and speaker
 * configurations, as well as rumble modes and reductions, for the
 * designated controller.
 *
 * @param ControllerId The unique identifier of the gamepad controller
 *                     to configure.
 * @param bIsMic       Indicates whether the microphone is enabled (1)
 *                     or disabled (0).
 * @param bIsHeadset   Indicates whether the headset is connected (1)
 *                     or not (0).
 * @param bIsSpeaker   Indicates whether the speaker is in use (1)
 *                     or not (0).
 * @param MicVolume    The desired microphone volume level.
 * @param AudioVolume  The desired audio output volume level.
 * @param RumbleMode   Specifies the operational mode for haptic feedback (rumble).
 * @param RumbleReduce Adjusts the intensity of the rumble effect.
 * @param TriggerReduce Adjusts the resistance of the adaptive triggers.
 */
GCH_API void GCH_DualSenseSettings(int ControllerId,
                                   std::uint8_t bIsMic,
                                   std::uint8_t bIsHeadset,
                                   std::uint8_t bIsSpeaker,
                                   std::uint8_t MicVolume,
                                   std::uint8_t AudioVolume,
                                   std::uint8_t RumbleMode,
                                   std::uint8_t RumbleReduce,
                                   std::uint8_t TriggerReduce);

/**
 * Resets the gyroscope orientation to its default state for a specific gamepad device.
 *
 * This method recalibrates the gyroscope sensor, setting the current physical
 * orientation as the new reference point. This is useful for correcting drift
 * or establishing a new baseline orientation.
 *
 * @param ControllerId The unique identifier of the gamepad device.
 */
GCH_API void GCH_ResetGyroOrientation(int ControllerId);

/**
 * Enables or disables gyroscope data collection for a specific gamepad device.
 *
 * This method controls whether the gyroscope sensor is active and providing
 * orientation data. Disabling the gyroscope can save power and processing resources
 * when motion sensing is not needed.
 *
 * @param ControllerId The unique identifier of the gamepad device.
 * @param EnableGyroscope true to enable gyroscope data collection, false to disable it.
 */
GCH_API void GCH_EnableGyroscopeValues(int ControllerId, bool EnableGyroscope);

/**
 * Enables or disables touchpad input for a specific gamepad device.
 *
 * This method controls whether the touchpad is active and providing touch
 * input data. Disabling the touchpad can save processing resources when
 * touch input is not needed.
 *
 * @param ControllerId The unique identifier of the gamepad device.
 * @param EnableTouch true to enable touchpad input, false to disable it.
 */
GCH_API void GCH_EnableTouch(int ControllerId, bool EnableTouch);

/**
 * Applies a custom adaptive trigger effect to a specific gamepad device.
 *
 * This method configures the adaptive trigger with custom parameters provided
 * as raw byte data. This allows for precise control over trigger resistance
 * and feedback effects, particularly on devices like the DualSense controller.
 *
 * @param ControllerId The unique identifier of the gamepad device.
 * @param HexBytes A pointer to an array of bytes containing the custom trigger configuration data.
 * @param ByteCount The number of bytes in the configuration data array.
 * @param Hand Specifies which trigger to configure (0 for left, 1 for right).
 * @return true if the custom trigger effect was successfully applied, false otherwise.
 */
GCH_API bool GCH_CustomTrigger(int ControllerId, const std::uint8_t* HexBytes, int ByteCount, int Hand);

/**
 * Stops and resets the adaptive trigger effect on a specific gamepad device.
 *
 * This method disables any active adaptive trigger effects and returns the
 * trigger to its default state with no resistance or feedback.
 *
 * @param ControllerId The unique identifier of the gamepad device.
 * @param Hand Specifies which trigger to reset (0 for left, 1 for right).
 */
GCH_API void GCH_StopTrigger(int ControllerId, int Hand);

/**
 * Sets the vibration intensity for the rumble motors on a specific gamepad device.
 *
 * This method controls the intensity of both the left and right rumble motors,
 * providing haptic feedback to the user. The intensity is specified as a value
 * from 0 (no vibration) to 255 (maximum vibration).
 *
 * @param ControllerId The unique identifier of the gamepad device.
 * @param LeftRumble The intensity of the left rumble motor (0-255).
 * @param RightRumble The intensity of the right rumble motor (0-255).
 */
GCH_API void GCH_SetVibration(int ControllerId, std::uint8_t LeftRumble, std::uint8_t RightRumble);


}
