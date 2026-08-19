// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.
#pragma once
#include "GCore/Templates/TBasicDeviceRegistry.h"
#include "Policies/DeviceRegistryPolicy.h"
#include <memory>

namespace GCH
{
	class FDeviceRegistry : public GamepadCore::TBasicDeviceRegistry<DeviceRegistryPolicy>
	{
	public:
		~FDeviceRegistry() override;

		/**
		 * Shuts down the static instance of the FDeviceRegistry class, releasing the memory
		 * associated with it. This method ensures that the singleton instance is properly cleaned
		 * up when it is no longer needed, preventing potential memory leaks or lingering references.
		 * Should be called during the application's shutdown sequence to free resources.
		 */
		static void Shutdown();

		/**
		 * Retrieves the static instance of the FDeviceRegistry class. This method
		 * ensures that only a single instance of the manager class is created and provides
		 * global access to it for managing device library instances of Sony gamepad controllers.
		 *
		 * @return A pointer to the singleton instance of UDeviceContainerManager. Returns nullptr
		 *         if the instance has not been initialized.
		 */
		static FDeviceRegistry* Get();

	private:
		FDeviceRegistry();

		static std::unique_ptr<FDeviceRegistry> Instance;
	};
} // GCH
