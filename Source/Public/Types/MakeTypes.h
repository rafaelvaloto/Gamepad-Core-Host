// Project: Gamepad-Core-Host
// This project uses the Gamepad-Core library (Dualsense-Multiplatform) https://github.com/rafaelvaloto/Dualsense-Multiplatform
// Copyright (c) 2026 valoto.games. All rights reserved.

#pragma once
#include "GCHTypes.h"
#include "GCore/Types/Structs/Context/DeviceContext.h"

/**
 * Namespace que contiene utilidades de conversión entre tipos de contexto
 * de dispositivo del host y descriptores de dispositivo de Gamepad-Core.
 */
namespace GCH {
    /**
     * Crea un contexto de dispositivo (FDeviceContext) a partir de un descriptor
     * de dispositivo de gamepad (GamepadDeviceDescriptor).
     *
     * Esta función convierte la información del descriptor del dispositivo en un
     * formato de contexto utilizado internamente por el host.
     *
     * @param Descriptor Referencia constante al descriptor del dispositivo gamepad
     *                   que contiene la información del hardware.
     * @return Un objeto FDeviceContext inicializado con los datos del descriptor.
     */
    FDeviceContext MakeDeviceContext(const GamepadDeviceDescriptor &Descriptor);

    /**
     * Crea un descriptor de dispositivo de gamepad (GamepadDeviceDescriptor) a partir
     * de un contexto de dispositivo (FDeviceContext).
     *
     * Esta función realiza la conversión inversa, transformando el contexto interno
     * del host en un descriptor de dispositivo compatible con Gamepad-Core.
     *
     * @param Context Puntero al contexto del dispositivo que contiene información
     *                sobre el estado y configuración actual del gamepad.
     * @return Un objeto GamepadDeviceDescriptor con la información extraída del contexto.
     */
    GamepadDeviceDescriptor MakeDeviceDescriptor(FDeviceContext* Context);
}
