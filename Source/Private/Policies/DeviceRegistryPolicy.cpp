// Project: Gamepad-Core-Host
// Copyright (c) 2026 valoto.games
// All rights reserved.

#include "Policies/DeviceRegistryPolicy.h"
#include "Types/GCHTypes.h"

namespace GCH {
    DeviceRegistryPolicy::EngineIdType DeviceRegistryPolicy::AllocEngineDevice() {
        if (GCL_DEBUG && GCL::LogCallback)
            GCL::Log(1, "AllocEngineDevice");
        return g_AllocDeviceCallback ? g_AllocDeviceCallback() : -1;
    }

    void DeviceRegistryPolicy::DisconnectDevice(EngineIdType id) {
        if (GCL_DEBUG && GCL::LogCallback)
            GCL::Log(1, "DisconnectDevice");
        if (g_DisconnectDeviceCallback)
            g_DisconnectDeviceCallback(id);
    }

    void DeviceRegistryPolicy::DispatchNewGamepad(EngineIdType id) {
        if (GCL_DEBUG && GCL::LogCallback)
            GCL::Log(1, "DispatchNewGamepad");
        if (g_DispatchDeviceCallback)
            g_DispatchDeviceCallback(id);
    }
} // GCH
