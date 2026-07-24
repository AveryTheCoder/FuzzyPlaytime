#pragma once

#include "raylib.h"

class ShaderManager {
public:
    ShaderManager();
    ~ShaderManager();

    bool LoadShader(const char* vertexPath, const char* fragmentPath);
    Shader GetShader() const;

private:
    void UnloadShaderInternal();
    Shader shader;
    bool loaded;
};
