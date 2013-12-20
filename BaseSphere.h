#pragma once
#include "Utils.h"
#include "Camera.h"
class BaseSphere
{
public:
    void Init( float rotation_speed, float radius, float orbit_speed, float distance_from_sun, std::string texture_path, BaseSphere* object_to_rotate_around );
    void Update(float delta_time, Camera* camera);
    void Cleanup();
    void Draw();
    float GetLocationX();
    float GetLocationY();
    float GetLocationZ();
    float GetRadius();

protected:

    // Helpers
    GLuint LoadTexture(const char * location);
    bool LoadOBJ( const char * path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals );
    std::string ReadFile(const char* file_path);
    GLuint LoadShader(const char*vertex_path, const char*fragment_path);
    bool BaseSphere::OpenGLStatus();

    BaseSphere * m_oRotateAround;

    float m_fOrbitSpeed;
    float m_fRotationSpeed;
    float m_fRadius;

    // These will go away eventually since everything is centered
    float m_fPositionX;
    float m_fPositionY;
    float m_fPositionZ;
    float m_fDistanceFromSun;


    // All of our OpenGL index id's
    GLuint m_iShaderProgram;
    GLuint m_iTexture;
    GLuint m_iUVBuffer;
    GLuint m_iVertexBuffer;
    GLuint m_iVertexArrayID;

    std::vector< glm::vec3 > m_v3Vertices;
    std::vector< glm::vec2 > m_v2UV;
    std::vector< glm::vec3 > m_v2Normals; // Won't be used at the moment

    std::string m_sTexturePath;

};