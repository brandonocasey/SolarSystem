#include "SimulationEngine.h"

bool SimulationEngine::Run(const char* window_title, int window_height, int window_width, bool fullscreen)
{
    if( !Init(window_title, window_height, window_width, fullscreen) )
    {
        Cleanup();
        return false;
    }
    float delta_time;
    float current_time = GetTime();
    float previous_time = GetTime();
    while( Running() )
    {
        previous_time = current_time;
        current_time = GetTime();
        delta_time = current_time - previous_time;

            if( delta_time > 1 )
            {
                printf("delta time = %f", delta_time);
            }
        HandleInput(delta_time);
        Update(delta_time);
        Draw();
    }

    return true;
}

float SimulationEngine::GetTime()
{
    return (float)(SDL_GetTicks()/1000.0f);
}

bool SimulationEngine::Running()
{
    return m_bRunning;
}


bool SimulationEngine::Init(const char* window_title, int window_width, int window_height, bool fullscreen)
{
    if( !InitSDL(window_title, window_width, window_height, fullscreen) )
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

    //              days,            miles,           days,               miles
    // ( float rotation_speed, float radius, float orbit_speed, int distance_from_sun, std::string texture_path )
    // the sun
    m_vPlanetList.push_back( new BaseSphere );
    m_vPlanetList.back()->Init(25, 432450, 0, 0, "sunmap.jpg", nullptr);

    // Mercury
    m_vPlanetList.push_back( new BaseSphere );
    m_vPlanetList.back()->Init(59, 15000, 88, 1,  "mercurymap.jpg", m_vPlanetList[0]);


    // Venus
    m_vPlanetList.push_back( new BaseSphere );
    m_vPlanetList.back()->Init(117, 37600, 225, 2,  "venusmap.jpg", m_vPlanetList[0]);


    // Earth
    m_vPlanetList.push_back( new BaseSphere );
    m_vPlanetList.back()->Init(1, 39590, 965, 3,  "earthmap1k.jpg", m_vPlanetList[0]);



    // Mars
    m_vPlanetList.push_back( new BaseSphere );
    m_vPlanetList.back()->Init(1, 21060, 687, 4,  "mars_1k_color.jpg", m_vPlanetList[0]);


    // Jupiter
    m_vPlanetList.push_back( new BaseSphere );
    m_vPlanetList.back()->Init( 0.4f, 434410, (12*365), 5,  "jupitermap.jpg", m_vPlanetList[0]);


    // Saturn
    m_vPlanetList.push_back( new BaseSphere );
    m_vPlanetList.back()->Init(0.4f, 361840, (29 * 365), 6,  "saturnmap.jpg", m_vPlanetList[0]);



    // Uranus
    m_vPlanetList.push_back( new BaseSphere );
    m_vPlanetList.back()->Init(0.7f, 157590, (84 * 365), 7,  "uranusmap.jpg", m_vPlanetList[0]);


    // Neptune
    m_vPlanetList.push_back( new BaseSphere );
    m_vPlanetList.back()->Init(0.6f, 152990, (60190), 8,  "neptunemap.jpg", m_vPlanetList[0]);

    m_bRunning = true;
    return true;
}

void SimulationEngine::Quit()
{
    for(BaseSphere* planet : m_vPlanetList)
    {
        planet->Cleanup();
    }

    SDL_GL_DeleteContext(m_cContext);
    SDL_DestroyWindow(m_cWindow);
    m_bRunning = false;

    IMG_Quit();
    SDL_Quit();
}
void SimulationEngine::HandleInput(float delta_time)
{
    SDL_Event event;
    // Keep the mouse in the window always
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
        }
        
        if (event.type == SDL_QUIT) 
        {
            Quit();
        }
    }
    m_cCamera.HandleInput(&event, delta_time);
    SDL_WarpMouseInWindow(m_cWindow, m_iWindowHeight/2, m_iWindowWidth/2);
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

    glViewport(0, 0, window_width, window_height);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);



    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    //clear background screen to dark blue when we clear it
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Clear The Screen And The Depth Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    OpenGLContextInfo();

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

    // Dont Show Mouse
    SDL_ShowCursor(0);
    int window_flags = SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL;
    if (fullscreen)
    {
        window_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    if( !InitSDLGLAttributes() )
    {
        return false;
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

    m_cContext = SDL_GL_CreateContext(m_cWindow);

    if (m_cContext == NULL)
    {
        std::cout << "SDL_GL_CreateContext " << SDL_GetError() << std::endl;
        OpenGLStatus();
        return false;
    }
    else
    {
        std::cout << "SDL_GL_CreateContext Successfully Created!" << std::endl;
    }

    return true;
}

bool SimulationEngine::InitSDLGLAttributes()
{
    bool return_var = true;

    std::cout <<  "OpenGLStatus After SDL GL SetAttribute Calls: " << std::endl;

    return return_var;
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

// Print Stuff about the context we created
bool SimulationEngine::OpenGLContextInfo()
{
    printf("OpenGL version Info: %s\n", glGetString(GL_VERSION) );
    printf("OpenGL Vendor Info: %s\n", glGetString(GL_VENDOR) );
    printf("OpenGL Renderer Info: %s\n", glGetString(GL_RENDERER) );
    int open_gl_extension_count;
    glGetIntegerv(GL_NUM_EXTENSIONS, &open_gl_extension_count);
    for(int i = 0; i < open_gl_extension_count; i++)
    {
        printf("Extension %i info: %s\n", i, glGetStringi(GL_EXTENSIONS, i));
    }

    printf("OpenGL Shader Info: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION) );

    return true;
}