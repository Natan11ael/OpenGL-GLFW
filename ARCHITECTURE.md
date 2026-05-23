# Arquitetura do Between Worlds

Use este documento para explicar a arquitetura geral do projeto e como os componentes se conectam.

## Visão geral

Descreva o propósito da arquitetura e os principais requisitos do sistema.

## Objetivos

- Plataforma multiplataforma: Android e Windows.
- Reuso de código quando possível.
- Separação clara entre núcleo, plataforma Android e plataforma Windows.

## Componentes principais

### Android

- `core/android/app/src/main/cpp/` – código nativo C++ para Android.
- `core/android/app/src/main/java/com/example/app/` – código Java de interface Android.
- `core/android/app/src/main/AndroidManifest.xml` – manifesto Android.
- `core/android/app/build.gradle` – definições de build do módulo.
- `core/android/CMakeLists.txt` – configuração do build nativo Android.

### Windows

- `core/windows/src/main.cpp` – entrada principal do aplicativo Windows.
- `core/windows/CMakeLists.txt` – configuração do build Windows.
- `core/windows/include/` – cabeçalhos de dependências.
- `core/windows/lib/` – bibliotecas vinculadas.

## Estrutura de pastas

Explique aqui como as pastas estão organizadas e por que a divisão foi feita dessa forma.

## Fluxo de build

### Android

1. Executar `gradlew assembleDebug`.
2. O Gradle chama o CMake para construir o código nativo.
3. O APK é gerado em `core/android/app/build/outputs/apk/`.

### Windows

1. Executar `cmake --build build --config Debug` em `core/windows`.
2. O build gera o executável e os artefatos de build em `build/bin/Debug`.

## Dependências

Liste aqui as dependências principais do projeto:

- Android SDK/NDK
- Gradle
- CMake
- GLFW
- GLEW

## Possíveis melhorias

Adicione notas sobre extensões futuras da arquitetura, modularização ou refatorações planejadas.
