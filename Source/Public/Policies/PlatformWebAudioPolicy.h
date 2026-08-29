// Project: Gamepad-Core-Host
// Copyright (c) 2026 valoto.games. All rights reserved.
#pragma once

#include "GCore/Types/Structs/Context/DeviceContext.h"
#include "GCore/Interfaces/Segregations/IGamepadHaptics.h"
#include "Types/GCHTypes.h"
#include <cstdint>
#include <algorithm>
#include <array>
#include <cstdio>
#include <cmath>
#include <deque>
#include <mutex>
#include <string>
#include <vector>
#include <opus.h>

namespace GCH
{
	struct PlatformWebAudioPolicy
	{
		using Policy = PlatformWebAudioPolicy;
		using DevicePathType = std::string;
		using ContextType = FDeviceContext;
		using AudioDeviceType = void*;
		using AudioDeviceIdType = std::string;
		using AudioRingBufferType = std::vector<float>;
		using AudioFrameCountType = std::uint32_t;

		struct BTPacket
		{
			std::vector<std::uint8_t> haptics = std::vector<std::uint8_t>(64, 0);
			std::vector<std::uint8_t> signal = std::vector<std::uint8_t>(200, 0);
		};

		PlatformWebAudioPolicy() = default;

		~PlatformWebAudioPolicy()
		{
			Close();
		}

		void Close()
		{
			std::scoped_lock Lock(Mutex);
			if (DSOpusEncoding)
			{
				opus_encoder_destroy(DSOpusEncoding);
				DSOpusEncoding = nullptr;
			}
			Buffer.clear();
			AudioPacketQueue.clear();
			LowPassStateLeft = 0.0f;
			LowPassStateRight = 0.0f;
			bInitialized = false;
		}

		[[nodiscard]] bool IsValid() const
		{
			std::scoped_lock Lock(Mutex);
			return bInitialized;
		}

		bool InitializeAudioContainer(ContextType*)
		{
			std::scoped_lock Lock(Mutex);
			if (!DSOpusEncoding)
			{
				int Error = OPUS_OK;
				DSOpusEncoding = opus_encoder_create(48000, 2, OPUS_APPLICATION_AUDIO, &Error);
				if (!DSOpusEncoding || Error != OPUS_OK)
				{
					DSOpusEncoding = nullptr;
					return false;
				}

				opus_encoder_ctl(DSOpusEncoding, OPUS_SET_EXPERT_FRAME_DURATION(OPUS_FRAMESIZE_10_MS));
				opus_encoder_ctl(DSOpusEncoding, OPUS_SET_BITRATE(OPUS_BITRATE_MAX));
				opus_encoder_ctl(DSOpusEncoding, OPUS_SET_VBR(0));
				opus_encoder_ctl(DSOpusEncoding, OPUS_SET_COMPLEXITY(0));
				opus_encoder_ctl(DSOpusEncoding, OPUS_SET_PREDICTION_DISABLED(1));
				opus_encoder_ctl(DSOpusEncoding, OPUS_SET_SIGNAL(OPUS_SIGNAL_MUSIC));
			}
			bInitialized = true;
			return true;
		}

		bool InitializeWithDeviceId(const AudioDeviceIdType*, int = 48000, int = 2)
		{
			return InitializeAudioContainer(nullptr);
		}

		void SetVolume(const float VolumeValue)
		{
			std::scoped_lock Lock(Mutex);
			Volume = std::isfinite(VolumeValue) ? std::clamp(VolumeValue, 0.0f, 1.0f) : 1.0f;
		}

		void SetGain(const float GainValue)
		{
			if (GainValue > 1.8f)
			{
				GCL::Log(0, "[Warning]: Gain value exceeds 1.8, which may cause audio distortion");
				return;
			}
			std::scoped_lock Lock(Mutex);
			Gain = std::isfinite(GainValue) ? std::max(0.8f, GainValue) : 1.0f;
		}

		bool SubmitAudio(const float* AudioData, AudioFrameCountType FrameCount,
		                int InNumChannels, int InSampleRate)
		{
			if (!AudioData || FrameCount == 0 || InNumChannels <= 0 || InSampleRate <= 0)
			{
				GCL::Log(0, "[Error]: SubmitAudio called with invalid parameters");
				return false;
			}


			std::scoped_lock Lock(Mutex);
			{
				NumChannels = InNumChannels;
				SampleRate = InSampleRate;

				if (InSampleRate != 48000 || InNumChannels != 2)
				{
					char Message[128];
					std::snprintf(Message, sizeof(Message), "[Error]: SubmitAudio called with unsupported SampleRate=%d or NumChannels=%d", InSampleRate, InNumChannels);
					GCL::Log(0, Message);
					return false;
				}

				Buffer.insert(Buffer.end(), AudioData, AudioData + FrameCount * InNumChannels);
			}

			constexpr AudioFrameCountType EncodeFrameCount = 1024;
			// Buffer stores interleaved samples, while EncodeFrameCount is measured
			// in frames. A stereo block of 1024 frames contains 2048 float samples.
			while (Buffer.size() >= EncodeFrameCount * static_cast<std::size_t>(NumChannels))
			{
				if (!EncodeAudio(Buffer.data(), EncodeFrameCount))
				{
					GCL::Log(0, "[Error]: EncodeAudio failed");
					return false;
				}

				Buffer.erase(Buffer.begin(), Buffer.begin() + EncodeFrameCount * InNumChannels);
			}
			bInitialized = true;

			return true;
		}

		[[nodiscard]] std::vector<float> GetAudioBuffer() const
		{
			std::scoped_lock Lock(Mutex);
			return Buffer;
		}

		bool WriteHapticData(const std::vector<float>&) { return IsValid(); }

		bool ProcessAudioHaptics(IGamepadHaptics* AudioHaptics)
		{
			if (!AudioHaptics)
			{
				char Message[128];
				std::snprintf(Message, sizeof(Message), "[Error] GCH_ProcessAudioHaptics called with null AudioHaptics");
				GCL::Error(0, Message);
				return false;
			}

			std::scoped_lock Lock(Mutex);
			bool bProcessed = false;
			while (!AudioPacketQueue.empty())
			{
				BTPacket Packet = std::move(AudioPacketQueue.front());
				AudioPacketQueue.pop_front();
				if (Packet.haptics.empty() || Packet.signal.empty())
				{
					char Message[128];
					std::snprintf(Message, sizeof(Message), "[Error] GCH_ProcessAudioHaptics called with empty haptics or signal");
					GCL::Error(0, Message);
					continue;
				}

				AudioHaptics->AudioHapticUpdate(Packet.haptics, Packet.signal);
				bProcessed = true;
			}
			return bProcessed;
		}

		private:
		bool EncodeAudio(const float* AudioData, AudioFrameCountType FrameCount)
		{
			constexpr int32_t InputFrames = 1024;
			constexpr int32_t OutputFrames = 960;
			constexpr int32_t OpusFramesPerPacket = 480;
			constexpr int32_t HapticsSampleRate = 3000;
			constexpr int32_t PacketHapticsBytes = 64;
			constexpr int32_t MaxOpusBytes = 200;
			constexpr float LowPassAlphaWireless = 0.98f;

			const int32_t RatioHaptics = SampleRate / HapticsSampleRate;
			if (!AudioData || !DSOpusEncoding || SampleRate % HapticsSampleRate != 0 ||
				RatioHaptics <= 0 || FrameCount != InputFrames || NumChannels != 2)
			{
				char Message[128];
				std::snprintf(Message, sizeof(Message), "[Error] EncodeAudio failed: DSOpusEncoding=%p, RatioHaptics=%d, FrameCount=%d", DSOpusEncoding, RatioHaptics, FrameCount);
				GCL::Error(0, Message);
				return false;
			}

			// Downsample 48 kHz interleaved stereo to the 3 kHz wireless haptics rate.
			const int32_t NumSamples = static_cast<int32_t>(FrameCount * NumChannels);
			const int32_t TargetSamples = NumSamples / RatioHaptics;
			std::vector<std::int8_t> ResampledDataLeft;
			std::vector<std::int8_t> ResampledDataRight;
			ResampledDataLeft.reserve(TargetSamples / 2);
			ResampledDataRight.reserve(TargetSamples / 2);

			for (int32_t i = 0; i < TargetSamples / 2; ++i)
			{
				const int32_t SourceIndex = i * RatioHaptics * 2;
				if (SourceIndex + 1 >= NumSamples)
				{
					break;
				}

				const float InLeft = AudioData[SourceIndex];
				const float InRight = AudioData[SourceIndex + 1];
				LowPassStateLeft = (1.0f - LowPassAlphaWireless) * InLeft + LowPassAlphaWireless * LowPassStateLeft;
				LowPassStateRight = (1.0f - LowPassAlphaWireless) * InRight + LowPassAlphaWireless * LowPassStateRight;

				const float OutLeft = InLeft - LowPassStateLeft;
				const float OutRight = InRight - LowPassStateRight;
				ResampledDataLeft.push_back(static_cast<std::int8_t>(std::clamp(OutLeft * 127.0f, -128.0f, 127.0f)));
				ResampledDataRight.push_back(static_cast<std::int8_t>(std::clamp(OutRight * 127.0f, -128.0f, 127.0f)));
			}

			std::vector<std::uint8_t> HapticsPacket1(PacketHapticsBytes, 0);
			std::vector<std::uint8_t> HapticsPacket2(PacketHapticsBytes, 0);
			for (int32_t i = 0; i < PacketHapticsBytes / 2; ++i)
			{
				if (i < static_cast<int32_t>(ResampledDataLeft.size()))
				{
					HapticsPacket1[i * 2] = static_cast<std::uint8_t>(ResampledDataLeft[i]);
					HapticsPacket1[i * 2 + 1] = static_cast<std::uint8_t>(ResampledDataRight[i]);
				}

				const int32_t SecondHalfIndex = i + PacketHapticsBytes / 2;
				if (SecondHalfIndex < static_cast<int32_t>(ResampledDataLeft.size()))
				{
					HapticsPacket2[i * 2] = static_cast<std::uint8_t>(ResampledDataLeft[SecondHalfIndex]);
					HapticsPacket2[i * 2 + 1] = static_cast<std::uint8_t>(ResampledDataRight[SecondHalfIndex]);
				}
			}

			// Resample 1024 input frames to 960 frames for the two 10 ms Opus packets.
			const float ResampleRatio = static_cast<float>(InputFrames) / static_cast<float>(OutputFrames);
			std::vector<float> AudioDataResampled;
			AudioDataResampled.reserve(OutputFrames * NumChannels);
			for (int32_t i = 0; i < OutputFrames; ++i)
			{
				const float SourceIndex = static_cast<float>(i) * ResampleRatio;
				const int32_t IndexLow = static_cast<int32_t>(SourceIndex);
				const int32_t IndexHigh = std::min(IndexLow + 1, InputFrames - 1);
				const float Fraction = SourceIndex - static_cast<float>(IndexLow);
				for (int32_t Channel = 0; Channel < NumChannels; ++Channel)
				{
					const float SampleLow = AudioData[IndexLow * NumChannels + Channel];
					const float SampleHigh = AudioData[IndexHigh * NumChannels + Channel];
					AudioDataResampled.push_back(std::clamp(
						(SampleLow + Fraction * (SampleHigh - SampleLow)) * Volume * Gain, -1.0f, 1.0f));
				}
			}

			BTPacket Packet1;
			BTPacket Packet2;
			std::array<std::uint8_t, MaxOpusBytes> Encoded{};
			int EncodedBytes1 = opus_encode_float(DSOpusEncoding, AudioDataResampled.data(), OpusFramesPerPacket, Encoded.data(), Encoded.size());
			if (EncodedBytes1 < 0)
			{
				char Message[128];
				std::snprintf(Message, sizeof(Message), "[Error]: opus_encode_float failed with error code=%d", EncodedBytes1);
				GCL::Error(0, Message);
				return false;
			}
			Packet1.signal.assign(Encoded.begin(), Encoded.begin() + EncodedBytes1);
			Packet1.haptics = std::move(HapticsPacket1);
			Encoded.fill(0);

			int EncodedBytes2 = opus_encode_float(DSOpusEncoding, AudioDataResampled.data() + OpusFramesPerPacket * NumChannels, OpusFramesPerPacket, Encoded.data(), Encoded.size());
			if (EncodedBytes2 < 0)
			{
				char Message[128];
				std::snprintf(Message, sizeof(Message), "[Error]: opus_encode_float failed with error code=%d", EncodedBytes2);
				GCL::Error(0, Message);
				return false;
			}

			Packet2.signal.assign(Encoded.begin(), Encoded.begin() + EncodedBytes2);
			Packet2.haptics = std::move(HapticsPacket2);
			AudioPacketQueue.push_back(std::move(Packet1));
			AudioPacketQueue.push_back(std::move(Packet2));
			return true;
		}

		mutable std::mutex Mutex;
		std::vector<float> Buffer;
		std::deque<BTPacket> AudioPacketQueue;
		OpusEncoder* DSOpusEncoding = nullptr;
		int NumChannels = 2;
		int SampleRate = 0;
		float Gain = 1.0f;
		float Volume = 1.0f;
		float LowPassStateLeft = 0.0f;
		float LowPassStateRight = 0.0f;
		bool bInitialized = false;
	};
}
