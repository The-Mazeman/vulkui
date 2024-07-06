#version 460

layout(input_attachment_index = 0, binding = 0) uniform subpassInput inputColor;

layout(location = 0) out vec4 outputColor;

void main()
{
    outputColor = subpassLoad(inputColor);
}


