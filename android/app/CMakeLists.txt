cmake_minimum_required(VERSION 3.10)
project(MyApp)

set(NDK_GLUE ${ANDROID_NDK}/sources/android/native_app_glue)

add_library(native-lib SHARED
    src/main/cpp/main.cpp
)

add_library(native-app-glue STATIC
    ${NDK_GLUE}/android_native_app_glue.c
)

target_include_directories(native-lib PRIVATE
    ${NDK_GLUE}
)

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