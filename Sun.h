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
    SDL_Surface* GetSurfaceAndMode(const char * location, int *mode);

private:
    GLuint m_iShaderProgram;
    GLfloat m_fSpeed;
    GLfloat m_fAngle;
};
