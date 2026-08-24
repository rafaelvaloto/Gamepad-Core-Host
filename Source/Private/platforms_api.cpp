// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.

#include "platforms_api.h"
#include "Adapters/DeviceRegistry.h"
#include "GCore/Templates/TGenericHardwareInfo.h"
#include "Policies/PlatformBridgePolicy.h"

GCH_API void GCH_InitializePlatformBridge(
	const PlatformReadCallback ReadCallback,
	const PlatformWriteCallback WriteCallback,
	const PlatformDetectCallback DetectCallback,
	const PlatformCreateHandleCallback CreateHandleCallback,
	const PlatformInvalidateHandleCallback InvalidateHandleCallback,
	const PlatformConfigureFeaturesCallback ConfigureFeaturesCallback,
	const PlatformProcessAudioHapticsCallback ProcessAudioHapticsCallback)
{
	g_PlatformReadCallback = ReadCallback;
	g_PlatformWriteCallback = WriteCallback;
	g_PlatformDetectCallback = DetectCallback;
	g_PlatformCreateHandleCallback = CreateHandleCallback;
	g_PlatformInvalidateHandleCallback = InvalidateHandleCallback;
	g_PlatformConfigureFeaturesCallback = ConfigureFeaturesCallback;
	g_PlatformProcessAudioHapticsCallback = ProcessAudioHapticsCallback;

	IPlatformHardware::SetInstance(
		std::make_unique<GamepadCore::TGenericHardwareInfo<GCH::PlatformBridgePolicy> >());

	(void)GCH::FDeviceRegistry::Get();
}

GCH_API void GCH_InitializeDeviceRegistryPolicy(
	const int TypeId,
	const AllocEngineDeviceCallback AllocCallback,
	const DispatchNewGamepadCallback DispatchCallback,
	const DisconnectDeviceCallback DisconnectCallback)
{
	g_EngineTypeId = TypeId;
	g_AllocDeviceCallback = AllocCallback;
	g_DispatchDeviceCallback = DispatchCallback;
	g_DisconnectDeviceCallback = DisconnectCallback;
}

#if defined(__EMSCRIPTEN__)
namespace
{
	template <typename TCallback>
	TCallback FromWasmTablePointer(const std::uintptr_t Pointer)
	{
		return reinterpret_cast<TCallback>(Pointer);
	}
}

GCH_API void GCH_InitializePlatformBridgeWasm(
	const std::uintptr_t ReadCallbackPtr,
	const std::uintptr_t WriteCallbackPtr,
	const std::uintptr_t DetectCallbackPtr,
	const std::uintptr_t CreateHandleCallbackPtr,
	const std::uintptr_t InvalidateHandleCallbackPtr,
	const std::uintptr_t ConfigureFeaturesCallbackPtr,
	const std::uintptr_t ProcessAudioHapticsCallbackPtr)
{
	GCH_InitializePlatformBridge(
		FromWasmTablePointer<PlatformReadCallback>(ReadCallbackPtr),
		FromWasmTablePointer<PlatformWriteCallback>(WriteCallbackPtr),
		FromWasmTablePointer<PlatformDetectCallback>(DetectCallbackPtr),
		FromWasmTablePointer<PlatformCreateHandleCallback>(CreateHandleCallbackPtr),
		FromWasmTablePointer<PlatformInvalidateHandleCallback>(InvalidateHandleCallbackPtr),
		FromWasmTablePointer<PlatformConfigureFeaturesCallback>(ConfigureFeaturesCallbackPtr),
		FromWasmTablePointer<PlatformProcessAudioHapticsCallback>(ProcessAudioHapticsCallbackPtr));
}

GCH_API void GCH_InitializeDeviceRegistryPolicyWasm(
	const int TypeId,
	const std::uintptr_t AllocCallbackPtr,
	const std::uintptr_t DispatchCallbackPtr,
	const std::uintptr_t DisconnectCallbackPtr)
{
	GCH_InitializeDeviceRegistryPolicy(
		TypeId,
		FromWasmTablePointer<AllocEngineDeviceCallback>(AllocCallbackPtr),
		FromWasmTablePointer<DispatchNewGamepadCallback>(DispatchCallbackPtr),
		FromWasmTablePointer<DisconnectDeviceCallback>(DisconnectCallbackPtr));
}
#endif
