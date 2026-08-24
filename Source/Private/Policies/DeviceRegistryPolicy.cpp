// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.

#include "Policies/DeviceRegistryPolicy.h"
#include "Types/GCHTypes.h"

namespace GCH
{
	DeviceRegistryPolicy::EngineIdType DeviceRegistryPolicy::AllocEngineDevice()
	{
		if constexpr (GCL_DEBUG)
		{
			char Message[128];
			std::snprintf(Message, sizeof(Message), "AllocEngineDevice");
			GCL::Log(1, Message);
		}

		if (g_PendingDeviceId >= 0)
		{
			const EngineIdType Id = g_PendingDeviceId;
			g_PendingDeviceId = -1;
			return Id;
		}

		return g_AllocDeviceCallback ? g_AllocDeviceCallback() : -1;
	}

	void DeviceRegistryPolicy::DisconnectDevice(EngineIdType id)
	{
		if constexpr (GCL_DEBUG)
		{
			char Message[128];
			std::snprintf(Message, sizeof(Message), "DisconnectDevice: %d", id);
			GCL::Log(1, Message);
		}

		if (g_DisconnectDeviceCallback)
			g_DisconnectDeviceCallback(id);
	}

	void DeviceRegistryPolicy::DispatchNewGamepad(EngineIdType id)
	{
		if constexpr (GCL_DEBUG)
		{
			char Message[128];
			std::snprintf(Message, sizeof(Message), "DispatchNewGamepad: %d", id);
			GCL::Log(1, Message);
		}

		if (g_DispatchDeviceCallback)
			g_DispatchDeviceCallback(id);
	}
} // GCH
