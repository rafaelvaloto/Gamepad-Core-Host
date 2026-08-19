// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.

#include "Adapters/DeviceRegistry.h"

namespace GCH
{
	std::unique_ptr<FDeviceRegistry> FDeviceRegistry::Instance = nullptr;

	FDeviceRegistry::FDeviceRegistry() = default;

	FDeviceRegistry::~FDeviceRegistry() = default;

	FDeviceRegistry* FDeviceRegistry::Get()
	{
		if (!Instance)
		{
			// this code is inside a member function of FDeviceRegistry.
			Instance.reset(new FDeviceRegistry());
		}

		return Instance.get();
	}

	void FDeviceRegistry::Shutdown()
	{
		Instance.reset();
	}
} // GCH
