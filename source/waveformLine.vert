#version 460

layout(push_constant, std430) uniform Window
{
    vec2 window;
};
layout(set = 0, binding = 1) buffer InputSamples
{
    float inputSamples[];
};

layout(location = 0) out vec4 outputColor;

void main()
{
    uint vertexID = gl_VertexIndex;
    float sampleHeight = inputSamples[vertexID];
    float x = float(vertexID) / window.x;
    x *= 2.0;
    x -= 1.0;

    gl_Position = vec4(x, sampleHeight, 1.0, 1.0);

    outputColor = vec4(1.0, 0.0, 0.0, 0.0);
}

