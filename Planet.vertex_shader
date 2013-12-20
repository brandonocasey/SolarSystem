#version 330 core

// input vertex data different for all executions of shader
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;


// Output Data that will be interpolated for each fragment, (goes to fragment shader)
out vec2 UV;

// Uniform Values Stay Constant for the whole mesh this call
uniform mat4 MVP;

void main()
{
    // Model, View, and Perspective matrices multiplied
    // Output position of the vertex, in clip space : MVP * position
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);

    // axes of a 2D texture are U and V, thus UVmap
    UV = vertexUV;

}