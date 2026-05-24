cmake_minimum_required(VERSION 3.10)
project(MyApp)

set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)
set(NDK_GLUE ${ANDROID_NDK}/sources/android/native_app_glue)

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    "${PROJECT_ROOT}/src/*.cpp"
    "${PROJECT_ROOT}/src/*.hpp"
    "${PROJECT_ROOT}/src/*.c"
    "${PROJECT_ROOT}/src/*.h"
    "${PROJECT_ROOT}/include/*.cpp"
    "${PROJECT_ROOT}/include/*.hpp"
    "${PROJECT_ROOT}/include/*.c"
    "${PROJECT_ROOT}/include/*.h"
)

file(GLOB LOCAL_SOURCES CONFIGURE_DEPENDS
    src/main/cpp/*.cpp
    src/main/cpp/*.hpp
    src/main/cpp/*.c
    src/main/cpp/*.h
)

add_library(native-lib SHARED
    ${CORE_SOURCES}
    ${LOCAL_SOURCES}
)

add_library(native-app-glue STATIC
    ${NDK_GLUE}/android_native_app_glue.c
)

target_include_directories(native-lib PRIVATE
    ${PROJECT_ROOT}/include
    ${PROJECT_ROOT}/src
    ${NDK_GLUE}
)

target_link_options(native-lib PRIVATE
    "-Wl,--undefined=ANativeActivity_onCreate"
)

# Linka as bibliotecas necessárias para o aplicativo Android, incluindo a biblioteca de log, a biblioteca de Android, a biblioteca EGL e a biblioteca OpenGL ES 3.0
find_library(log-lib log)
find_library(android-lib android)
find_library(egl-lib EGL)
find_library(gles-lib GLESv3)

target_link_libraries(native-lib
    native-app-glue
    ${log-lib}
    ${android-lib}
    ${egl-lib}
    ${gles-lib}
)