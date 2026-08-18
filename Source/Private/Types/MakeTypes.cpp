// Project: Gamepad-Core-Host
// Copyright (c) 2026 valoto.games
// All rights reserved.

#include "Types/MakeTypes.h"
#include <algorithm>
#include <cstring>

namespace GCH {
    FDeviceContext MakeDeviceContext(const GamepadDeviceDescriptor &Descriptor){
        FDeviceContext Context{};
        Context.Handle = reinterpret_cast<FPlatformDeviceHandle>(Descriptor.Handle);
        Context.Path = Descriptor.Path;
        Context.IsConnected = Descriptor.IsConnected;
        Context.DeviceType = static_cast<EDSDeviceType>(Descriptor.DeviceType);
        Context.ConnectionType = static_cast<EDSDeviceConnection>(Descriptor.ConnectionType);

        return Context;
    }

    GamepadDeviceDescriptor MakeDeviceDescriptor(FDeviceContext *Context) {
        GamepadDeviceDescriptor Descriptor{};

        Descriptor.Handle = reinterpret_cast<std::uint64_t>(Context->Handle);
        Descriptor.IsConnected = static_cast<std::int32_t>(Context->IsConnected);
        Descriptor.DeviceType = static_cast<std::int32_t>(Context->DeviceType);
        Descriptor.ConnectionType = static_cast<std::int32_t>(Context->ConnectionType);

        const std::size_t PathLength = std::min(Context->Path.size(), sizeof(Descriptor.Path) - 1);
        std::memcpy(Descriptor.Path, Context->Path.data(), PathLength);
        Descriptor.Path[PathLength] = '\0';

        return Descriptor;
    }
}
