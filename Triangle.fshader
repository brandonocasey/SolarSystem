#version 330 core

// interpolated values from the vertex shaders
in vec2 UV;

// Output data
out vec3 color;

// values that stay constant
uniform sampler2D texPuppy;

void main()
{
    // Output color = color of the texture at this UV location
    color = texture2D( texPuppy, UV).rgb;
}