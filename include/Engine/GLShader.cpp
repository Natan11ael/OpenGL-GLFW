// @file GLShader.cpp
// @brief Gerencia Shaders do programa, incluindo compilação, linkagem e uso do programa de shader OpenGL
// @details Esta classe encapsula o processo de carregamento, compilação e linkagem dos shaders de vértice e fragmento a partir de arquivos de shader. Ela também fornece métodos para usar o programa de shader durante a renderização e para limpar os recursos associados quando não forem mais necessários.
//
// @author Natanael de Sousa
// @date 2026-05-23
// ------------------------------------------------------------------------------------------------------
//

// OpenGL e GLFW headers
#ifdef ANDROID
#include <GLES3/gl3.h>
#define ASSETS_PATH ""
#else
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define ASSETS_PATH "src/assets/"
#endif

// Engine headers
#include "Engine/GLShader.hpp"
#include "Engine/GLLog.hpp"

#include <string>

// Destrutor para limpar os recursos do programa de shader OpenGL
GLShader::~GLShader()
{
    Delete();
}

// Função para compilar um shader a partir do código-fonte do shader
unsigned int GLShader::GetCompileShader(unsigned int shader_type, const char *shader_source)
{
    unsigned int shader_id = glCreateShader(shader_type); // Cria um shader do tipo especificado (GL_VERTEX_SHADER ou GL_FRAGMENT_SHADER)

#ifdef ANDROID
    const char *version = (shader_type == GL_FRAGMENT_SHADER) ? "#version 300 es\nprecision highp float;\n" : "#version 300 es\n";
#else
    const char *version = "#version 330 core\n";
#endif

    const char *sources[] = { version, shader_source };

    glShaderSource(shader_id, 2, sources, NULL);
    glCompileShader(shader_id);

    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_id, 1024, NULL, infoLog);
        LOGE("SHADER_COMPILATION_ERROR: %s", infoLog);
    }

    return shader_id;
}

// Função para carregar, compilar e linkar os shaders de vértice e fragmento a partir dos arquivos de shader
bool GLShader::Load(const char *vertex_path, const char *fragment_path
#ifdef ANDROID
    ,
    AAssetManager *am
#endif
)
{
    std::string vs_path = std::string(ASSETS_PATH) + vertex_path;
    std::string fs_path = std::string(ASSETS_PATH) + fragment_path;

#ifdef ANDROID
    //
    AAsset *vs_asset = AAssetManager_open(am, vs_path.c_str(), AASSET_MODE_BUFFER);
    AAsset *fs_asset = AAssetManager_open(am, fs_path.c_str(), AASSET_MODE_BUFFER);

    std::string f_vs((const char *)AAsset_getBuffer(vs_asset), AAsset_getLength(vs_asset));
    std::string f_fs((const char *)AAsset_getBuffer(fs_asset), AAsset_getLength(fs_asset));

    AAsset_close(vs_asset);
    AAsset_close(fs_asset);
#else
    //
    std::ifstream is_vs(vs_path), is_fs(fs_path);

    std::stringstream buffer_vs, buffer_fs;
    buffer_vs << is_vs.rdbuf();
    buffer_fs << is_fs.rdbuf();

    const std::string f_vs = buffer_vs.str();
    const std::string f_fs = buffer_fs.str();
#endif

    //
    m_program = glCreateProgram();

    //
    unsigned int vs = GetCompileShader(GL_VERTEX_SHADER, f_vs.c_str());
    unsigned int fs = GetCompileShader(GL_FRAGMENT_SHADER, f_fs.c_str());

    //
    glAttachShader(m_program, vs);
    glAttachShader(m_program, fs);

    //
    glLinkProgram(m_program);
    glValidateProgram(m_program);

    //
    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_program, 1024, NULL, infoLog);
        LOGE("PROGRAM_LINKING_ERROR: %s", infoLog);
    }

    //
    glDeleteShader(vs);
    glDeleteShader(fs);
    return true; //
}

// F
unsigned int GLShader::Get() const
{
    return m_program;
}

// Função para usar o programa de shader OpenGL para renderização
void GLShader::Use()
{
    glUseProgram(m_program);
}

// Função para deletar o programa de shader OpenGL e liberar os recursos associados
void GLShader::Delete()
{
    glDeleteProgram(m_program);
}