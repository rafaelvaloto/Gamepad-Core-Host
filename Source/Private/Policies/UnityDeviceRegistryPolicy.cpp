// Project: Gamepad-Core-Unity
// Copyright (c) 2026 valoto.games
// All rights reserved.

#include "Policies/UnityDeviceRegistryPolicy.h"
#include "gamepad_types_api.h"

namespace GCU {
    UnityDeviceRegistryPolicy::EngineIdType UnityDeviceRegistryPolicy::AllocEngineDevice() {
        if (GCL::LogCallback)
            GCL::LogCallback(1, "AllocEngineDevice");
        return g_UnityAllocDeviceCallback ? g_UnityAllocDeviceCallback() : -1;
    }

    void UnityDeviceRegistryPolicy::DisconnectDevice(EngineIdType id) {
        if (GCL::LogCallback)
            GCL::LogCallback(1, "DisconnectDevice");
        if (g_UnityDisconnectDeviceCallback)
            g_UnityDisconnectDeviceCallback(id);
    }

    void UnityDeviceRegistryPolicy::DispatchNewGamepad(EngineIdType id) {
        if (GCL::LogCallback)
            GCL::LogCallback(1, "DispatchNewGamepad");
        if (g_UnityDispatchDeviceCallback)
            g_UnityDispatchDeviceCallback(id);
    }
} // GCU
