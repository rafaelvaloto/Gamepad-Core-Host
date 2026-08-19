// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.
#pragma once
#include <functional>

namespace GCH
{
	struct DeviceRegistryPolicy
	{
		using EngineIdType = int;

		static EngineIdType AllocEngineDevice();

		static void DisconnectDevice(EngineIdType id);

		static void DispatchNewGamepad(EngineIdType id);

		struct Hasher
		{
			size_t operator()(const EngineIdType& id) const { return std::hash<EngineIdType>{}(id); }
		};
	};
} // GCH
