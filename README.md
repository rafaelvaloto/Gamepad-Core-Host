# Gamepad-Core Host Bridge

Gamepad-Core Host Bridge is a native C++ library that exposes the Gamepad-Core functionality through a high-level C-compatible API.

The exported API is designed to be consumed by Host through C# P/Invoke, but it can also be used from other languages that support native dynamic libraries and function-pointer callbacks. The bridge keeps the platform-specific implementation outside the core library and forwards device operations to callbacks supplied by the host application.

## Features

- Device registry integration for Host or another host engine.
- Platform bridge callbacks for device detection and I/O.
- Support for creating and invalidating platform device handles.
- Forwarding of input, output, and audio-haptic operations.
- C-compatible exported entry points for managed-language integrations.

## Building

Requirements:

- CMake 3.20 or newer.
- A C++20-compatible compiler.
- The `3rdParty/Gamepad-Core` submodule.

Initialize the submodule and configure the project:

```bash
git submodule update --init
cmake -S . -B build
cmake --build build
```

The build produces the `GamepadCoreBridge` shared library.

## Native API

The public API is declared in [`GamepadApi.h`](Source/Public/GamepadApi.h). The host application should:

1. Register the platform callbacks with `GCH_InitializePlatformBridge`.
2. Register the device registry callbacks with `GCH_InitializeDeviceRegistryPolicy`.
3. Call `GCH_DiscoverDevices` from the host application's update loop.

The platform callbacks are responsible for detecting devices, reading input, writing output, creating and invalidating handles, and processing audio haptics. The device registry callbacks notify the host when a new device is allocated, dispatched, or disconnected.

`FDeviceContext` is an opaque native context in the public API. Callback implementations must use the ABI and data layout expected by the native bridge.

## Project structure

- `GamepadApi.h` — exported API and callback declarations.
- `Source/Public` — public bridge policies and adapters.
- `Source/Private` — bridge implementations.
- `3rdParty/Gamepad-Core` — Gamepad-Core library submodule.

## License

Copyright (c) 2026 valoto.games. All rights reserved.
