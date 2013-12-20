#include "Camera.h"

// Camera Constants

static const float MaxVerticalAngle = 85.0f;
static const float CameraSpeed = 10.0f;
static const float MouseSensitivity = 1.0f;
static const float ZoomSpeed = 3.0f;


void Camera::Init()
{
    // Start the camera at the origin
    m_v3Position = glm::vec3( 0.0f, 0.0f, 1100.0f );

    // Toward -Z
    m_fHorizontalAngle = 0.0f;

    // 0 looks at the horizon
    m_fVerticalAngle = 0.0f;

    // FoV is the level of zoom. 80° = very wide angle, huge deformations. 60° – 45° : standard. 20° : big zoom.
    m_fFieldOfView = 50.0f;

    m_fNearPlane = 0.01f;

    m_fFarPlane = 100000.0f;

    m_fViewportAspectRatio = 4.0f/3.0f;


    m_iMouseX = 0;
    m_iMouseY = 0;
    m_iMouseScroll = 0;
    m_bForward = false;
    m_bRight = false;
    m_bBackward = false;
    m_bLeft = false;
    m_bUp = false;
    m_bDown = false;

}

void Camera::HandleInput(SDL_Event* event, float delta_time)
{
    int mouse_x, mouse_y;
    SDL_GetRelativeMouseState(&mouse_x, &mouse_y);
    if(event->type == SDL_KEYDOWN )
    {
        float movement_speed = CameraSpeed;
        if( event->key.keysym.sym == SDLK_UP || event->key.keysym.sym == SDLK_w )
        {
            printf("Forward x=%f, y=%f, z=%f\n", m_v3Position.x, m_v3Position.y, m_v3Position.z);
            OffsetPosition( movement_speed * Forward() );
        }
        if( event->key.keysym.sym == SDLK_DOWN || event->key.keysym.sym == SDLK_s )
        {
            printf("Backward x=%f, y=%f, z=%f\n", m_v3Position.x, m_v3Position.y, m_v3Position.z);
            OffsetPosition( movement_speed * -Forward() );
        }
        if( event->key.keysym.sym == SDLK_RIGHT || event->key.keysym.sym == SDLK_d )
        {
            printf("RIGHT x=%f, y=%f, z=%f\n", m_v3Position.x, m_v3Position.y, m_v3Position.z);
            OffsetPosition( movement_speed * Right() );
        }
        if( event->key.keysym.sym == SDLK_LEFT || event->key.keysym.sym == SDLK_a )
        {
            printf("LEFT x=%f, y=%f, z=%f\n", m_v3Position.x, m_v3Position.y, m_v3Position.z);
            OffsetPosition( movement_speed * -Right() );
        }
        if( event->key.keysym.sym == SDLK_z )
        {
            printf("Up x=%f, y=%f, z=%f\n", m_v3Position.x, m_v3Position.y, m_v3Position.z);
            OffsetPosition( movement_speed * Up() );
        }
        if( event->key.keysym.sym == SDLK_x )
        {
            printf("Up x=%f, y=%f, z=%f\n", m_v3Position.x, m_v3Position.y, m_v3Position.z);
            OffsetPosition( movement_speed * -Up() );
        }

    }

    if(event->type == SDL_MOUSEWHEEL )
    {
        // we only want the up and down scroll which is -y
        float fieldOfView = m_fFieldOfView + -ZoomSpeed * event->wheel.y;
        if(fieldOfView < 5.0f) fieldOfView = 5.0f;
        if(fieldOfView > 130.0f) fieldOfView = 130.0f;
        m_fFieldOfView = fieldOfView;
    }


    //OffsetOrientation( MouseSensitivity * mouse_x, MouseSensitivity * mouse_y);
}

void Camera::OffsetOrientation(float up_angle, float right_angle)
{
    m_fHorizontalAngle += right_angle;
    m_fVerticalAngle += up_angle;
    NormalizeAngles();
}

void Camera::NormalizeAngles()
{
    m_fHorizontalAngle = fmodf(m_fHorizontalAngle, 360.0f);
    //fmodf can return negative values, but this will make them all positive
    if(m_fHorizontalAngle < 0.0f)
        m_fHorizontalAngle += 360.0f;

    if(m_fVerticalAngle > MaxVerticalAngle)
        m_fVerticalAngle = MaxVerticalAngle;
    else if(m_fVerticalAngle < -MaxVerticalAngle)
        m_fVerticalAngle = -MaxVerticalAngle;
}

void Camera::Update( float delta_time )
{
    // Limit Mouse Scroll
    if( m_iMouseScroll < -14 )
    {
        m_iMouseScroll = -14;
    }
    if( m_iMouseScroll > 3 )
    {
        m_iMouseScroll = 3;
    }
}

void Camera::OffsetPosition(const glm::vec3& offset)
{
    m_v3Position += offset;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::perspective(m_fFieldOfView, m_fViewportAspectRatio, m_fNearPlane, m_fFarPlane);
}


glm::mat4 Camera::Orientation() const
{
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, m_fVerticalAngle, glm::vec3(1.0f, 0.0f,0.0f) );
    orientation = glm::rotate(orientation, m_fHorizontalAngle, glm::vec3(0.0f, 1.0f, 0.0f) );
    return orientation;
}

glm::mat4 Camera::GetViewMatrix() const
{
    return Orientation() * glm::translate(glm::mat4(), -m_v3Position);
}

glm::vec3 Camera::Forward() const
{
    glm::vec4 forward = glm::inverse(Orientation()) * glm::vec4( 0.0f, 0.0f, -1.0f, 1.0f );
    return glm::vec3(forward);
}

glm::vec3 Camera::Right() const
{
    glm::vec4 right = glm::inverse(Orientation()) * glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
    return glm::vec3(right);
}

glm::vec3 Camera::Up() const
{
    glm::vec4 up = glm::inverse(Orientation()) * glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
    return glm::vec3(up);
}

glm::mat4 Camera::GetMatrix() const
{
    return GetProjectionMatrix() * GetViewMatrix();
}


void Camera::CleanUp()
{
}

int Camera::GetX()
{
    return m_v3Position.b;
}

int Camera::GetY()
{
    return m_v3Position.p;
}