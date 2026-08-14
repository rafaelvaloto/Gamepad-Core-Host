// Project: Gamepad-Core-Unity
// Copyright (c) 2026 valoto.games
// All rights reserved.

#include "Adapters/UnityDeviceRegistry.h"
#include "gamepad_unity_api.h"

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
} // GCU
