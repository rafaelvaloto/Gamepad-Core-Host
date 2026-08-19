# 🎮 Gamepad-Core Host Bridge

**Gamepad-Core Host Bridge** is a native C++ library that exposes the functionality of Gamepad-Core through a high-level, C-compatible API. The current project version is **1.0.6**.

---

## ✨ Features

* **Device Registry Integration:** Support for host applications or external game engines.
* **Platform Bridge Callbacks:** Efficient device detection and I/O operations.
* **Handle Management:** Support for creating and invalidating device handles natively within the platform.
* **Advanced Forwarding:** Seamless forwarding of input, output, and audio-haptics operations.
* **C-Compatible Entry Points:** Exported endpoints optimized for managed language integrations.
* **Direct Debugging:** Configurable log callbacks for real-time host debugging.

---

## ⚙️ Native API Lifecycle

The host application should implement the following lifecycle and update loop steps:

1. **Initialize the Bridge:** Set up platform bridge callbacks by calling `GCH_InitializePlatformBridge`.
2. **Register Policies:** Define allocation, dispatch, and disconnection callbacks via `GCH_InitializeDeviceRegistryPolicy`.
3. **Discover Devices:** Continuously poll for new hardware by calling `GCH_DiscoverDevices` from within the update loop.
4. **Update Controllers:** Refresh the state of each connected controller using `GCH_UpdateInput`.
5. **Retrieve Data:** Access input states and hardware information via `GCH_GetInputState` and `GCH_GetDeviceDescriptor`.

### Callbacks & Shutdown
* **Logging:** Bridge logs can be redirected by configuring a function callback via `GCH_SetLogCallback`.
* **Version Checking:** Retrieve the compiled library version with `GCH_GetVersion`.
* **Safe Shutdown:** Upon termination, the host application **must** call `GCH_Shutdown` to safely destroy the platform instance and clear callback pointers.

---

## 📚 API References

The following functions are exported by the native bridge:

| Function                                                                                                                                                                                                                                           | Return type |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|-------------|
| `GCH_DiscoverDevices(float DeltaTime)`                                                                                                                                                                                                             | `void`      |
| `GCH_UpdateInput(int DeviceId, float DeltaTime)`                                                                                                                                                                                                   | `void`      |
| `GCH_UpdateOutput(int ControllerId)`                                                                                                                                                                                                               | `void`      |
| `GCH_GetInputState(int DeviceId, FInputContext* OutInputState)`                                                                                                                                                                                    | `bool`      |
| `GCH_GetDeviceDescriptor(int DeviceId, GamepadDeviceDescriptor* OutDescriptor)`                                                                                                                                                                    | `bool`      |
| `GCH_DeviceIsConnected(int ControllerId)`                                                                                                                                                                                                          | `bool`      |
| `GCH_GetDeviceType(int ControllerId)`                                                                                                                                                                                                              | `int`       |
| `GCH_GetConnectionType(int ControllerId)`                                                                                                                                                                                                          | `int`       |
| `GCH_BatteryLevelDevice(int ControllerId)`                                                                                                                                                                                                         | `float`     |
| `GCH_Lightbar(int ControllerId, std::uint8_t R, std::uint8_t G, std::uint8_t B)`                                                                                                                                                                   | `void`      |
| `GCH_LightbarFlash(int ControllerId, std::uint8_t R, std::uint8_t G, std::uint8_t B, float BrightnessTime, float ToggleTime)`                                                                                                                      | `void`      |
| `GCH_PlayerLed(int ControllerId, int Led, std::uint8_t Brightness)`                                                                                                                                                                                | `void`      |
| `GCH_ResetLights(int ControllerId)`                                                                                                                                                                                                                | `void`      |
| `GCH_ResetGyroOrientation(int ControllerId)` | `void` |
| `GCH_EnableGyroscopeValues(int ControllerId, bool EnableGyroscope)` | `void` |
| `GCH_EnableTouch(int ControllerId, bool EnableTouch)` | `void` |
| `GCH_CustomTrigger(int ControllerId, const std::uint8_t* HexBytes, int ByteCount, int Hand)` | `bool` |
| `GCH_StopTrigger(int ControllerId, int Hand)` | `void` |
| `GCH_SetVibration(int ControllerId, std::uint8_t LeftRumble, std::uint8_t RightRumble)` | `void` |

The `int` parameters used for device type, connection type, LED, and trigger hand correspond to the enum values defined by the native [Gamepad-Core/Dualsense-Multiplatform](https://github.com/rafaelvaloto/Dualsense-Multiplatform) library.

**`GCH_DualSenseSettings`** allows you to configure advanced settings exclusive to the DualSense controller, including
audio control (microphone, headset, speaker), volumes, vibration modes, and force reduction on adaptive triggers.

```cpp
GCH_DualSenseSettings(int ControllerId, std::uint8_t bIsMic, std::uint8_t bIsHeadset, std::uint8_t bIsSpeaker, std::uint8_t MicVolume, std::uint8_t AudioVolume, std::uint8_t RumbleMode, std::uint8_t RumbleReduce, std::uint8_t TriggerReduce)
```

---

## 🧪 Quick Testing

Test quickly with a C# implementation or create an implementation for your preferred language. The C-compatible API is
designed to integrate seamlessly with any language that supports FFI (Foreign Function Interface), including Python,
Rust, Go, Java, and more.

### 📥 Precompiled Binaries (Windows x64)

For quick integration without building from source, **[download the precompiled GamepadCoreHost.dll](https://github.com/rafaelvaloto/Gamepad-Core/releases/download/v1.0.6/GamepadCoreHost.dll)** binary directly
from the releases page. This shared library is ready to use with any compatible host application or game engine that
supports C-compatible FFI (Foreign Function Interface).


### 🚀 Client Implementation

Check out the companion command-line application that consumes this API:
* [**Gamepad-Client (C#)**](https://github.com/rafaelvaloto/G-Client-Sharp) - Console application example consuming this

**Quick Start:**

```bash
git clone https://github.com/rafaelvaloto/G-Client-Sharp

dotnet build .\G-Client-Sharp\G-Client-Sharp.csproj
dotnet run --project .\G-Client-Sharp\G-Client-Sharp.csproj -- `"C:\path\to\GamepadCoreHost.dll"
```

---

## 🛠️ Building

**Initialization & Build:**

If you want to build your custom version, you can start with this example project and consult the available methods
at [https://github.com/rafaelvaloto/Dualsense-Multiplatform](https://github.com/rafaelvaloto/Dualsense-Multiplatform).

**Requirements:**
* CMake 4.2 or newer.
* A C++20 compatible compiler.
* The `3rdParty/Gamepad-Core` submodule included in the project setup.

Initialize the submodule and configure the project by running:

```bash
git submodule update --init
cmake -S . -B build -DGCL_DEBUG=OFF
cmake --build build
```

> **Note:** Set `GCL_DEBUG=ON` to enable Gamepad-Core host logging. The build process will generate the `GamepadCoreHost` shared library.

---

## 📂 Project Structure

* `Source/Public` — Public bridge policies and adapters.
* `Source/Private` — Internal bridge implementations.
* `3rdParty/Gamepad-Core` — The core dependency submodule.

---

## 🤝 Contributing

All suggestions and collaborations are welcome! Feel free to open issues, submit pull requests, or share your ideas to
help improve this project.

---

## 📄 License

This project utilizes the Gamepad-Core (Dualsense-Multiplatform) library.

Copyright (c) 2026 valoto.games. All rights reserved.
