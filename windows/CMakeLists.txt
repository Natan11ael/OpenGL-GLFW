# 1. Busca automática e recursiva de todos os arquivos fontes e headers
file(GLOB_RECURSE WIN_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)
message(STATUS "WIN = ${WIN_SOURCES}")

# 2. Cria o executável usando a lista gerada dinamicamente
add_executable(MyApp ${CORE_SOURCES} ${WIN_SOURCES})

target_compile_features(MyApp PRIVATE cxx_std_17)

# 3. Inclui a pasta base e subpastas de cabeçalhos de forma recursiva (opcional para subpastas)
target_include_directories(MyApp PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src       # Permite dar include em arquivos dentro de src/
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

# Remove os conflitos de Runtime entre bibliotecas estáticas e dinâmicas
target_link_options(MyApp PRIVATE "/IGNORE:4099")
target_link_options(MyApp PRIVATE "/NODEFAULTLIB:LIBCMT")
target_link_options(MyApp PRIVATE "/NODEFAULTLIB:MSVCRT")

# copia assets para a pasta final do executável
add_custom_command(TARGET MyApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/assets
    $<TARGET_FILE_DIR:MyApp>/src/assets
)

# execute the built app automatically after a successful build
add_custom_command(TARGET MyApp POST_BUILD
    COMMAND cd ${CMAKE_SOURCE_DIR} && $<TARGET_FILE:MyApp>
)