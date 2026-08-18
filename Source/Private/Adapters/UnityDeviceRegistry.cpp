// Project: Gamepad-Core-Unity
// Copyright (c) 2026 valoto.games
// All rights reserved.

#include "Adapters/UnityDeviceRegistry.h"

namespace GCU {
    std::unique_ptr<FUnityDeviceRegistry> FUnityDeviceRegistry::Instance = nullptr;

    FUnityDeviceRegistry::FUnityDeviceRegistry() = default;

    FUnityDeviceRegistry::~FUnityDeviceRegistry() = default;

    FUnityDeviceRegistry* FUnityDeviceRegistry::Get() {
        if (!Instance) {
            // this code is inside a member function of FUnityDeviceRegistry.
            Instance.reset(new FUnityDeviceRegistry());
        }

        return Instance.get();
    }

    void FUnityDeviceRegistry::Shutdown() {
        Instance.reset();
    }
} // GCU
