#version 460

layout(location = 0) in vec2 position;

void main()
{
    gl_Position = vec4(position, 1.0, 1.0);
}


