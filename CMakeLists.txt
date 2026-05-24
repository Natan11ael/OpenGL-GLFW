cmake_minimum_required(VERSION 3.10)

project(MyApp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 1. Busca automática e recursiva de todos os arquivos fontes e headers
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/src/*.h"
    "${CMAKE_SOURCE_DIR}/include/*.cpp"
    "${CMAKE_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_SOURCE_DIR}/include/*.c"
    "${CMAKE_SOURCE_DIR}/include/*.h"
)
message(STATUS "CORE = ${CORE_SOURCES}")

# Transforma CORE_SOURCES em global para que as subpastas consigam lê-la
set(CORE_SOURCES ${CORE_SOURCES} CACHE INTERNAL "Fontes comuns do projeto")

# 2. Adiciona as subpastas do projeto e compila os arquivos fontes
add_subdirectory(windows)