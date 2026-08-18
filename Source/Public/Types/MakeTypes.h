// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.

#pragma once
#include "GCHTypes.h"
#include "GCore/Types/Structs/Context/DeviceContext.h"

namespace GCH {
    FDeviceContext MakeDeviceContext(const GamepadDeviceDescriptor &Descriptor);

    GamepadDeviceDescriptor MakeDeviceDescriptor(FDeviceContext* Context);
}
