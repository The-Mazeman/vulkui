#version 460

layout(early_fragment_tests) in;
layout(set = 0, binding = 3, r32ui) uniform writeonly uimage2D indexSurface;
layout(set = 0, binding = 4) uniform texture2D fontImage;
layout(set = 0, binding = 5) uniform sampler textureSampler;

layout(location = 0) in vec4 color;
layout(location = 1) in vec2 textureVertex;
layout(location = 2) in flat uint windowTag;

layout(location = 0) out vec4 outputColor;

void main()
{
    vec4 screenPosition = gl_FragCoord;
    uint x = uint(screenPosition.x);
    uint y = uint(screenPosition.y);

    ivec2 pixel = ivec2(x, y);
    imageStore(indexSurface, pixel, uvec4(windowTag));

    vec4 texel = texture(sampler2D(fontImage, textureSampler), textureVertex);
    texel.rgb *= color.rgb;
    outputColor = texel;
}
