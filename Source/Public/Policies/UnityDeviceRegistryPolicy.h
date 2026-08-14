// Project: Gamepad-Core-Unity
// Copyright (c) 2026 valoto.games
// All rights reserved.
#pragma once
#include <functional>

namespace GCU {
    struct UnityDeviceRegistryPolicy {
        using EngineIdType = int;

        static EngineIdType AllocEngineDevice();
        static void DisconnectDevice(EngineIdType id);
        static void DispatchNewGamepad(EngineIdType id);

        struct Hasher
        {
            size_t operator()(const EngineIdType& id) const { return std::hash<EngineIdType>{}(id); }
        };
    };
} // GCU
