// Project: Gamepad-Core-Host
// Copyright (c) 2026 valoto.games. All rights reserved.
#pragma once

#include "GCore/Templates/TAudioDeviceRegistry.h"
#include "Policies/PlatformWebAudioPolicy.h"
#include "GCore/Interfaces/Segregations/IGamepadHaptics.h"
#include <memory>
#include <mutex>
#include <vector>

namespace GCH
{
	class AudioDeviceRegistry : public GamepadCore::TAudioDeviceRegistry<PlatformWebAudioPolicy>
	{
	public:
		using AudioFrameCountType = PlatformWebAudioPolicy::AudioFrameCountType;

		~AudioDeviceRegistry() override;
		static AudioDeviceRegistry* Get();
		static void Shutdown();

		bool StartWork();
		void StopWork();
		bool IsWorking() const;
		bool SubmitAudio(const float* AudioData, AudioFrameCountType FrameCount,
		                int NumChannels, int SampleRate);
		void SetVolume(float Volume);
		void SetGain(float Gain);

		float GetVolume() const
		{
			return this->Volume;
		}

		float GetGain() const
		{
			return this->Gain;
		}

		bool ProcessAudioHaptics(IGamepadHaptics* AudioHaptics);
		[[nodiscard]] std::vector<float> GetAudioBuffer() const;

		float Volume = 1.0f;
		float Gain = 1.0f;
	private:
		AudioDeviceRegistry() = default;
		AudioDeviceRegistry(const AudioDeviceRegistry&) = delete;
		AudioDeviceRegistry& operator=(const AudioDeviceRegistry&) = delete;

		mutable std::mutex RegistryMutex;
		std::unique_ptr<PlatformWebAudioPolicy> Worker;
		static std::unique_ptr<AudioDeviceRegistry> Instance;

	};

	using FAudioDeviceRegistry = AudioDeviceRegistry;
}
