#include "include/shaders.h"

ShaderManager::ShaderManager() : shader{}, loaded(false) {}
ShaderManager::~ShaderManager() { UnloadShaderInternal(); }

bool ShaderManager::LoadShader(const char* vertexPath, const char* fragmentPath)
{
    if (loaded) UnloadShaderInternal();
    shader = ::LoadShader(vertexPath, fragmentPath);
    loaded = true;
    return true;
}

void ShaderManager::UnloadShaderInternal()
{
    if (loaded)
    {
        ::UnloadShader(shader);
        loaded = false;
    }
}

Shader ShaderManager::GetShader() const { return shader; }
