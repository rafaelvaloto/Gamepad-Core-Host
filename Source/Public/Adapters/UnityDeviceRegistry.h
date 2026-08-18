// Project: Gamepad-Core-Unity
// Copyright (c) 2026 valoto.games
// All rights reserved.
#pragma once
#include "GCore/Templates/TBasicDeviceRegistry.h"
#include "Policies/UnityDeviceRegistryPolicy.h"
#include <memory>

namespace GCU {
    class FUnityDeviceRegistry : public GamepadCore::TBasicDeviceRegistry<UnityDeviceRegistryPolicy> {
    public:
        ~FUnityDeviceRegistry() override;

        /**
         * Shuts down the static instance of the FUnityDeviceRegistry class, releasing the memory
         * associated with it. This method ensures that the singleton instance is properly cleaned
         * up when it is no longer needed, preventing potential memory leaks or lingering references.
         * Should be called during the application's shutdown sequence to free resources.
         */
        static void Shutdown();

        /**
         * Retrieves the static instance of the FDeviceRegistry class. This method
         * ensures that only a single instance of the manager class is created and provides
         * global access to it for managing device library instances of Sony gamepad controllers.
         *
         * @return A pointer to the singleton instance of UDeviceContainerManager. Returns nullptr
         *         if the instance has not been initialized.
         */
        static FUnityDeviceRegistry *Get();

    private:
        FUnityDeviceRegistry();

        static std::unique_ptr<FUnityDeviceRegistry> Instance;
    };
} // GCU
