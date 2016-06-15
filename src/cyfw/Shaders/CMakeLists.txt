
set(SOURCE_FILES
        Basic2DShader.cpp
        Basic3DShader.cpp
        CyrilShader.cpp
        )

embed_resources(cyfw_SHADERS
        shaders/2d-vert.glsl
        shaders/2d-frag.glsl
        shaders/3d-vert.glsl
        shaders/3d-frag.glsl
        shaders/cyril-vert.glsl
        shaders/cyril-frag.glsl
        )

add_library(cyfw_shaders STATIC ${SOURCE_FILES} ${cyfw_SHADERS})
