// Project: Gamepad-Core-Unity
// Copyright (c) 2026 valoto.games
// All rights reserved.

#include "Policies/UnityDeviceRegistryPolicy.h"
#include "gamepad_unity_api.h"
#include "Adapters/UnityDeviceRegistry.h"

int g_UnityEngineTypeId = 0;
AllocEngineDeviceCallback g_UnityAllocDeviceCallback = nullptr;
DispatchNewGamepadCallback g_UnityDispatchDeviceCallback = nullptr;
DisconnectDeviceCallback g_UnityDisconnectDeviceCallback = nullptr;

namespace GCU {
    UnityDeviceRegistryPolicy::EngineIdType UnityDeviceRegistryPolicy::AllocEngineDevice() {
        return g_UnityAllocDeviceCallback ? g_UnityAllocDeviceCallback() : -1;
    }

    void UnityDeviceRegistryPolicy::DisconnectDevice(EngineIdType id) {
        if (g_UnityDisconnectDeviceCallback)
            g_UnityDisconnectDeviceCallback(id);
    }

    void UnityDeviceRegistryPolicy::DispatchNewGamepad(EngineIdType id) {
        if (g_UnityDispatchDeviceCallback)
            g_UnityDispatchDeviceCallback(id);
    }
} // GCU

void GCU_InitializeDeviceRegistryPolicy(const int TypeId, const AllocEngineDeviceCallback allocCallback,
    const DispatchNewGamepadCallback dispatchCallback, const DisconnectDeviceCallback disconnectCallback) {
    g_UnityEngineTypeId = TypeId;
    g_UnityAllocDeviceCallback = allocCallback;
    g_UnityDispatchDeviceCallback = dispatchCallback;
    g_UnityDisconnectDeviceCallback = disconnectCallback;
    GCU::FUnityDeviceRegistry::Initialize();
}
