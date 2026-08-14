// Project: Gamepad-Core-Unity
// Copyright (c) 2026 valoto.games
// All rights reserved.

#include "Adapters/UnityDeviceRegistry.h"
#include "gamepad_unity_api.h"
#include "GCore/Interfaces/IPlatformHardware.h"

#ifndef GCU_VERSION_STRING
#define GCU_VERSION_STRING "1.0.0"
#endif

namespace {
    constexpr char GCU_VERSION[] = GCU_VERSION_STRING;
    GCU_LogCallback LogCallback = nullptr;
}

void GCU_Shutdown() {
    GCU::FUnityDeviceRegistry::Shutdown();
    IPlatformHardware::SetInstance(nullptr);

    g_UnityAllocDeviceCallback = nullptr;
    g_UnityDispatchDeviceCallback = nullptr;
    g_UnityDisconnectDeviceCallback = nullptr;
    g_UnityPlatformReadCallback = nullptr;
    g_UnityPlatformWriteCallback = nullptr;
    g_UnityPlatformDetectCallback = nullptr;
    g_UnityPlatformCreateHandleCallback = nullptr;
    g_UnityPlatformInvalidateHandleCallback = nullptr;
    g_UnityPlatformProcessAudioHapticCallback = nullptr;
    g_UnityEngineTypeId = 0;
    LogCallback = nullptr;
}

const char* GCU_GetVersion() {
    return GCU_VERSION;
}

void GCU_SetLogCallback(const GCU_LogCallback Callback) {
    LogCallback = Callback;
}

void GCU_DiscoverDevices(const float DeltaTime) {
    if (GCU::FUnityDeviceRegistry* Registry = GCU::FUnityDeviceRegistry::Get()) {
        Registry->PlugAndPlay(DeltaTime);
    }
}

void GCU_UpdateInput(const int DeviceId, const float DeltaTime) {
    GCU::FUnityDeviceRegistry* Registry = GCU::FUnityDeviceRegistry::Get();
    if (!Registry) {
        return;
    }

    if (IGamepadBase* Gamepad = Registry->GetLibrary(DeviceId)) {
        Gamepad->UpdateInput(DeltaTime);
    }
}

bool GCU_GetInputState(const int DeviceId, FInputContext* InputState) {
    if (!InputState) {
        return false;
    }

    GCU::FUnityDeviceRegistry* Registry = GCU::FUnityDeviceRegistry::Get();
    if (!Registry) {
        return false;
    }

    IGamepadBase* Gamepad = Registry->GetLibrary(DeviceId);
    if (!Gamepad) {
        return false;
    }

    FDeviceContext* DeviceContext = Gamepad->GetMutableDeviceContext();
    if (!DeviceContext) {
        return false;
    }

    *InputState = *DeviceContext->GetInputState();
    return true;
}

namespace GCU {

    namespace {
        std::unique_ptr<FUnityDeviceRegistry> Instance;
    }

    FUnityDeviceRegistry::FUnityDeviceRegistry() = default;

    FUnityDeviceRegistry::~FUnityDeviceRegistry() = default;

    void FUnityDeviceRegistry::Initialize() {
        if (!Instance)
            Instance = std::unique_ptr<FUnityDeviceRegistry>(new FUnityDeviceRegistry());
    }

    FUnityDeviceRegistry* FUnityDeviceRegistry::Get() {
        return Instance.get();
    }

    void FUnityDeviceRegistry::Shutdown() {
        Instance.reset();
    }
} // GCU
