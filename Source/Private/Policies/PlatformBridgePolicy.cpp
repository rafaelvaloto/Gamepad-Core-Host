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
    void PlatformBridgePolicy::Read(FDeviceContext* Context) {
        if (g_UnityPlatformReadCallback)
            g_UnityPlatformReadCallback(Context);
    }

    void PlatformBridgePolicy::Write(FDeviceContext* Context) {
        if (g_UnityPlatformWriteCallback)
            g_UnityPlatformWriteCallback(Context);
    }

    void PlatformBridgePolicy::Detect(std::vector<FDeviceContext>& Devices) {
        if (!g_UnityPlatformDetectCallback)
            return;

        Devices.resize(MaxDetectedDevices);
        const int DetectedCount = g_UnityPlatformDetectCallback(Devices.data(), MaxDetectedDevices);
        if (DetectedCount <= 0) {
            Devices.clear();
            return;
        }

        Devices.resize(static_cast<size_t>(DetectedCount > MaxDetectedDevices
                                                ? MaxDetectedDevices
                                                : DetectedCount));
    }

    bool PlatformBridgePolicy::CreateHandle(FDeviceContext* Context) {
        return g_UnityPlatformCreateHandleCallback &&
               g_UnityPlatformCreateHandleCallback(Context);
    }

    void PlatformBridgePolicy::InvalidateHandle(FDeviceContext* Context) {
        if (g_UnityPlatformInvalidateHandleCallback)
            g_UnityPlatformInvalidateHandleCallback(Context);
    }

    void PlatformBridgePolicy::ProcessAudioHaptic(FDeviceContext* Context) {
        if (g_UnityPlatformProcessAudioHapticCallback)
            g_UnityPlatformProcessAudioHapticCallback(Context);
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
        std::make_unique<GamepadCore::TGenericHardwareInfo<GCU::PlatformBridgePolicy>>());
}
