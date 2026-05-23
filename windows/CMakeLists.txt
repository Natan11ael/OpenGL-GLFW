cmake_minimum_required(VERSION 3.10)

project(MyApp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 1. Busca automática e recursiva de todos os arquivos fontes e headers
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.hpp"
    "src/*.c"
    "src/*.h"
    "include/*.cpp"
    "include/*.hpp"
    "include/*.c"
    "include/*.h"
)

# 2. Cria o executável usando a lista gerada dinamicamente
add_executable(MyApp ${SOURCES})

target_compile_features(MyApp PRIVATE cxx_std_17)

# 3. Inclui a pasta base e subpastas de cabeçalhos de forma recursiva (opcional para subpastas)
target_include_directories(MyApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src       # Permite dar include em arquivos dentro de src/
)

# libs
target_link_directories(MyApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# define GLEW static (compatível com glew32s.lib)
target_compile_definitions(MyApp PRIVATE GLEW_STATIC)

target_link_libraries(MyApp
    glfw3
    glew32s
    opengl32
)

# output organizado igual Android
set_target_properties(MyApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin
)

# Remove os conflitos de Runtime entre bibliotecas estáticas e dinâmicas
target_link_options(MyApp PRIVATE "/IGNORE:4099")
target_link_options(MyApp PRIVATE "/NODEFAULTLIB:LIBCMT")
target_link_options(MyApp PRIVATE "/NODEFAULTLIB:MSVCRT")

# execute the built app automatically after a successful build
add_custom_command(TARGET MyApp POST_BUILD
    COMMAND $<TARGET_FILE:MyApp>
)