#pragma once
#include "BaseSphere.h"

class Sun : public BaseSphere
{
public:
    void InitRotation( bool rotation, int rotation_speed, double radius );
    void Update(float delta_time, Camera* camera);
    void Draw();
    void Cleanup();
    std::string ReadFile(const char* file_path);
    GLuint LoadShader(const char*vertex_path, const char*fragment_path);
    Sun();
    GLuint Sun::LoadTexture(const char * location);

private:
    GLuint m_iShaderProgram;
    GLuint m_iTexture;
    GLuint m_iUVBuffer;
    GLuint m_iVertexBuffer;
    GLuint m_iVertexArrayID;
};
