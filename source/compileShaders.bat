
glslangValidator -V -gVS quadPass.vert -o quadPass.vert.spirv
glslangValidator -V -gVS quadPass.frag -o quadPass.frag.spirv
glslangValidator -V -gVS quadDepthPass.vert -o quadDepthPass.vert.spirv

glslangValidator -V -gVS textureQuadPass.vert -o textureQuadPass.vert.spirv
glslangValidator -V -gVS textureQuadPass.frag -o textureQuadPass.frag.spirv
glslangValidator -V -gVS textureQuadDepthPass.vert -o textureQuadDepthPass.vert.spirv

glslangValidator -V -gVS compositePass.vert -o compositePass.vert.spirv
glslangValidator -V -gVS compositePass.frag -o compositePass.frag.spirv



PAUSE