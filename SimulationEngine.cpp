#include "SimulationEngine.h"

bool SimulationEngine::Run(const char* window_title, int window_height, int window_width, bool fullscreen)
{
    if( !Init(window_title, window_height, window_width, fullscreen) )
    {
        Cleanup();
        return false;
    }
    float delta_time;
    double current_time;
    double previous_time = SDL_GetTicks();
    while( Running() )
    {
        HandleInput();
        current_time = SDL_GetTicks();
        delta_time = (float)((current_time - previous_time)/1000);
        Update(delta_time);
        previous_time = current_time;
        Draw();
        previous_time = current_time;
    }

    return true;
}

bool SimulationEngine::Running()
{
    return m_bRunning;
}


bool SimulationEngine::Init(const char* window_title, int window_height, int window_width, bool fullscreen)
{
    if( !InitSDL(window_title, window_height, window_width, fullscreen) )
    {
        return false;
    }
    if( !InitOpenGL(window_width, window_height) )
    {
        return false;
    }

    m_iWindowWidth = window_width;
    m_iWindowHeight = window_height;
    // Keep the mouse in the window always
    m_cCamera.Init();

    // the sun
    Sun* sun = new Sun;
    sun->InitRotation(true, 25, 432450);
    m_vPlanetList.push_back( sun );

    /*
    //(bool rotate, bool orbit, int rotation_speed, int orbit_speed, int orbit_distance, int radius);
    BaseSphere* Earth;
    Earth->Init( true, true, 1, 365, 92960000, 3959, Sun )
    m_vPlanetList.push_back( Earth );

    BaseSphere* Uranus;
    Uranus->Init( true, true, (17.14/24), (84*365), 1787000000, 15759, Sun )
    m_vPlanetList.push_back( Uranus );

    BaseSphere* Saturn;
    Saturn->Init( true, true, (10.39/24), (29*365), 890700000, 36184, Sun )
    m_vPlanetList.push_back( Saturn );

    BaseSphere* Neptune;
    Neptune->Init( true, true, (16.06/24), (365*164.79), 2798000000, 15299, Sun )
    m_vPlanetList.push_back( Neptune );

    BaseSphere* Jupiter;
    Jupiter->Init( true, true, (9.56/24), (265*12), 483800000, 43441, Sun )
    m_vPlanetList.push_back( Jupiter );

    BaseSphere* Venus;
    Venus->Init( true, true, 116.18, 225, 67240000, 3760, Sun )
    m_vPlanetList.push_back( Venus );

    BaseSphere* Mars;
    Mars->Init( true, true, 1.004, 687, 141600000, 2106, Sun )
    m_vPlanetList.push_back( Mars );

    BaseSphere* Mercury;
    Mercury->Init( true, true, (58 +(15.3/24)), 88, 35980000, 1516, Sun )
    m_vPlanetList.push_back( Mercury );
    */

    m_bRunning = true;
    return true;
}

void SimulationEngine::Quit()
{
    SDL_GL_DeleteContext(m_cContext);
    SDL_DestroyWindow(m_cWindow);
    m_bRunning = false;
    SDL_Quit();
}
void SimulationEngine::HandleInput()
{
    SDL_Event event;
    // Keep the mouse in the window always
    //SDL_WarpMouseInWindow(m_cWindow, m_iWindowHeight/2, m_iWindowWidth/2);
    m_cCamera.HandleInput(&event);
    if (SDL_PollEvent(&event))
    {
        // Forces me to only use this for button when buttons get hit, this will only trigger when buttons are hit
        //m_cCamera.HandleInput(&event);
        if (event.type == SDL_KEYDOWN )
        {
            if( event.key.keysym.sym == SDLK_ESCAPE )
            {
                Quit();
            }
            if( event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w )
            {
                // Move world Up
            }
            if( event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s )
            {
                // Move World Down
            }
            if( event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d )
            {
                 // Move World Right
            }
            if( event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a )
            {
                // Move World Left
            }
        }
        
        if (event.type == SDL_QUIT) 
        {
            Quit();
        }
    }
}

void SimulationEngine::Update(float delta_time)
{
    m_cCamera.Update(delta_time);
    for(BaseSphere* planet : m_vPlanetList)
    {
        planet->Update(delta_time, &m_cCamera);
    }
}

void SimulationEngine::Draw()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    for(BaseSphere* planet : m_vPlanetList)
    {
        planet->Draw();
    }
    SDL_GL_SwapWindow(m_cWindow);
}

void SimulationEngine::Cleanup()
{

}

bool SimulationEngine::InitOpenGL( int window_width, int window_height )
{
    // Needed for core profile
    glewExperimental = GL_TRUE;
    GLenum status = glewInit();
    if (GLEW_OK != status)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(status));
        return false;
    }

    // Set our viewport at 0, 0 with a height and width equal to the window
    glViewport(0, 0, window_width, window_height);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    //clear background screen to black when we clear it
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    // Clear The Screen And The Depth Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return OpenGLStatus();
}



bool SimulationEngine::InitSDL( const char * window_title, int window_width, int window_height, bool fullscreen )
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS) != 0)
    {
        std::cout <<  "SDL_Init: " << SDL_GetError() << std::endl;
        return false;
    }
    else
    {
        std::cout <<  "SDL_Init: was successful" << std::endl;
    }

    int window_flags = SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL;
    if (fullscreen)
    {
        window_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    m_cWindow = SDL_CreateWindow(window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, window_flags);

    if( m_cWindow == nullptr )
    {
        std::cout << "SDL_CreateWindow " << SDL_GetError() << std::endl;
        return false;
    }
    else
    {
        std::cout << "SDL_CreateWindow was successful" << std::endl;
    }

    m_cContext = SDL_GL_CreateContext(m_cWindow);

    if (m_cContext == NULL)
    {
        std::cout << "SDL_GL_CreateContext " << SDL_GetError() << std::endl;
        return false;
    }
    else
    {
        std::cout << "Render Successfully Created!" << std::endl;
    }

    if( SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) == -1 )
    {
        std::cout <<  "SDL_GL_SetAttribute Major: " << SDL_GetError() << std::endl;
        return false;
    }
    if( SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3) == -1 )
    {
        std::cout <<  "SDL_GL_SetAttribute Major: " << SDL_GetError() << std::endl;
        return false;
    }
    if( SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8) == -1 )
    {
        std::cout <<  "SDL_GL_SetAttribute SDL_GL_STENCILSIZE: " << SDL_GetError() << std::endl;
        return false;
    }
    if( SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) == -1 )
    {
        std::cout <<  "SDL_GL_SetAttribute SDL_GL_ACCELERATED_VISUAL: " << SDL_GetError() << std::endl;
        return false;
    }
    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG) == -1)
    {
        std::cout <<  "SDL_GL_SetAttribute SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG: " << SDL_GetError() << std::endl;
        return false;
    }
    if( SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) == -1 )
    {
        std::cout <<  "SDL_GL_SetAttribute SDL_GL_CONTEXT_PROFILE_CORE: " << SDL_GetError() << std::endl;
        return false;
    }

    int img_flags = IMG_INIT_JPG|IMG_INIT_PNG;
    if ((IMG_Init(img_flags) != img_flags))
    {
        std::cout << "IMG_Init " << SDL_GetError() << std::endl;
        return false;
    }
    else
    {
        std::cout << "IMG_Init was successful!" << std::endl;
    }

    return true;
}

bool SimulationEngine::OpenGLStatus()
{
    printf("        GL Status: ");

    GLenum status = (GLenum)glGetError();
    switch(status)
    {
    case GL_NO_ERROR:
        printf("OK\n");
        return true;
    case GL_INVALID_ENUM:
        printf("GL_INVALID_ENUM\n");
        break;
    case GL_INVALID_VALUE:
        printf("GL_INVALID_VALUE\n");
        break;
    case GL_INVALID_OPERATION:
        printf("GL_INVALID_OPERATION\n");
        break;
    case GL_STACK_OVERFLOW:
        printf("GL_STACK_OVERFLOW\n");
        break;
    case GL_STACK_UNDERFLOW:
        printf("GL_STACK_UNDERFLOW\n");
        break;
    case GL_OUT_OF_MEMORY:
        printf("GL_OUT_OF_MEMORY\n");
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
        printf("GL_INVALID_FRAMEBUFFER_OPERATION_EXT\n");
        break;
    default:
        printf("UNKNWON\n");
    }
    return false;
}
