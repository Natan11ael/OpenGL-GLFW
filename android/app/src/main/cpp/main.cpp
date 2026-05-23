// OpenGLApp - Exemplo de aplicação OpenGL usando GLEW, GLFW e stb_image
// Este código é um exemplo básico de como configurar uma aplicação OpenGL para renderizar uma textura usando shaders. Ele inclui a criação de um programa de shader, o carregamento de uma textura a partir de um arquivo usando stb_image, e a renderização de um quadrado cobrindo toda a tela com a textura aplicada.
//
//
#ifdef __cplusplus
extern "C" void app_dummy();
#endif
//
// Android Tools - https://developer.android.com/studio
// Android NDK - https://developer.android.com/ndk
#include <android_native_app_glue.h>
#include <android/asset_manager.h>

// EGL e OpenGLES
#include <EGL/egl.h>
#include <GLES2/gl2.h>

// stb_image - v2.27 - public domain image loader - http://nothings.org/stb_image.h
// no warranty implied; use at your own risk
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Define o caminho para a textura a ser carregada
const char *IMAGE_PATH = "texture.png";

// Variáveis globais para EGL
static EGLDisplay display = EGL_NO_DISPLAY;
static EGLSurface surface = EGL_NO_SURFACE;
static EGLContext context = EGL_NO_CONTEXT;

// Variáveis globais para o programa de shader e a textura
static GLuint program = 0;
static GLuint tex = 0;

// Variável global para armazenar o ponteiro para a estrutura android_app
static android_app *g_app = nullptr;

// Vertex shader que passa as coordenadas de textura para o fragment shader
static const char *vs =
    "attribute vec2 pos;"
    "attribute vec2 uv;"
    "varying vec2 vUV;"
    "void main(){"
    "vUV=uv;"
    "gl_Position=vec4(pos,0.0,1.0);"
    "}";

// Fragment shader que amostra a textura usando as coordenadas de textura interpoladas
static const char *fs =
    "precision mediump float;"
    "varying vec2 vUV;"
    "uniform sampler2D tex;"
    "void main(){"
    "gl_FragColor=texture2D(tex,vUV);"
    "}";

// Compila um shader de vértice ou fragmento
GLuint compile(GLenum t, const char *s)
{
    GLuint sh = glCreateShader(t);
    glShaderSource(sh, 1, &s, 0);
    glCompileShader(sh);
    return sh;
}

// Cria um programa de shader a partir dos códigos-fonte dos shaders de vértice e fragmento
GLuint link()
{
    GLuint v = compile(GL_VERTEX_SHADER, vs);
    GLuint f = compile(GL_FRAGMENT_SHADER, fs);
    GLuint p = glCreateProgram();
    glAttachShader(p, v);
    glAttachShader(p, f);
    glBindAttribLocation(p, 0, "pos");
    glBindAttribLocation(p, 1, "uv");
    glLinkProgram(p);
    return p;
}

// Carrega uma textura usando o Asset Manager do Android e cria uma textura OpenGL
GLuint loadTextureFromAssets(AAssetManager *mgr, const char *path)
{
    AAsset *asset = AAssetManager_open(mgr, path, AASSET_MODE_BUFFER);
    if (!asset)
        return 0;

    size_t size = AAsset_getLength(asset);
    unsigned char *buffer = new unsigned char[size];
    AAsset_read(asset, buffer, size);
    AAsset_close(asset);

    int w, h, c;
    unsigned char *img = stbi_load_from_memory(buffer, size, &w, &h, &c, 4);
    delete[] buffer;

    if (!img)
        return 0;

    GLuint t;
    glGenTextures(1, &t);
    glBindTexture(GL_TEXTURE_2D, t);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(img);
    return t;
}

// Inicializa EGL e configura o contexto OpenGLES 2.0
void initEGL()
{
    EGLConfig cfg;
    EGLint n;

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, 0, 0);

    EGLint attr[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE};

    eglChooseConfig(display, attr, &cfg, 1, &n);

    EGLint ctxAttr[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    context = eglCreateContext(display, cfg, EGL_NO_CONTEXT, ctxAttr);

    surface = eglCreateWindowSurface(display, cfg, (EGLNativeWindowType)g_app->window, nullptr);
    eglMakeCurrent(display, surface, surface, context);

    program = link();
    tex = loadTextureFromAssets(g_app->activity->assetManager, IMAGE_PATH);

    glClearColor(0, 0, 0, 1);
}

// Limpa recursos EGL
void destroyEGL()
{
    if (display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (context != EGL_NO_CONTEXT)
            eglDestroyContext(display, context);

        if (surface != EGL_NO_SURFACE)
            eglDestroySurface(display, surface);

        eglTerminate(display);
    }

    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;

    program = 0;
    tex = 0;
}

// Desenha um frame
void draw()
{
    if (display == EGL_NO_DISPLAY)
        return;

    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat v[] = {
        -1, -1, 0, 0,
        1, -1, 1, 0,
        -1, 1, 0, 1,
        1, 1, 1, 1};

    glUseProgram(program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(glGetUniformLocation(program, "tex"), 0);

    glVertexAttribPointer(0, 2, GL_FLOAT, 0, 16, v);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 16, v + 2);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    eglSwapBuffers(display, surface);
}

// Handle app commands (like window creation/destruction)
void handle_cmd(android_app *app, int32_t cmd)
{
    switch (cmd)
    {
    case APP_CMD_INIT_WINDOW:
        if (app->window)
        {
            initEGL();
        }
        break;

    case APP_CMD_TERM_WINDOW:
        destroyEGL();
        break;
    }
}

// Função principal do aplicativo Android
void android_main(struct android_app *app)
{
    app_dummy(); // Necessário para evitar otimizações que podem remover a função android_main

    g_app = app;
    app->onAppCmd = handle_cmd;

    while (true)
    {
        int events;
        android_poll_source *source;

        while (ALooper_pollAll(0, nullptr, &events, (void **)&source) >= 0)
        {
            if (source)
                source->process(app, source);
        }

        draw();
    }
}