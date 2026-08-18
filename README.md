# 🎮 Gamepad-Core Host Bridge

**Gamepad-Core Host Bridge** is a native C++ library that exposes the functionality of Gamepad-Core through a high-level, C-compatible API. The current project version is **1.0.6**.

Designed to be consumed by the Host via C# P/Invoke, the exported API can also be easily integrated into other languages that support native dynamic libraries and function pointer callbacks.

---

## ✨ Features

* **Device Registry Integration:** Support for host applications or external game engines.
* **Platform Bridge Callbacks:** Efficient device detection and I/O operations.
* **Handle Management:** Support for creating and invalidating device handles natively within the platform.
* **Advanced Forwarding:** Seamless forwarding of input, output, and audio-haptics operations.
* **C-Compatible Entry Points:** Exported endpoints optimized for managed language integrations.
* **Direct Debugging:** Configurable log callbacks for real-time host debugging.

---

## 🛠️ Building

**Requirements:**
* CMake 4.2 or newer.
* A C++20 compatible compiler.
* The `3rdParty/Gamepad-Core` submodule included in the project setup.

**Initialization & Build:**
Initialize the submodule and configure the project by running:

```bash
git submodule update --init
cmake -S . -B build -DGCL_DEBUG=OFF
cmake --build build
```

> **Note:** Set `GCL_DEBUG=ON` to enable Gamepad-Core host logging. The build process will generate the `GamepadCoreHost` shared library.

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

## 🚀 Client Implementation

Check out the companion command-line application that consumes this API:
* [**Gamepad-Client (rafaelvaloto/G-Client-Sharp)**](https://github.com/rafaelvaloto/G-Client-Sharp) - A C# console application for consuming the native Gamepad Core Host API through callbacks and Windows HID interoperability.

---

## 📂 Project Structure

* `Source/Public` — Public bridge policies and adapters.
* `Source/Private` — Internal bridge implementations.
* `3rdParty/Gamepad-Core` — The core dependency submodule.

---

## 📄 License

This project utilizes the Gamepad-Core (Dualsense-Multiplatform) library.

Copyright (c) 2026 valoto.games. All rights reserved.