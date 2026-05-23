// @file GLImage.hpp
// @brief Declaração da classe GLImage para gerenciar recursos de textura no OpenGL
// @details Esta classe encapsula o processo de carregamento, configuração e limpeza dos recursos OpenGL associados a uma textura.
//
// @author Natanael de Sousa
// @date 2026-05-23
// ------------------------------------------------------------------------------------------------------
//
#pragma once

// Define STBI_rgb_alpha para garantir que as imagens sejam carregadas com 4 canais (RGBA)
#ifndef STBI_rgb_alpha
#define STBI_rgb_alpha 4
#endif

// Gerencia o carregamento de recursos para o programa (em desenvolvimento, pode ser expandida para incluir mais tipos de recursos no futuro)
class GLImage
{
private:
    unsigned int asset_id; // ID do recurso de textura OpenGL

public:
    bool Load(const char *path); // Função para carregar recursos de textura usando stb_image e criar texturas OpenGL
    unsigned int Get() const;    // Função para obter o ID da textura OpenGL carregada
    void Delete();               // Função para deletar a textura OpenGL e liberar os recursos associados
};