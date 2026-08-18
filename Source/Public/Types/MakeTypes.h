// Project: Gamepad-Core-Host
// Copyright (c) 2026 valoto.games
// All rights reserved.

#pragma once
#include "GCHTypes.h"
#include "GCore/Types/Structs/Context/DeviceContext.h"

namespace GCH {
    FDeviceContext MakeDeviceContext(const GamepadDeviceDescriptor &Descriptor);

    GamepadDeviceDescriptor MakeDeviceDescriptor(FDeviceContext* Context);
}
