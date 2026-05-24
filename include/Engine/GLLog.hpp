// @file main.cpp
// @brief
// @details
//
// @author Natanael de Sousa
// @date 2026-05-24
// ------------------------------------------------------------------------------------------------------
//
#ifdef ANDROID
#include <android/log.h>
#include <cstdlib>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "GL", __VA_ARGS__)
#define LOGE(...)                                                  \
    do                                                             \
    {                                                              \
        __android_log_print(ANDROID_LOG_ERROR, "GL", __VA_ARGS__); \
        abort();                                                   \
    } while (0)
#else

#include <cstdio>
#include <cstdlib>
#define LOGI(...) printf("[INFO] " __VA_ARGS__), printf("\n")
#define LOGE(...)                                \
    do                                           \
    {                                            \
        fprintf(stderr, "[ERROR] " __VA_ARGS__); \
        fprintf(stderr, "\n");                   \
        abort();                                 \
    } while (0)
#endif