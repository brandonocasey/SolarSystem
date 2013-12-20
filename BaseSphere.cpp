#include "BaseSphere.h"


void BaseSphere::Init( float rotation_speed, float radius, float orbit_speed, float distance_from_sun, std::string texture_path, BaseSphere* object_to_rotate_around )
{
    m_oRotateAround = object_to_rotate_around;

    // custom math yay!
    m_fRotationSpeed = 360.0f/(rotation_speed+3);
    m_fOrbitSpeed = fmodf(orbit_speed, 360.0f);
    m_fRadius = radius/10000.0f;


    m_fDistanceFromSun = distance_from_sun + m_fRadius;
    // These will go away eventually since everything is centered
    m_fPositionX = 0.0f;
    m_fPositionY = 0.0f;
    m_fPositionZ = 0.0f;

    // All of our OpenGL index id's
    m_iShaderProgram = 0;
    m_iTexture = 0;
    m_iUVBuffer = 0;
    m_iVertexBuffer = 0;
    m_iVertexArrayID = 0;

    m_sTexturePath = texture_path;

    glGenVertexArrays(1, &m_iVertexArrayID);
    glBindVertexArray(m_iVertexArrayID);

    bool res = LoadOBJ("sphere.obj", m_v3Vertices, m_v2UV, m_v2Normals);

    // Load our shaders
    m_iShaderProgram = LoadShader("Planet.vertex_shader", "Planet.fragment_shader");

    glGenBuffers(1, &m_iVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_iVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_v3Vertices.size() * sizeof(glm::vec3),  &m_v3Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_iUVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_iUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_v2UV.size() * sizeof(glm::vec3), &m_v2UV[0], GL_STATIC_DRAW);
    m_iTexture = LoadTexture(m_sTexturePath.c_str());
}

void BaseSphere::Draw()
{
    glUseProgram(m_iShaderProgram);
    // Bind our texture in Texture Unit 0
    glBindTexture(GL_TEXTURE_2D, m_iTexture);

    // Set our "texPuppy" sampler to use Texture Unit 0
    glUniform1i(glGetUniformLocation(m_iShaderProgram, "texPlanet"), 0);


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

    glBindBuffer(GL_ARRAY_BUFFER, m_iVertexArrayID);
    glDrawArrays(GL_TRIANGLES, 0, m_v3Vertices.size());

    // Reset everything we already drew
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glUseProgram(0);
}

float BaseSphere::GetLocationX()
{
    return m_fPositionX;
}
float BaseSphere::GetLocationY()
{
    return m_fPositionY;
}
float BaseSphere::GetLocationZ()
{
    return m_fPositionZ;
}
float BaseSphere::GetRadius()
{
    return m_fRadius;
}


void BaseSphere::Update(float delta_time, Camera* camera)
{
    glUseProgram(m_iShaderProgram);
    GLint mvp_id = glGetUniformLocation(m_iShaderProgram, "MVP");

    glm::mat4 ModelMatrix;

    ModelMatrix = glm::scale(ModelMatrix, glm::vec3( m_fRadius, m_fRadius, m_fRadius ) );

    if( m_fOrbitSpeed > 0 && m_oRotateAround != nullptr )
    {
        float angle = (float)(SDL_GetTicks()/1000.0f) / 360.0f * m_fOrbitSpeed;
        m_fPositionX = m_oRotateAround->GetLocationX() + sin(angle) * (m_oRotateAround->GetRadius() + m_fDistanceFromSun);
        m_fPositionZ =  m_oRotateAround->GetLocationZ() + cos(angle) * (m_oRotateAround->GetRadius() + m_fDistanceFromSun);
    }


    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(m_fPositionX, m_fPositionY, m_fPositionZ));


    ModelMatrix = glm::rotate(ModelMatrix, (float)(SDL_GetTicks()/1000.0f) / (m_fRotationSpeed) * 360.0f, glm::vec3(0, 1, 0));
   
    glm::mat4 MVP = camera->GetMatrix() * ModelMatrix;
    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(MVP));
    glUseProgram(0);
}


void BaseSphere::Cleanup()
{
    glDeleteBuffers(1, &m_iVertexBuffer);
    glDeleteBuffers(1, &m_iUVBuffer);
    glDeleteProgram(m_iShaderProgram);
    glDeleteTextures(1, &m_iTexture);
    glDeleteVertexArrays(1, &m_iVertexArrayID);
}



bool BaseSphere::OpenGLStatus()
{
    printf("->GL Status: ");

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


// opengl-tutorial.org
// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc
bool BaseSphere::LoadOBJ( const char * path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals )
{
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while( 1 )
    {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 )
        {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}
        else if ( strcmp( lineHeader, "vt" ) == 0 )
        {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
        else if ( strcmp( lineHeader, "vn" ) == 0 )
        {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}
        else if ( strcmp( lineHeader, "f" ) == 0 )
        {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
        else
        {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ )
    {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		
		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	}

	return true;
}

GLuint BaseSphere::LoadTexture(const char * location)
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

std::string BaseSphere::ReadFile(const char *filePath)
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


GLuint BaseSphere::LoadShader(const char *vertex_path, const char *fragment_path)
{
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