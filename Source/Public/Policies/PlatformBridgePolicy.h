// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.
#pragma once

#include "GCore/Types/Structs/Context/DeviceContext.h"

namespace GCH
{
	class PlatformBridgePolicy
	{
	public:
		static void Read(FDeviceContext* Context);

		static void Write(FDeviceContext* Context);

		static void Detect(std::vector<FDeviceContext>& Devices);

		static bool CreateHandle(FDeviceContext* Context);

		static void InvalidateHandle(FDeviceContext* Context);

		static void ConfigureFeatures(FDeviceContext* Context);

		static void ProcessAudioHaptic(FDeviceContext* Context);
	};
} // namespace GCH
