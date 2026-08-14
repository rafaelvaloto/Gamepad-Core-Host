// Project: Gamepad-Core-Unity
// Copyright (c) 2026 valoto.games
// All rights reserved.

#include "Adapters/UnityDeviceRegistry.h"

void GCU_DiscoverDevices(const float DeltaTime) {
    GCU::FUnityDeviceRegistry::Get()->PlugAndPlay(DeltaTime);
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
