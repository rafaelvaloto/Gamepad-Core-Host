// Project: Gamepad-Core-Unity
// Copyright (c) 2026 valoto.games
// All rights reserved.
#pragma once

#include "GCore/Types/Structs/Context/DeviceContext.h"

namespace GCU {
    class PlatformBridgePolicy {
    public:
        static void Read(FDeviceContext* Context);
        static void Write(FDeviceContext* Context);
        static void Detect(std::vector<FDeviceContext>& Devices);
        static bool CreateHandle(FDeviceContext* Context);
        static void InvalidateHandle(FDeviceContext* Context);
        static void ProcessAudioHaptic(FDeviceContext* Context);
    };
} // namespace GCU
