# Between Worlds

**Descrição**

Substitua este texto por uma descrição breve do projeto, seu propósito e os resultados esperados.

**Status do projeto**

- Status: Em desenvolvimento / Em produção / Em protótipo
- Plataforma: Android e Windows
- Linguagens principais: C++, Java, CMake

## Estrutura do repositório

O projeto contém aplicação Android e Windows em um mesmo repositório.

```
.vscode
windows
├── bin
│    ├── glew32.dll
│    └── glfw3.dll
├── build
│    └── bin\Debug
├── core/windows/build/bin/Debug
├── include
│    ├── GL
│    └── GLFW
├── lib
│    ├── glew32.lib
│    ├── glew32s.lib
│    └── glfw3.lib
├── src
│    └── main.cpp
└── CMakeLists.txt
ARCHITECTURE.md
CONTRIBUTING.md
LICENSE
README.md
```

## Arquivos importantes

- `core/windows/CMakeLists.txt` – projeto Windows.
- `core/windows/src/main.cpp` – ponto de entrada do aplicativo Windows.
- `core/windows/lib/` – bibliotecas vinculadas para Windows.
- `core/windows/include/` – cabeçalhos necessários para Windows.

## Como compilar

### Windows

```powershell
cd windows
cmake --build build --config Debug
```

## Uso

Descreva aqui quais passos são necessários para executar o aplicativo, quais parâmetros são aceitos e como testar as principais funcionalidades.

## Contribuição

Consulte `CONTRIBUTING.md` para orientações de contribuição, fluxo de trabalho de branches e padrões de commit.

## Licença

O arquivo `LICENSE` define a licença escolhida para este projeto.
