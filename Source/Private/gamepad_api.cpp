// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.

#include "gamepad_api.h"
#include <cstdio>
#include "Adapters/DeviceRegistry.h"
#include "Types/MakeTypes.h"

GCH_API void GCH_DiscoverDevices(const float DeltaTime)
{
    if (auto* Registry = GCH::FDeviceRegistry::Get())
        Registry->PlugAndPlay(DeltaTime);
}

GCH_API void GCH_UpdateInput(const int DeviceId, const float DeltaTime)
{
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

GCH_API bool GCH_GetInputState(const int DeviceId, FInputContext* OutInputState)
{
    if (!OutInputState)
    {
        GCL::Error(0, "[Error]: OutInputState is null");
        return false;
    }

    auto* Registry = GCH::FDeviceRegistry::Get();
    if (!Registry)
    {
        GCL::Error(0, "[Error]: Not registry GCH::FDeviceRegistry::Get()");
        return false;
    }

    auto* Gamepad = Registry->GetLibrary(DeviceId);
    if (!Gamepad)
        return false;

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

    auto* Registry = GCH::FDeviceRegistry::Get();
    if (!Registry)
        return false;

    auto* Gamepad = Registry->GetLibrary(DeviceId);
    if (!Gamepad)
        return false;

    auto* DeviceContext = Gamepad->GetMutableDeviceContext();
    if (!DeviceContext)
        return false;

    *OutDescriptor = GCH::MakeDeviceDescriptor(DeviceContext);
    return true;
}
