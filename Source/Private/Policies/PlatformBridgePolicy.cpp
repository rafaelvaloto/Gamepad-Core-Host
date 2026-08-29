// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.

#include "Policies/PlatformBridgePolicy.h"
#include <cstring>
#include "Types/GCHTypes.h"
#include "GCore/Templates/TAudioDeviceRegistry.h"
#include "Types/MakeTypes.h"
#include "GImplementations/Utils/GamepadSensors.h"

namespace GCH
{

	void PlatformBridgePolicy::Read(FDeviceContext* Context)
	{
		if (!Context || !Context->IsConnected)
			return;

		std::uint8_t* Buffer = Context->Buffer;
		const auto Handle = reinterpret_cast<std::uintptr_t>(Context->Handle);
		//
		std::int32_t InputReportLength = 64;
		if (Context->ConnectionType == EDSDeviceConnection::Bluetooth && Context->DeviceType == EDSDeviceType::DualShock4)
		{
			InputReportLength = 547;
		}
		else
		{
			InputReportLength = Context->ConnectionType == EDSDeviceConnection::Bluetooth ? 78 : 64;
		}

		std::int32_t BytesRead = 0;
		const std::int32_t Result = g_PlatformReadCallback(Handle, Buffer, InputReportLength, &BytesRead);

		if (Result != 1)
		{
			char Message[128];
			std::snprintf(Message, sizeof(Message), "Bytes Read %d file status: %d", BytesRead, Result);
			GCL::Error(0, Message);
			InvalidateHandle(Context);
		}
	}

	void PlatformBridgePolicy::Write(FDeviceContext* Context)
	{
		if (!Context || !Context->IsConnected)
			return;

		unsigned char* buffer = Context->GetRawOutputBuffer();
		const auto Handle = reinterpret_cast<std::uintptr_t>(Context->Handle);

		std::int32_t OutputReportLength = 74;
		if (Context->DeviceType == EDSDeviceType::DualShock4)
		{
			OutputReportLength = Context->ConnectionType == EDSDeviceConnection::Bluetooth ? 78 : 32;
		}
		else
		{
			// PS5 devices
			OutputReportLength = Context->ConnectionType == EDSDeviceConnection::Bluetooth ? 78 : 74;
		}

		std::int32_t BytesWritten = 0;
		if (const std::int32_t Result = g_PlatformWriteCallback(Handle, buffer, OutputReportLength, &BytesWritten))
		{
			char Message[128];
			std::snprintf(Message, sizeof(Message), "Bytes Written %d file status: %d", BytesWritten, Result);
			GCL::Log(0, Message);
		}
	}

	void PlatformBridgePolicy::Detect(std::vector<FDeviceContext>& Devices)
	{
		if (!g_PlatformDetectCallback)
			return;

		GamepadDeviceDescriptor Descriptors[MaxDetectedDevices]{};
		const auto Count = g_PlatformDetectCallback(Descriptors, MaxDetectedDevices);

		if constexpr (GCL_DEBUG)
		{
			char Message[128];
			std::snprintf(Message, sizeof(Message), "Detected %d devices", Count);
			GCL::Log(0, Message);
		}

		Devices.clear();
		for (int i = 0; i < Count && i < MaxDetectedDevices; ++i)
		{
			GamepadDeviceDescriptor& Descriptor = Descriptors[i];
			FDeviceContext Context = MakeDeviceContext(Descriptor);

			if constexpr (GCL_DEBUG)
			{
				char Message[128];
				std::snprintf(Message, sizeof(Message),
				              "Detected Connection=%hhd Type=%hhd Path:%s",
				              Context.ConnectionType,
				              Context.DeviceType,
				              Context.Path.c_str()
					);
				GCL::Log(0, Message);
			}

			Devices.push_back(Context);
		}
	}

	bool PlatformBridgePolicy::CreateHandle(FDeviceContext* Context)
	{
		if (!Context)
		{
			GCL::Error(0, "[Error]: Context is null in CreateHandle");
			return false;
		}

		if (!g_PlatformCreateHandleCallback)
		{
			GCL::Error(0, "[Error]: PlatformCreateHandleCallback is not configured");
			return false;
		}

		GamepadDeviceDescriptor OutDescriptor = MakeDeviceDescriptor(Context);
		if (!g_PlatformCreateHandleCallback(&OutDescriptor))
		{
			GCL::Error(0, "[Error]: Platform handle creation failed");
			return false;
		}

		*Context = MakeDeviceContext(OutDescriptor);
		ConfigureFeatures(Context);
		return true;
	}

	void PlatformBridgePolicy::ConfigureFeatures(FDeviceContext* Context)
	{
		if (!Context)
			return;

		std::int32_t BytesRead = 0;
		const auto Handle = reinterpret_cast<std::uint64_t>(Context->Handle);

		using namespace FGamepadSensors;
		FGamepadCalibration Calibration;
		if (Context->DeviceType == EDSDeviceType::DualShock4)
		{
			if (Context->ConnectionType == EDSDeviceConnection::Usb)
			{
				unsigned char FeatureBuffer[37] = {0};
				std::memset(FeatureBuffer, 0, sizeof(FeatureBuffer));

				FeatureBuffer[0] = 0x02;
				g_PlatformConfigureFeaturesCallback(Handle, FeatureBuffer, sizeof(FeatureBuffer), &BytesRead);
				DualShockCalibrationSensors(FeatureBuffer, Calibration, Context);
			}
			else
			{
				unsigned char FeatureBuffer[41] = {0};
				std::memset(FeatureBuffer, 0, sizeof(FeatureBuffer));

				FeatureBuffer[0] = 0x05;
				g_PlatformConfigureFeaturesCallback(Handle, FeatureBuffer, sizeof(FeatureBuffer), &BytesRead);
				DualShockCalibrationSensors(FeatureBuffer, Calibration, Context);
			}

			Context->Calibration = Calibration;
			GCL::Log(0, "Configure Features DualShock, Calibration");
		}
		else
		{
			unsigned char FeatureBuffer[41] = {0};
			std::memset(FeatureBuffer, 0, sizeof(FeatureBuffer));

			FeatureBuffer[0] = 0x05;
			g_PlatformConfigureFeaturesCallback(Handle, FeatureBuffer, sizeof(FeatureBuffer), &BytesRead);

			DualSenseCalibrationSensors(FeatureBuffer, Calibration, Context);
			Context->Calibration = Calibration;

			GCL::Log(0, "Configure Features DualSense 41, Calibration");
		}
	}

	void PlatformBridgePolicy::InvalidateHandle(FDeviceContext* Context)
	{
		GCL::Log(0, "Invalidate Handle");
		if (!Context)
			return;

		const auto Handle = reinterpret_cast<std::uintptr_t>(Context->Handle);
		if (Context->Handle != INVALID_PLATFORM_HANDLE)
		{
			if (g_PlatformInvalidateHandleCallback)
			{
				g_PlatformInvalidateHandleCallback(Handle);
			}

			Context->Handle = INVALID_PLATFORM_HANDLE;
		}

		Context->IsConnected = false;
		Context->Path.clear();

		std::memset(Context->Buffer, 0, sizeof(Context->Buffer));
		std::memset(Context->BufferDS4, 0, sizeof(Context->BufferDS4));
		std::memset(Context->BufferHapitcs, 0, sizeof(Context->BufferHapitcs));
		std::memset(Context->GetRawOutputBuffer(), 0, 78);
	}

	void PlatformBridgePolicy::ProcessAudioHaptic(FDeviceContext* Context)
	{
		GCL::Log(0, "[Platform] ProcessAudioHaptic");
		if (!Context)
		{
			GCL::Log(0, "Invalid Context");
			return;
		}

		if (Context->ConnectionType != EDSDeviceConnection::Bluetooth)
		{
			GCL::Log(0, "Invalid Connection Type");
			return;
		}


		unsigned char* buffer = Context->BufferHapitcs;
		const auto Handle = reinterpret_cast<std::uintptr_t>(Context->Handle);

		std::int32_t BytesWritten = 0;
		constexpr std::int32_t BytesSending = 398;
		if (const std::int32_t Result = g_PlatformWriteCallback(Handle, buffer, BytesSending, &BytesWritten))
		{
			char Message[128];
			std::snprintf(Message, sizeof(Message), "Bytes Written %d file status: %d", BytesWritten, Result);
			GCL::Log(0, Message);
		}
	}
} // namespace GCH
