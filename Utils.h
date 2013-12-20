#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
// Requires OpenGL32.lib to be linked against

// Needs and include/lib, but no dynamic library
#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CLOCKS_PER_SEC 4000

#define UPDATES_PER_SECOND 60;
// For complex math to generate the circle, these used to be defined
#define M_PI_2 1.57079632679489661923

// Safely delete pointers, arrays of points, and release memory and such from the heap (dynamic memory)
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=nullptr; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=nullptr; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=nullptr; } }
#endif