#pragma once
#include "Utils.h"

class Camera
{
public:
    void Init();
    void Update(float delta_time);
    void HandleInput(SDL_Event* event, float delta_time);
    void CleanUp();
    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 Orientation() const;
    glm::mat4 GetViewMatrix() const;
    glm::vec3 Forward() const;
    glm::vec3 Right() const;
    glm::vec3 Up() const;
    glm::mat4 GetMatrix() const;
    void OffsetPosition(const glm::vec3& offset);
    void Camera::OffsetOrientation(float up_angle, float right_angle);
    void Camera::NormalizeAngles();
    int Camera::GetX();

    int Camera::GetY();

private:
    glm::vec3 m_v3Position;
    float m_fSpeed;
    float m_fMouseSpeed;

    float m_fFieldOfView;
    float m_fHorizontalAngle;
    float m_fVerticalAngle;
    float m_fNearPlane;
    float m_fFarPlane;
    float m_fViewportAspectRatio;


    int m_iMouseX;
    int m_iMouseY;
    int m_iMouseScroll;
    bool m_bUp;
    bool m_bRight;
    bool m_bDown;
    bool m_bLeft;
    bool m_bForward;
    bool m_bBackward;
};