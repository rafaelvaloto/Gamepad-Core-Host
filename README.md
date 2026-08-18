# Gamepad-Core Host Bridge

A Gamepad-Core Host Bridge é uma biblioteca nativa em C++ que expõe a funcionalidade do Gamepad-Core através de uma API de alto nível compatível com C. A versão atual do projeto é 1.0.6[cite: 2].

A API exportada foi projetada para ser consumida pelo Host através de C# P/Invoke, mas também pode ser utilizada por outras linguagens que suportem bibliotecas dinâmicas nativas e callbacks de ponteiro de função.

---

## Funcionalidades

* Integração de registro de dispositivos para o Host ou outra engine.
* Callbacks de ponte da plataforma para detecção de dispositivos e I/O.
* Suporte para criação e invalidação de handles de dispositivos na plataforma.
* Encaminhamento de operações de input, output e áudio-haptics.
* Pontos de entrada exportados compatíveis com C para integrações de linguagens gerenciadas.
* Configuração de callbacks de log para depuração direta do host[cite: 2].

---

## Compilação

Requisitos:

* CMake 4.2 ou mais recente[cite: 2].
* Um compilador compatível com C++20[cite: 2].
* O submódulo `3rdParty/Gamepad-Core` incluído na configuração do projeto[cite: 2].

Inicialize o submódulo e configure o projeto:

    git submodule update --init
    cmake -S . -B build -DGCL_DEBUG=OFF
    cmake --build build

A variável `GCL_DEBUG` pode ser ativada para habilitar o log do host do Gamepad-Core[cite: 2]. A compilação produzirá a biblioteca compartilhada `GamepadCoreHost`[cite: 2].

---

## API Nativa

A aplicação host deve seguir os seguintes passos de ciclo de vida e atualização:

1. Inicializar os callbacks de ponte de plataforma chamando `GCH_InitializePlatformBridge`[cite: 9].
2. Registrar os callbacks de alocação, despacho e desconexão via `GCH_InitializeDeviceRegistryPolicy`[cite: 9].
3. Descobrir novos dispositivos chamando `GCH_DiscoverDevices` a partir do loop de atualização[cite: 5].
4. Atualizar o estado de cada controle com `GCH_UpdateInput`[cite: 5].
5. Recuperar os estados de input e informações do hardware do dispositivo através de `GCH_GetInputState` e `GCH_GetDeviceDescriptor`[cite: 5].

### Callbacks e Encerramento

* O log da ponte pode ser redirecionado configurando um callback de função através de `GCH_SetLogCallback`[cite: 2].
* A versão atual compilada da biblioteca pode ser obtida por `GCH_GetVersion`[cite: 2].
* Ao finalizar a execução, a aplicação host deve chamar a função `GCH_Shutdown` para destruir a instância da plataforma e zerar os ponteiros de callback com segurança[cite: 2].

---

## Estrutura do Projeto

* `Source/Public` — políticas e adaptadores públicos da ponte.
* `Source/Private` — implementações da ponte.
* `3rdParty/Gamepad-Core` — submódulo da dependência.

---

## Licença

Este projeto utiliza a biblioteca Gamepad-Core (Dualsense-Multiplatform).

Copyright (c) 2026 valoto.games. Todos os direitos reservados[cite: 2].