// @file GLImage.cpp
// @brief Implementação da classe GLImage para gerenciar recursos de textura no OpenGL
// @details Esta classe encapsula o processo de carregamento, configuração e limpeza dos recursos OpenGL associados a uma textura.
//
// @author Natanael de Sousa
// @date 2026-05-23
// ------------------------------------------------------------------------------------------------------
//

// stb_image - v2.27 - public domain image loader - http://nothings.org/stb_image.h
// no warranty implied; use at your own risk
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

// OpenGL e GLFW headers
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Engine headers
#include "GLImage.hpp"

// C++ standard library
#include <iostream>

// Função para carregar recursos de textura usando stb_image e criar texturas OpenGL
bool GLImage::Load(const char *path)
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char *data = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
    if (!data)
    {
        std::cerr << "Falha ao carregar imagem: " << path << "\n";
        return false;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
    asset_id = tex;
    return true;
}

// Função para obter o ID da textura OpenGL carregada
unsigned int GLImage::Get() const
{
    return asset_id;
}

// Função para deletar a textura OpenGL e liberar os recursos associados
void GLImage::Delete()
{
    glDeleteTextures(1, &asset_id);
}