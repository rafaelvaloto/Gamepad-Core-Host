// Project: Gamepad-Core-Unity
// Copyright (c) 2026 valoto.games
// All rights reserved.
#pragma once
#include "GCore/Templates/TBasicDeviceRegistry.h"
#include "Policies/UnityDeviceRegistryPolicy.h"

namespace GCU {
    class FUnityDeviceRegistry : public GamepadCore::TBasicDeviceRegistry<UnityDeviceRegistryPolicy> {
    public:
        ~FUnityDeviceRegistry() override;
	    /**
	     * Retrieves the static instance of the FDeviceRegistry class. This method
	     * ensures that only a single instance of the manager class is created and provides
	     * global access to it for managing device library instances of Sony gamepad controllers.
	     *
	     * @return A pointer to the singleton instance of UDeviceContainerManager. Returns nullptr
	     *         if the instance has not been initialized.
	     */
	    static FUnityDeviceRegistry* Get();
	    /**
	     * Initializes the singleton instance of the FUnityDeviceRegistry class. This method
	     * must be called before accessing the instance through Get(). It creates the registry
	     * instance only if it hasn't been created yet, ensuring thread-safe singleton initialization.
	     * Should be called once during application startup.
	     */
        static void Initialize();

    private:
        FUnityDeviceRegistry();
    };
} // GCU
