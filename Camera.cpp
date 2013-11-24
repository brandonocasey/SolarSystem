#include "Camera.h"

void Camera::Init()
{
    // Start the camera at the origin
    m_v3Position = glm::vec3( 0, 0, -5 );

    // Toward -Z
    m_fHorizontalAngle = 3.14f;

    // 0 looks at the horizon
    m_fVerticalAngle = 0.0f;

    // FoV is the level of zoom. 80° = very wide angle, huge deformations. 60° – 45° : standard. 20° : big zoom.
    m_fInitialFieldOfView = 45.0f;

    // 3 Units per second
    m_fSpeed = 100.0f;

    float m_fMouseSpeed = 0.005f;


    m_iMouseX = 0;
    m_iMouseY = 0;
    m_iMouseScroll = 0;
    m_bUp = false;
    m_bRight = false;
    m_bDown = false;
    m_bLeft = false;
}

void Camera::HandleInput(SDL_Event* event)
{
    //SDL_GetMouseState(&m_iMouseX, &m_iMouseY);
    //printf("Normal: x = %i and y = %i\n", m_iMouseX, m_iMouseY);
    SDL_GetRelativeMouseState(&m_iMouseX, &m_iMouseY);
    //printf("Relative: x = %i and y = %i\n", mouse_x, mouse_y);
    if(event->type == SDL_KEYDOWN )
    {
        if( event->key.keysym.sym == SDLK_UP || event->key.keysym.sym == SDLK_w )
        {
            m_bUp = true;
        }
        else
        {
            m_bUp = false;
        }
        if( event->key.keysym.sym == SDLK_RIGHT || event->key.keysym.sym == SDLK_d)
        {
            m_bRight = true;
        }
        else
        {
            m_bRight = false;
        }

        if( event->key.keysym.sym == SDLK_DOWN || event->key.keysym.sym == SDLK_s)
        {
            m_bDown = true;
        }
        else
        {
            m_bDown = false;
        }
        if( event->key.keysym.sym == SDLK_LEFT || event->key.keysym.sym == SDLK_a )
        {
            m_bLeft = true;
        }
        else
        {
            m_bLeft = false;
        }
    }
    else
    {
        m_bUp = false;
        m_bRight = false;
        m_bDown = false;
        m_bLeft = false;
    }

    if(event->type == SDL_MOUSEWHEEL )
    {
        // we only want the up and down scroll which is y
        m_iMouseScroll += event->wheel.y;
        printf("scroll = %d\n", m_iMouseScroll);
    }
}

void Camera::Update( float delta_time )
{
    m_fHorizontalAngle = m_fMouseSpeed * delta_time * float( m_iMouseX);
    m_fVerticalAngle = m_fMouseSpeed * delta_time * float( m_iMouseY);

    glm::vec3 direction = glm::vec3(
        cos(m_fVerticalAngle) * sin(m_fHorizontalAngle),
        sin(m_fVerticalAngle),
        cos(m_fVerticalAngle) * cos(m_fHorizontalAngle)
        );

    // Right vector
    glm::vec3 right = glm::vec3(
        sin(m_fHorizontalAngle - 3.14f/2.0f),
        0,
        cos(m_fHorizontalAngle - 3.14f/2.0f)
        );

    glm::vec3 up = glm::cross( right, direction );

    if( m_bUp )
    {
        printf("UP x=%f, y=%f, z=%f\n", m_v3Position.x, m_v3Position.y, m_v3Position.z);
        m_v3Position += direction * delta_time * m_fSpeed;
    }
    if( m_bDown )
    {
        printf("DOWN x=%f, y=%f, z=%f\n", m_v3Position.x, m_v3Position.y, m_v3Position.z);
        m_v3Position -= direction * delta_time * m_fSpeed;
    }
    if( m_bRight )
    {
        printf("RIGHT x=%f, y=%f, z=%f\n", m_v3Position.x, m_v3Position.y, m_v3Position.z);
        m_v3Position += right * delta_time * m_fSpeed;
    }
    if( m_bLeft )
    {
        printf("LEFT x=%f, y=%f, z=%f\n", m_v3Position.x, m_v3Position.y, m_v3Position.z);
        m_v3Position -= right * delta_time * m_fSpeed;
    }

    // Limit Mouse Scroll
    if( m_iMouseScroll < -14 )
    {
        m_iMouseScroll = -14;
    }
    if( m_iMouseScroll > 3 )
    {
        m_iMouseScroll = 3;
    }

    float FoV = m_fInitialFieldOfView - 5 * m_iMouseScroll;

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    m_m4ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    m_m4ViewMatrix       = glm::lookAt(
        m_v3Position,           // Camera is here
        m_v3Position+direction, // and looks here : at the same position, plus "direction"
        up                  // Head is up (set to 0,-1,0 to look upside-down)
        );
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return m_m4ProjectionMatrix;
}

glm::mat4 Camera::GetViewMatrix()
{
    return m_m4ViewMatrix;
}

void Camera::CleanUp()
{
}
