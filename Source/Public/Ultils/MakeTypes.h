// Project: Gamepad-Core-Unity
// Copyright (c) 2026 valoto.games
// All rights reserved.

#pragma once
#include "gamepad_types_api.h"
#include "GCore/Types/Structs/Context/DeviceContext.h"

namespace GCU {
    FDeviceContext MakeDeviceContext(const GCUDeviceDescriptor &Descriptor);

    GCUDeviceDescriptor MakeDeviceDescriptor(FDeviceContext* Context);
}
