// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.

#include "gamepad_api.h"
#include <cstdio>
#include <vector>
#include "Adapters/DeviceRegistry.h"
#include "Adapters/AudioDeviceRegistry.h"
#include "GCore/Types/ECoreGamepad.h"
#include "Types/MakeTypes.h"

namespace
{
	IGamepadBase* FindGamepad(const int ControllerId)
	{
		auto* Registry = GCH::FDeviceRegistry::Get();
		return Registry ? Registry->GetLibrary(ControllerId) : nullptr;
	}

	bool IsValidHand(const int Hand)
	{
		return Hand >= static_cast<int>(EDSGamepadHand::Left) &&
		       Hand <= static_cast<int>(EDSGamepadHand::AnyHand);
	}
}

GCH_API bool GCH_AudioSubmitSamples(const float* AudioData, const int FrameCount,
									const int NumChannels, const int SampleRate)
{
	if (FrameCount <= 0)
	{
		GCL::Error(0, "[Error]: GCH_AudioSubmitSamples called with invalid FrameCount");
		return false;
	}

	return GCH::AudioDeviceRegistry::Get()->SubmitAudio(
		AudioData, static_cast<std::uint32_t>(FrameCount), NumChannels, SampleRate);
}

GCH_API void GCH_InitializeAudio(const float Volume, const float Gain)
{
	if (auto* Audio = GCH::AudioDeviceRegistry::Get())
	{
		Audio->SetVolume(Volume);
		Audio->SetGain(Gain);
	}
}

GCH_API bool GCH_GetProcessAudioHaptics(const int DeviceId)
{
	auto* Registry = GCH::FDeviceRegistry::Get();
	if (!Registry)
	{
		if constexpr (GCL_DEBUG)
		{
			char Message[128];
			std::snprintf(Message, sizeof(Message), "GCH_GetProcessAudioHaptics called before GCH_InitializePlatformBridge: %d", DeviceId);
			GCL::Error(1, Message);
		}
		return false;
	}

	auto* Gamepad = Registry->GetLibrary(DeviceId);
	if (!Gamepad || !Gamepad->IsConnected() ||
		Gamepad->GetConnectionType() != EDSDeviceConnection::Bluetooth)
		return false;

	auto* AudioHaptics = Gamepad->GetIGamepadHaptics();
	return AudioHaptics && GCH::AudioDeviceRegistry::Get()->ProcessAudioHaptics(AudioHaptics);
}

GCH_API void GCH_DiscoverDevices(const float DeltaTime)
{
	if (auto* Registry = GCH::FDeviceRegistry::Get())
		Registry->PlugAndPlay(DeltaTime);
}


GCH_API void GCH_CreateDevice(const GamepadDeviceDescriptor* Descriptor)
{
	if (!Descriptor)
	{
		GCL::Error(0, "[Error]: GCH_CreateDevice called with null Descriptor");
		return;
	}

	auto* Registry = GCH::FDeviceRegistry::Get();
	if (!Registry)
	{
		GCL::Error(0, "[Error]: GCH_CreateDevice called before GCH_InitializePlatformBridge");
		return;
	}

	FDeviceContext Context = GCH::MakeDeviceContext(*Descriptor);
	Registry->CreateDevice(Context);
}

GCH_API void GCH_UpdateInput(const int DeviceId, const float DeltaTime)
{
	GCH_GetProcessAudioHaptics(DeviceId);

	auto* Registry = GCH::FDeviceRegistry::Get();
	if (!Registry)
	{
		GCL::Error(0, "Not registry GCH::FDeviceRegistry::Get()");
		return;
	}

	auto* Gamepad = Registry->GetLibrary(DeviceId);
	if (!Gamepad)
	{
		char Message[128];
		std::snprintf(Message, sizeof(Message), "Not gamepad library %d", DeviceId);
		GCL::Error(0, Message);
		return;
	}

    Gamepad->UpdateInput(DeltaTime);
}

GCH_API void GCH_UpdateOutput(const int ControllerId)
{
    auto* Gamepad = FindGamepad(ControllerId);
    if (!Gamepad)
    {
        char Message[128];
        std::snprintf(Message, sizeof(Message), "Not gamepad library %d", ControllerId);
        GCL::Error(0, Message);
        return;
    }

    Gamepad->UpdateOutput();
}

GCH_API bool GCH_GetInputState(const int DeviceId, FInputContext* OutInputState)
{
	if (!OutInputState)
	{
		if constexpr (GCL_DEBUG)
		{
			char Message[128];
			std::snprintf(Message, sizeof(Message), "GCH_GetInputState is null: %d", DeviceId);
			GCL::Log(1, Message);
		}
		return false;
	}

	auto* Gamepad = FindGamepad(DeviceId);
	if (!Gamepad)
	{
		if constexpr (GCL_DEBUG)
		{
			char Message[128];
			std::snprintf(Message, sizeof(Message), "FindGamepad is null: %d", DeviceId);
			GCL::Log(1, Message);
		}
		return false;
	}


	auto* DeviceContext = Gamepad->GetMutableDeviceContext();
	if (!DeviceContext)
		return false;

	const FInputContext* InputContext = DeviceContext->GetInputState();
	if (!InputContext)
		return false;

	*OutInputState = *InputContext;
	return true;
}

GCH_API bool GCH_GetDeviceDescriptor(
	const int DeviceId,
	GamepadDeviceDescriptor* OutDescriptor)
{
	if (!OutDescriptor)
		return false;

	auto* Gamepad = FindGamepad(DeviceId);
	if (!Gamepad)
		return false;

	auto* DeviceContext = Gamepad->GetMutableDeviceContext();
	if (!DeviceContext)
		return false;

	*OutDescriptor = GCH::MakeDeviceDescriptor(DeviceContext);
	return true;
}

GCH_API bool GCH_DeviceIsConnected(const int ControllerId)
{
	auto* Gamepad = FindGamepad(ControllerId);
	return Gamepad && Gamepad->IsConnected();
}

GCH_API int GCH_GetDeviceType(const int ControllerId)
{
	auto* Gamepad = FindGamepad(ControllerId);
	if (!Gamepad)
	{
		return static_cast<int>(EDSDeviceType::NotFound);
	}

	return static_cast<int>(Gamepad->GetDeviceType());
}

GCH_API int GCH_GetConnectionType(const int ControllerId)
{
	auto* Gamepad = FindGamepad(ControllerId);
	if (!Gamepad)
	{
		return static_cast<int>(EDSDeviceConnection::Unrecognized);
	}

	return static_cast<int>(Gamepad->GetConnectionType());
}

GCH_API float GCH_BatteryLevelDevice(const int ControllerId)
{
	auto* Gamepad = FindGamepad(ControllerId);
	return Gamepad ? Gamepad->GetBattery() : 0.0f;
}

GCH_API void GCH_Lightbar(const int ControllerId, const std::uint8_t R,
                          const std::uint8_t G, const std::uint8_t B)
{
	auto* Gamepad = FindGamepad(ControllerId);
	if (auto* Lightbar = Gamepad ? Gamepad->GetIGamepadLightbar() : nullptr)
		Lightbar->SetLightbar({R, G, B, 0});
}

GCH_API void GCH_LightbarFlash(const int ControllerId, const std::uint8_t R,
                               const std::uint8_t G, const std::uint8_t B,
                               const float BrightnessTime, const float ToggleTime)
{
	auto* Gamepad = FindGamepad(ControllerId);
	if (auto* Lightbar = Gamepad ? Gamepad->GetIGamepadLightbar() : nullptr)
		Lightbar->SetLightbarFlash({R, G, B, 0}, BrightnessTime, ToggleTime);
}

GCH_API void GCH_PlayerLed(const int ControllerId, const int Led,
                           const std::uint8_t Brightness)
{
	auto* Gamepad = FindGamepad(ControllerId);
	if (auto* Lightbar = Gamepad ? Gamepad->GetIGamepadLightbar() : nullptr)
		Lightbar->SetPlayerLed(static_cast<EDSPlayer>(Led), Brightness);
}

GCH_API void GCH_ResetLights(const int ControllerId)
{
	auto* Gamepad = FindGamepad(ControllerId);
	if (auto* Lightbar = Gamepad ? Gamepad->GetIGamepadLightbar() : nullptr)
		Lightbar->ResetLights();
}

GCH_API void GCH_DualSenseSettings(
	const int ControllerId,
	const std::uint8_t bIsMic,
	const std::uint8_t bIsHeadset,
	const std::uint8_t bIsSpeaker,
	const std::uint8_t MicVolume,
	const std::uint8_t AudioVolume,
	const std::uint8_t RumbleMode,
	const std::uint8_t RumbleReduce,
	const std::uint8_t TriggerReduce)
{
	auto* Gamepad = FindGamepad(ControllerId);
	auto* Context = Gamepad ? Gamepad->GetMutableDeviceContext() : nullptr;
	if (auto* Settings = Gamepad ? Gamepad->GetIGamepadSettings() : nullptr)
	{
		if (Context->Output.Audio.HeadsetVolume != AudioVolume)
		{
			Context->Output.Audio.HeadsetVolume = AudioVolume;
			Context->Output.Audio.SpeakerVolume = AudioVolume;
			if (const auto* Audio = GCH::AudioDeviceRegistry::Get())
			{
				const float Volume = static_cast<float>(AudioVolume) / 100.0f;
				GCH_InitializeAudio(Volume, Audio->GetGain());
			}
		}
		Settings->DualSenseSettings(Context->Output.Audio.MicStatus, bIsHeadset, bIsSpeaker, Context->Output.Audio.MicVolume, AudioVolume,RumbleMode, RumbleReduce, TriggerReduce);
	}
}

GCH_API void GCH_ResetGyroOrientation(const int ControllerId)
{
	auto* Gamepad = FindGamepad(ControllerId);
	if (auto* Sensors = Gamepad ? Gamepad->GetIGamepadSensors() : nullptr)
		Sensors->ResetGyroOrientation();
}

GCH_API void GCH_EnableGyroscopeValues(const int ControllerId,
                                       const bool EnableGyroscope)
{
	auto* Gamepad = FindGamepad(ControllerId);
	if (auto* Sensors = Gamepad ? Gamepad->GetIGamepadSensors() : nullptr)
		Sensors->EnableMotionSensor(EnableGyroscope);
}

GCH_API void GCH_EnableTouch(const int ControllerId, const bool EnableTouch)
{
	auto* Gamepad = FindGamepad(ControllerId);
	if (auto* Touch = Gamepad ? Gamepad->GetIGamepadTouch() : nullptr)
		Touch->EnableTouch(EnableTouch);
}

GCH_API bool GCH_CustomTrigger(const int ControllerId,
                               const std::uint8_t* HexBytes,
                               const int ByteCount,
                               const int Hand)
{
	if (ByteCount < 0 || (ByteCount > 0 && !HexBytes) || !IsValidHand(Hand))
		return false;

	auto* Gamepad = FindGamepad(ControllerId);
	auto* Trigger = Gamepad ? Gamepad->GetIGamepadTrigger() : nullptr;
	if (!Trigger)
		return false;

	std::vector<std::uint8_t> Bytes(HexBytes, HexBytes + ByteCount);
	Trigger->SetCustomTrigger(static_cast<EDSGamepadHand>(Hand), Bytes);
	return true;
}

GCH_API void GCH_StopTrigger(const int ControllerId, const int Hand)
{
	if (!IsValidHand(Hand))
		return;

	auto* Gamepad = FindGamepad(ControllerId);
	if (auto* Trigger = Gamepad ? Gamepad->GetIGamepadTrigger() : nullptr)
		Trigger->StopTrigger(static_cast<EDSGamepadHand>(Hand));
}

GCH_API void GCH_SetVibration(const int ControllerId,
                              const std::uint8_t LeftRumble,
                              const std::uint8_t RightRumble)
{
	auto* Gamepad = FindGamepad(ControllerId);
	if (auto* Rumbles = Gamepad ? Gamepad->GetIGamepadRumbles() : nullptr)
		Rumbles->SetVibration(LeftRumble, RightRumble);
}
