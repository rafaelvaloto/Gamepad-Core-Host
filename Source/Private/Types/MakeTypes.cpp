// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.

#include <algorithm>
#include <cstring>
#include "Types/MakeTypes.h"

namespace GCH
{
	FDeviceContext MakeDeviceContext(const GamepadDeviceDescriptor& Descriptor)
	{
		FDeviceContext Context{};
		Context.Handle = reinterpret_cast<FPlatformDeviceHandle>(Descriptor.Handle);
		Context.Path = Descriptor.Path;
		Context.IsConnected = static_cast<std::int32_t>(Descriptor.IsConnected) == 1;
		Context.DeviceType = static_cast<EDSDeviceType>(Descriptor.DeviceType);
		Context.ConnectionType = static_cast<EDSDeviceConnection>(Descriptor.ConnectionType);

		return Context;
	}

	GamepadDeviceDescriptor MakeDeviceDescriptor(FDeviceContext* Context)
	{
		GamepadDeviceDescriptor Descriptor{};

		Descriptor.Handle = reinterpret_cast<std::uint64_t>(Context->Handle);
		Descriptor.IsConnected = Context->IsConnected ? 1 : 0;
		Descriptor.DeviceType = static_cast<std::int32_t>(Context->DeviceType);
		Descriptor.ConnectionType = static_cast<std::int32_t>(Context->ConnectionType);

		const std::size_t PathLength = std::min(Context->Path.size(), sizeof(Descriptor.Path) - 1);
		std::memcpy(Descriptor.Path, Context->Path.data(), PathLength);
		Descriptor.Path[PathLength] = '\0';

		return Descriptor;
	}
}
