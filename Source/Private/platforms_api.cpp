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
        std::make_unique<GamepadCore::TGenericHardwareInfo<GCH::PlatformBridgePolicy>>());

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
