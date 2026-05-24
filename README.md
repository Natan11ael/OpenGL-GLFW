# OpenGL GLFW Sources

**Descrição**

Projeto reutilizavel para criação de projeto em diferentes contexto simplificando o uso do GLFW, Template inical pronto tanto para android quanto par windows.

**Status do projeto**

- Status: Indefinida (apenas projeto template)
- Plataforma: Android e Windows
- Linguagens principais: C++, Java, CMake

## Estrutura do repositório

O projeto contém aplicação Android e Windows em um mesmo repositório.

```
.vscode
include
├── Engine
├── GL
├── glad
├── GLFW
├── glm
├── KHR
└── stb
src
└── assets
android
├── app
│    ├── src\main
│    │   ├── cpp
│    │   ├── java
│    │   ├── res
│    │   └── AndroidManifest.xml
│    └── CMakeLists.txt
├── gradlew
└── settings.gradle
windows
├── bin
│    ├── glew32.dll
│    └── glfw3.dll
├── lib
│    ├── glew32.lib
│    ├── glew32s.lib
│    └── glfw3.lib
├── src
│    ├── GLWindow.cpp
│    ├── GLWindow.hpp
│    └── main.cpp
└── CMakeLists.txt
build
├── windows
└── android
.gitignore
CMakeLists.txt
ARCHITECTURE.md
CONTRIBUTING.md
LICENSE
README.md
```

## Como compilar

### Windows

```powershell
cmake -B build
cmake --build build
```

### Android

```powershell
cmd /c "cd android && gradlew clean assembleDebug"
```

Para iniciar o emulador Android:

```powershell
emulator -avd Medium_Phone_API_36.1 -gpu swiftshader_indirect -no-snapshot -no-boot-anim
```

## Licença

O arquivo `LICENSE` define a licença escolhida para este projeto.