// Project: Gamepad-Core-Unity
// Copyright (c) 2026 valoto.games
// All rights reserved.

#include "Policies/PlatformBridgePolicy.h"
#include <cstring>
#include "gamepad_types_api.h"
#include "GCore/Templates/TAudioDeviceRegistry.h"
#include "Ultils/MakeTypes.h"
#include "GImplementations/Utils/GamepadSensors.h"

namespace GCU {

    void PlatformBridgePolicy::Read(FDeviceContext *Context) {
        if (!Context || !Context->IsConnected)
            return;

        std::uint8_t *Buffer = Context->Buffer;
        const auto Handle = reinterpret_cast<std::uintptr_t>(Context->Handle);

        std::int32_t BytesRead = 0;
        constexpr std::int32_t Length = 78;
        const std::int32_t Result = g_UnityPlatformReadCallback(Handle, Buffer, Length, &BytesRead);

        if (Result != 1) {
            InvalidateHandle(Context);
        }
    }

    void PlatformBridgePolicy::Write(FDeviceContext *Context) {
        if (!Context || !Context->IsConnected)
            return;

        const auto Handle = reinterpret_cast<std::uintptr_t>(Context->Handle);

        unsigned char* buffer = Context->GetRawOutputBuffer();

        //size_t InReportLength = Context->DeviceType == EDSDeviceType::DualShock4 ? 32 : 74;
        constexpr std::int32_t OutputReportLength = 78;
        std::int32_t BytesWritten = 0;
        const std::int32_t Result = g_UnityPlatformWriteCallback(Handle, buffer, OutputReportLength, &BytesWritten);

        char Message[128];
        std::snprintf(Message,sizeof(Message),"Write file status: %d", Result);
        GCL::LogCallback(0, Message);
    }

    void PlatformBridgePolicy::Detect(std::vector<FDeviceContext> &Devices) {
        if (!g_UnityPlatformDetectCallback)
            return;

        GCUDeviceDescriptor Descriptors[MaxDetectedDevices]{};
        const auto Count = g_UnityPlatformDetectCallback(Descriptors, MaxDetectedDevices);

        if (GCL::LogCallback) {
            char Message[128];
            std::snprintf(Message,sizeof(Message), "Detected %d devices", Count);

            GCL::LogCallback(0, Message);
        }


        Devices.clear();
        for (int i = 0; i < Count && i < MaxDetectedDevices; ++i) {
            GCUDeviceDescriptor &Descriptor = Descriptors[i];
            FDeviceContext Context = MakeDeviceContext(Descriptor);
            char Message[128];
            std::snprintf(Message,sizeof(Message),
            "Detected ConnectionType=%hhd DeviceType=%hhd",
                Context.ConnectionType,
                Context.DeviceType
            );
            GCL::LogCallback(0, Message);

            Devices.push_back(Context);
        }
    }

    bool PlatformBridgePolicy::CreateHandle(FDeviceContext *Context) {
        if (!Context)
            return false;

        GCUDeviceDescriptor OutDescriptor = MakeDeviceDescriptor(Context);
        if (!g_UnityPlatformCreateHandleCallback(&OutDescriptor)) {
            GCL::LogCallback(0, "not create handle");
            return false;
        }

        if (GCL::LogCallback) {
            char Message[128];
            std::snprintf(Message,sizeof(Message),
                "After callback: Handle=%llu Connected=%d Path=%s",
                OutDescriptor.Handle,
                OutDescriptor.IsConnected,
                OutDescriptor.Path
                );

            GCL::LogCallback(0, Message);
        }

        *Context = MakeDeviceContext(OutDescriptor);

        if (GCL::LogCallback) {
            char Message[128];
            std::snprintf(Message,sizeof(Message),
                "After set context: Handle=%llu Connected=%d Path=%s",
                (long long)Context->Handle,
                Context->IsConnected,
                Context->Path.c_str());

            GCL::LogCallback(0, Message);
        }

        ConfigureFeatures(Context);
        return true;
    }

    void PlatformBridgePolicy::ConfigureFeatures(FDeviceContext *Context) {
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
                g_UnityPlatformConfigureFeaturesCallback(Handle, FeatureBuffer, sizeof(FeatureBuffer), &BytesRead);
                DualShockCalibrationSensors(FeatureBuffer, Calibration, Context);
            }
            else
            {
                unsigned char FeatureBuffer[41] = {0};
                std::memset(FeatureBuffer, 0, sizeof(FeatureBuffer));

                FeatureBuffer[0] = 0x05;
                g_UnityPlatformConfigureFeaturesCallback(Handle, FeatureBuffer, sizeof(FeatureBuffer), &BytesRead);
                DualShockCalibrationSensors(FeatureBuffer, Calibration, Context);
            }

            Context->Calibration = Calibration;
        }
        else
        {
            unsigned char FeatureBuffer[41] = {0};
            std::memset(FeatureBuffer, 0, sizeof(FeatureBuffer));

            FeatureBuffer[0] = 0x05;
            g_UnityPlatformConfigureFeaturesCallback(Handle, FeatureBuffer, sizeof(FeatureBuffer), &BytesRead);

            DualSenseCalibrationSensors(FeatureBuffer, Calibration, Context);
            Context->Calibration = Calibration;

            GCL::LogCallback(0, "Configure Features 41, Calibration");
        }
    }

    void PlatformBridgePolicy::InvalidateHandle(FDeviceContext *Context) {
        if (!Context)
            return;

        const auto Handle = reinterpret_cast<std::uintptr_t>(Context->Handle);
        if (Context->Handle != INVALID_PLATFORM_HANDLE) {
            if (g_UnityPlatformInvalidateHandleCallback) {
                g_UnityPlatformInvalidateHandleCallback(Handle);
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

    void PlatformBridgePolicy::ProcessAudioHaptic(FDeviceContext *Context) {}
} // namespace GCU
