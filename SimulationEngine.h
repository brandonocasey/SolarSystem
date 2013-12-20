#pragma once
#include "Utils.h"
#include "BaseSphere.h"
#include "Camera.h"

class SimulationEngine
{
public:
    bool Run(const char* window_title, int window_width, int window_height, bool fullscreen);
    bool Running();
    bool Init(const char* window_title, int window_width, int window_height, bool fullscreen);
    void HandleInput(float delta_time);
    void Update(float delta_time);
    void Draw();
    float SimulationEngine::GetTime();
    void Cleanup();
    void Quit();
    bool InitOpenGL(int window_width, int window_height);
    bool InitSDL(const char * window_title, int window_width, int window_height, bool fullscreen);
    bool OpenGLStatus();
    bool OpenGLContextInfo();
    bool InitSDLGLAttributes();
private:
    bool m_bRunning;
    Camera m_cCamera;
    SDL_Window* m_cWindow;
    SDL_GLContext m_cContext;
    SDL_Renderer* m_cRenderer;
    std::vector<BaseSphere*> m_vPlanetList;
    GLuint m_iTexture[1];
    int m_iWindowHeight;
    int m_iWindowWidth;
};
