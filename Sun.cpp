#include "Sun.h"


void Sun::InitRotation( bool rotation, int rotation_speed, double radius )
{
    m_iPositionx = 0;
    m_iPositiony = 0;
    m_iPositionz = 0;

    m_bRotation = rotation;
    m_iRotationSpeed = rotation_speed;
    m_dRadius = radius;


    // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat g_vertex_buffer_data[] = { 
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    // Two UV coordinates for each vertex. They were created withe Blender.
    static const GLfloat g_uv_buffer_data[] = { 
        0.000059f, 0.000004f, 
        0.000103f, 0.336048f, 
        0.335973f, 0.335903f, 
        1.000023f, 0.000013f, 
        0.667979f, 0.335851f, 
        0.999958f, 0.336064f, 
        0.667979f, 0.335851f, 
        0.336024f, 0.671877f, 
        0.667969f, 0.671889f, 
        1.000023f, 0.000013f, 
        0.668104f, 0.000013f, 
        0.667979f, 0.335851f, 
        0.000059f, 0.000004f, 
        0.335973f, 0.335903f, 
        0.336098f, 0.000071f, 
        0.667979f, 0.335851f, 
        0.335973f, 0.335903f, 
        0.336024f, 0.671877f, 
        1.000004f, 0.671847f, 
        0.999958f, 0.336064f, 
        0.667979f, 0.335851f, 
        0.668104f, 0.000013f, 
        0.335973f, 0.335903f, 
        0.667979f, 0.335851f, 
        0.335973f, 0.335903f, 
        0.668104f, 0.000013f, 
        0.336098f, 0.000071f, 
        0.000103f, 0.336048f, 
        0.000004f, 0.671870f, 
        0.336024f, 0.671877f, 
        0.000103f, 0.336048f, 
        0.336024f, 0.671877f, 
        0.335973f, 0.335903f, 
        0.667969f, 0.671889f, 
        1.000004f, 0.671847f, 
        0.667979f, 0.335851f
    };

    glGenVertexArrays(1, &m_iVertexArrayID);
    glBindVertexArray(m_iVertexArrayID);

    // Load our shaders
    m_iShaderProgram = LoadShader("Triangle.vshader", "Triangle.fshader");


    glGenBuffers(1, &m_iVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_iVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &m_iUVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_iUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
    m_iTexture = LoadTexture("sample2.png");
}

void Sun::Update(float delta_time, Camera* camera)
{
    GLint mvp_id = glGetUniformLocation(m_iShaderProgram, "MVP");

    glm::mat4 ProjectionMatrix = camera->GetProjectionMatrix();
    glm::mat4 ViewMatrix = camera->GetViewMatrix();

    //glm::mat4 ProjectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    //// Camera matrix
    //glm::mat4 ViewMatrix       = glm::lookAt(
    //    glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
    //    glm::vec3(0,0,0), // and looks at the origin
    //    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    //    );
    glm::mat4 ModelMatrix = glm::mat4(1.0);
   
    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(MVP));
}

void Sun::Draw()
{
    glUseProgram(m_iShaderProgram);
    GLint texture_id = glGetUniformLocation(m_iShaderProgram, "texPuppy");

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_iTexture);

    // Set our "texPuppy" sampler to user Texture Unit 0
    glUniform1i(texture_id, 0);

    // 1st attribute buffer : vertices's
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_iVertexBuffer);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_iUVBuffer);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        2,                                // size : U+V => 2
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
        );

    glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
Sun::Sun()
{

}

void Sun::Cleanup()
{
    glDeleteBuffers(1, &m_iVertexBuffer);
    glDeleteBuffers(1, &m_iUVBuffer);
    glDeleteProgram(m_iShaderProgram);
    glDeleteTextures(1, &m_iTexture);
    glDeleteVertexArrays(1, &m_iVertexArrayID);
}


GLuint Sun::LoadTexture(const char * location)
{
    int mode; // the mode of the texture that SDL_image loaded
    SDL_Surface* image = IMG_Load(location);

    // Get the OpenGL mode of the SDL Image, kind of hacky :/
    switch (image->format->BytesPerPixel)
    {
    case 4:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            mode = GL_BGRA;
        else
            mode = GL_RGBA;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            mode = GL_BGR;
        else
            mode = GL_RGB;
        break;
    }

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, mode, GL_UNSIGNED_BYTE, image->pixels);

    // OpenGL has now copied the data. Free our own version
    SDL_FreeSurface(image);

    // Poor filtering, or ...
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

    // ... nice trilinear filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
    glGenerateMipmap(GL_TEXTURE_2D);

    OpenGLStatus();
    // Return the ID of the texture we just created
    return textureID;
}

std::string Sun::ReadFile(const char *filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open())
    {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}


GLuint Sun::LoadShader(const char *vertex_path, const char *fragment_path) {
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read shaders
    std::string vertShaderStr = ReadFile(vertex_path);
    std::string fragShaderStr = ReadFile(fragment_path);
    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;

    // Compile vertex shader
    //std::cout << "Compiling vertex shader." << std::endl;
    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);

    // Check vertex shader
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
    if( logLength > 0 )
    {
        char * vertex_shader_log = new char [logLength];
        glGetShaderInfoLog(vertShader, logLength, NULL, vertex_shader_log);
        if( vertex_shader_log )
        {
            std::cout << vertex_shader_log << std::endl;
        }
        SAFE_DELETE_ARRAY(vertex_shader_log);
    }

    // Compile fragment shader
    //std::cout << "Compiling fragment shader." << std::endl;
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);

    // Check fragment shader
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
    if( logLength > 0 )
    {
        char * frag_shader_log = new char [logLength];
        glGetShaderInfoLog(fragShader, logLength, NULL, frag_shader_log);
        if( frag_shader_log )
        {
            std::cout << frag_shader_log << std::endl;
        }
        SAFE_DELETE_ARRAY(frag_shader_log);
    }

    //std::cout << "Linking program" << std::endl;
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> programError( (logLength > 1) ? logLength : 1 );
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    std::cout << &programError[0] << std::endl;

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);


    return program;
}