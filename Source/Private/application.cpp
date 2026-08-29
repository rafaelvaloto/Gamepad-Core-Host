// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.

#include "application.h"

#include "Adapters/DeviceRegistry.h"
#include "Adapters/AudioDeviceRegistry.h"

GCH_API void GCH_Shutdown()
{
	GCH::AudioDeviceRegistry::Shutdown();
	GCH::FDeviceRegistry::Shutdown();
	IPlatformHardware::SetInstance(nullptr);

	g_AllocDeviceCallback = nullptr;
	g_DispatchDeviceCallback = nullptr;
	g_DisconnectDeviceCallback = nullptr;
	g_PlatformReadCallback = nullptr;
	g_PlatformWriteCallback = nullptr;
	g_PlatformDetectCallback = nullptr;
	g_PlatformCreateHandleCallback = nullptr;
	g_PlatformInvalidateHandleCallback = nullptr;
	g_PlatformConfigureFeaturesCallback = nullptr;
	g_PlatformProcessAudioHapticsCallback = nullptr;
	g_EngineTypeId = 0;
	GCL::LogCallback = nullptr;
}

GCH_API const char* GCH_GetVersion()
{
	return GCH_VERSION;
}

GCH_API void GCH_SetLogCallback(const GamepadLogCallback Callback)
{
	GCL::LogCallback = Callback;

	if constexpr (GCL_DEBUG)
	{
		if (GCL::LogCallback)
			GCL::Log(0, "GCH_SetLogCallback registered");
	}
}
