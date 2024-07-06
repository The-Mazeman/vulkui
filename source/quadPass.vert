#version 460

struct Entity
{
    uint windowTag;
    uint colorIndex;
    float depth;
    uint pad;
};
layout(set = 0, binding = 0) buffer WindowClipPlanes
{
    vec4 windowClipPlanes[];
}; 
layout(set = 0, binding = 1) buffer Entites
{
    Entity entities[];
}; 
layout(set = 0, binding = 2) buffer Colors
{
    vec4 colors[];
}; 
layout(set = 0, binding = 3, r32ui) uniform writeonly uimage2D indexSurface;

layout(set = 0, binding = 4) buffer EntityIndices
{
    uint entityIndices[];
}; 
layout(set = 0, binding = 5) buffer Vertices
{
    vec2 vertices[];
}; 

layout(location = 0) out vec4 colorOutput;
layout(location = 1) out uint windowTagOutput;

out gl_PerVertex
{
    float gl_ClipDistance[4];
    vec4 gl_Position;
};
void main()
{
    uint instanceIndex = gl_InstanceIndex;
    uint vertexIndex = gl_VertexIndex;
    vertexIndex += (instanceIndex * 4);
    vec2 vertex = vertices[vertexIndex];

    uint entityIndex = entityIndices[instanceIndex];
    Entity entity = entities[entityIndex];

    float depth = entity.depth;
    gl_Position = vec4(vertex, depth, 1.0);

    uint windowTag = entity.windowTag;
    windowTagOutput = windowTag;
    
    vec4 clipPlane = windowClipPlanes[windowTag];

    vec4 left = vec4(1.0, 0.0, 0.0, clipPlane.x);
    vec4 top = vec4(0.0, 1.0, 0.0, clipPlane.y); 
    vec4 right = vec4(-1.0, 0.0, 0.0, clipPlane.z); 
    vec4 bottom = vec4(0.0, -1.0, 0.0, clipPlane.w); 

    gl_ClipDistance[0] = dot(left, vec4(vertex, 0.0f, 1.0f));
    gl_ClipDistance[1] = dot(top, vec4(vertex, 0.0f, 1.0f));
    gl_ClipDistance[2] = dot(right, vec4(vertex, 0.0f, 1.0f));
    gl_ClipDistance[3] = dot(bottom, vec4(vertex, 0.0f, 1.0f));

    uint colorIndex = entity.colorIndex;
    colorOutput = colors[colorIndex];
}
