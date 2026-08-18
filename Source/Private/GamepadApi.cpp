// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.

#include "GamepadApi.h"
#include <cstdio>
#include <cstring>
#include <memory>

#include "Adapters/DeviceRegistry.h"
#include "GCore/Templates/TGenericHardwareInfo.h"
#include "Policies/PlatformBridgePolicy.h"
#include "Types/MakeTypes.h"

extern "C" {

GCH_API void GCH_Shutdown() {
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
    g_PlatformProcessAudioHapticsCallback = nullptr;
    g_EngineTypeId = 0;
    GCL::LogCallback = nullptr;
}

GCH_API const char *GCH_GetVersion() { return GCH_VERSION; }

GCH_API void GCH_SetLogCallback(const GamepadLogCallback Callback) {
    GCL::LogCallback = Callback;
    if (GCL_DEBUG && GCL::LogCallback)
        GCL::Log(0, "GCH_SetLogCallback registered");
}

GCH_API void GCH_InitializeDeviceRegistryPolicy(const int TypeId,
                                                              const AllocEngineDeviceCallback allocCallback,
                                                              const DispatchNewGamepadCallback dispatchCallback,
                                                              const DisconnectDeviceCallback disconnectCallback) {
    g_EngineTypeId = TypeId;
    g_AllocDeviceCallback = allocCallback;
    g_DispatchDeviceCallback = dispatchCallback;
    g_DisconnectDeviceCallback = disconnectCallback;
}

GCH_API void GCH_InitializePlatformBridge(const PlatformReadCallback readCallback,
                                                        const PlatformWriteCallback writeCallback,
                                                        const PlatformDetectCallback detectCallback,
                                                        const PlatformCreateHandleCallback createHandleCallback,
                                                        const PlatformInvalidateHandleCallback invalidateHandleCallback,
                                                        const PlatformConfigureFeaturesCallback configureFeaturesCallback,
                                                        const PlatformProcessAudioHapticsCallback processAudioHapticsCallback) {
    g_PlatformReadCallback = readCallback;
    g_PlatformWriteCallback = writeCallback;
    g_PlatformDetectCallback = detectCallback;
    g_PlatformCreateHandleCallback = createHandleCallback;
    g_PlatformInvalidateHandleCallback = invalidateHandleCallback;
    g_PlatformConfigureFeaturesCallback = configureFeaturesCallback;
    g_PlatformProcessAudioHapticsCallback = processAudioHapticsCallback;

    IPlatformHardware::SetInstance(std::make_unique<GamepadCore::TGenericHardwareInfo<GCH::PlatformBridgePolicy> >());

    // Get() creates the singleton once and keeps ownership in FDeviceRegistry::Instance.
    (void)GCH::FDeviceRegistry::Get();
}

GCH_API void GCH_DiscoverDevices(const float DeltaTime) {
    if (auto *Registry = GCH::FDeviceRegistry::Get()) Registry->PlugAndPlay(DeltaTime);
}

GCH_API void GCH_UpdateInput(const int DeviceId, const float DeltaTime) {

    auto *Registry = GCH::FDeviceRegistry::Get();
    if (!Registry) {
        GCL::Error(0, "not registry GCH::FDeviceRegistry::Get()");
        return;
    }

    auto *Gamepad = Registry->GetLibrary(DeviceId);
    if (!Gamepad) {
        char Message[128];
        std::snprintf(Message, sizeof(Message), "not gamepad Library GetLibrary %d", DeviceId);
        GCL::Error(0, Message);
        return;
    };

    Gamepad->UpdateInput(0.016f);
}

GCH_API bool GCH_GetInputState(const int DeviceId, FInputContext *OutInputState) {
    if (!OutInputState) {
        GCL::Error(0, "[Error]: OutInputState is null");
        return false;
    }

    auto *Registry = GCH::FDeviceRegistry::Get();
    if (!Registry) {
        GCL::Error(0, "[Error]: Not registry GCH::FDeviceRegistry::Get()");
        return false;
    }

    auto *Gamepad = Registry->GetLibrary(DeviceId);
    if (!Gamepad) {
        char Message[128];
        std::snprintf(Message, sizeof(Message), "[Error]: Not gamepad library %d", DeviceId);
        GCL::Error(0, Message);
        return false;
    };

    auto *DeviceContext = Gamepad->GetMutableDeviceContext();
    if (!DeviceContext) {
        char Message[128];
        std::snprintf(Message, sizeof(Message), "[Error]: Not device context %d", DeviceId);
        GCL::Error(0, Message);
        return false;
    }

    // copy buffer.
    const FInputContext *InputContext = DeviceContext->GetInputState();
    *OutInputState = *InputContext;

    char MessageInputs[128];
    std::snprintf(
        MessageInputs, sizeof(MessageInputs),
        "OutInputState buttons: Cross=%d, Circle=%d",
        OutInputState->bCross,
        OutInputState->bCircle
    );
    GCL::Log(1, MessageInputs);

    return true;
}

}
