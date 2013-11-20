#pragma once
#include "Utils.h"

class Camera
{
public:
    void Init();
    void Update(float delta_time);
    void HandleInput(SDL_Event* event);
    void CleanUp();
    glm::mat4 GetProjectionMatrix();
    glm::mat4 GetViewMatrix();

private:
    glm::vec3 m_v3Position;
    float m_fSpeed;
    float m_fInitialFieldOfView;
    float m_fMouseSpeed;
    float m_fHorizontalAngle;
    float m_fVerticalAngle;
    int m_iMouseX;
    int m_iMouseY;
    int m_iMouseScroll;
    bool m_bUp;
    bool m_bRight;
    bool m_bDown;
    bool m_bLeft;

    glm::mat4 m_m4ProjectionMatrix;
    glm::mat4 m_m4ViewMatrix;
};