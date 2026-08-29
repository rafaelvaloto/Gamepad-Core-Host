// Project: Gamepad-Core-Host
// Copyright (c) 2026 valoto.games. All rights reserved.

#include "Adapters/AudioDeviceRegistry.h"

namespace GCH
{
	std::unique_ptr<AudioDeviceRegistry> AudioDeviceRegistry::Instance;

	namespace
	{
		std::mutex& InstanceMutex()
		{
			static std::mutex Mutex;
			return Mutex;
		}
	}

	AudioDeviceRegistry::~AudioDeviceRegistry()
	{
		StopWork();
	}

	AudioDeviceRegistry* AudioDeviceRegistry::Get()
	{
		std::scoped_lock Lock(InstanceMutex());
		if (!Instance)
			Instance = std::unique_ptr<AudioDeviceRegistry>(new AudioDeviceRegistry());
		return Instance.get();
	}

	void AudioDeviceRegistry::Shutdown()
	{
		std::scoped_lock Lock(InstanceMutex());
		Instance.reset();
	}

	bool AudioDeviceRegistry::StartWork()
	{
		std::scoped_lock Lock(RegistryMutex);
		if (Worker)
			return Worker->IsValid();

		Worker = std::make_unique<PlatformWebAudioPolicy>();
		return Worker->InitializeAudioContainer(nullptr);
	}

	void AudioDeviceRegistry::StopWork()
	{
		{
			std::scoped_lock Lock(RegistryMutex);
			std::unique_ptr<PlatformWebAudioPolicy> WorkerToStop = std::move(Worker);
		}
	}

	bool AudioDeviceRegistry::IsWorking() const
	{
		std::scoped_lock Lock(RegistryMutex);
		return Worker && Worker->IsValid();
	}

	bool AudioDeviceRegistry::SubmitAudio(const float* AudioData, AudioFrameCountType FrameCount,
	                                      int NumChannels, int SampleRate)
	{
		std::scoped_lock Lock(RegistryMutex);
		if (!Worker)
		{
			Worker = std::make_unique<PlatformWebAudioPolicy>();
			Worker->InitializeAudioContainer(nullptr);
		}
		return Worker->SubmitAudio(AudioData, FrameCount, NumChannels, SampleRate);
	}

	bool AudioDeviceRegistry::ProcessAudioHaptics(IGamepadHaptics* AudioHaptics)
	{
		std::scoped_lock Lock(RegistryMutex);
		return Worker && AudioHaptics && Worker->ProcessAudioHaptics(AudioHaptics);
	}

	void AudioDeviceRegistry::SetGain(const float Gain)
	{
		this->Gain = std::isfinite(Gain) ? std::clamp(Gain, 0.8f, 1.8f) : 1.0f;
		std::scoped_lock Lock(RegistryMutex);
		if (!Worker)
		{
			Worker = std::make_unique<PlatformWebAudioPolicy>();
			Worker->InitializeAudioContainer(nullptr);
		}
		Worker->SetGain(Gain);
	}

	void AudioDeviceRegistry::SetVolume(const float Volume)
	{
		this->Volume = std::isfinite(Volume) ? std::clamp(Volume, 0.0f, 1.0f) : 1.0f;
		std::scoped_lock Lock(RegistryMutex);
		if (!Worker)
		{
			Worker = std::make_unique<PlatformWebAudioPolicy>();
			Worker->InitializeAudioContainer(nullptr);
		}
		Worker->SetVolume(Volume);
	}

	std::vector<float> AudioDeviceRegistry::GetAudioBuffer() const
	{
		std::scoped_lock Lock(RegistryMutex);
		return Worker ? Worker->GetAudioBuffer() : std::vector<float>{};
	}
}
