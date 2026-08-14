// Project: Gamepad-Core-Unity
// Copyright (c) 2026 valoto.games
// All rights reserved.

#include "Policies/PlatformBridgePolicy.h"
#include "GCore/Interfaces/IPlatformHardware.h"
#include "GCore/Templates/TGenericHardwareInfo.h"
#include "gamepad_unity_api.h"

namespace {
    constexpr int MaxDetectedDevices = 64;
}

PlatformReadCallback g_UnityPlatformReadCallback = nullptr;
PlatformWriteCallback g_UnityPlatformWriteCallback = nullptr;
PlatformDetectCallback g_UnityPlatformDetectCallback = nullptr;
PlatformCreateHandleCallback g_UnityPlatformCreateHandleCallback = nullptr;
PlatformInvalidateHandleCallback g_UnityPlatformInvalidateHandleCallback = nullptr;
PlatformProcessAudioHapticCallback g_UnityPlatformProcessAudioHapticCallback = nullptr;

namespace GCU {

    namespace {
        FDeviceContext MakeDeviceContext(const GCUDeviceDescriptor &Descriptor) {
            FDeviceContext Context{};
            Context.Handle =
                    reinterpret_cast<FPlatformDeviceHandle>(
                        static_cast<std::uintptr_t>(Descriptor.Handle));

            Context.Path = Descriptor.Path;
            Context.DeviceType =
                    static_cast<EDSDeviceType>(Descriptor.DeviceType);
            Context.ConnectionType =
                    static_cast<EDSDeviceConnection>(Descriptor.ConnectionType);
            Context.IsConnected =
                    Descriptor.IsConnected != 0;

            return Context;
        }
    }

    void PlatformBridgePolicy::Read(FDeviceContext *Context) {
        if (!Context || !Context->IsConnected)
            return;

        std::uint8_t *Buffer = Context->Buffer;
        const auto Handle = reinterpret_cast<std::uintptr_t>(Context->Handle);

        std::int32_t Length = 64;
        if (Context->DeviceType == EDSDeviceType::DualShock4 &&
            Context->ConnectionType == EDSDeviceConnection::Bluetooth) {
            Buffer = Context->BufferDS4;
            Length = 547;
        } else if (Context->ConnectionType == EDSDeviceConnection::Bluetooth) {
            Length = 78;
        }

        std::int32_t BytesRead = 0;
        if (const std::int32_t Result = g_UnityPlatformReadCallback(Handle, Buffer, Length, &BytesRead); Result == 0) {
            InvalidateHandle(Context);
        }
    }

    void PlatformBridgePolicy::Write(FDeviceContext *Context) {
        if (!Context || !Context->IsConnected)
            return;

        std::int32_t BytesWritten = 0;
        const auto Handle = reinterpret_cast<std::uintptr_t>(Context->Handle);
        const int Length = Context->DeviceType == EDSDeviceType::DualShock4 ? 32 : 74;
        if (!g_UnityPlatformWriteCallback(Handle, Context->GetRawOutputBuffer(), Length, &BytesWritten)) {
            InvalidateHandle(Context);
        }
    }

    void PlatformBridgePolicy::Detect(std::vector<FDeviceContext> &Devices) {
        if (!g_UnityPlatformDetectCallback)
            return;

        GCUDeviceDescriptor Descriptors[MaxDetectedDevices]{};
        const int Count = g_UnityPlatformDetectCallback(Descriptors, MaxDetectedDevices);

        Devices.clear();
        for (int i = 0; i < Count && i < MaxDetectedDevices; ++i) {
            Devices.push_back(MakeDeviceContext(Descriptors[i]));
        }

        for (int i = 0; i < Count && i < 64; ++i) {
            Devices.push_back(MakeDeviceContext(Descriptors[i]));
        }
    }

    bool PlatformBridgePolicy::CreateHandle(FDeviceContext * /*Context*/) {
        return true;
    }

    void PlatformBridgePolicy::InvalidateHandle(FDeviceContext * /*Context*/) {
    }

    void PlatformBridgePolicy::ProcessAudioHaptic(FDeviceContext *Context) {
    }
} // namespace GCU

void GCU_InitializePlatformBridge(
    const PlatformReadCallback readCallback,
    const PlatformWriteCallback writeCallback,
    const PlatformDetectCallback detectCallback,
    const PlatformCreateHandleCallback createHandleCallback,
    const PlatformInvalidateHandleCallback invalidateHandleCallback,
    const PlatformProcessAudioHapticCallback processAudioHapticCallback) {
    g_UnityPlatformReadCallback = readCallback;
    g_UnityPlatformWriteCallback = writeCallback;
    g_UnityPlatformDetectCallback = detectCallback;
    g_UnityPlatformCreateHandleCallback = createHandleCallback;
    g_UnityPlatformInvalidateHandleCallback = invalidateHandleCallback;
    g_UnityPlatformProcessAudioHapticCallback = processAudioHapticCallback;

    IPlatformHardware::SetInstance(
        std::make_unique<GamepadCore::TGenericHardwareInfo<GCU::PlatformBridgePolicy> >());
}
