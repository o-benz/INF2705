#ifndef RESOURCES_H
#define RESOURCES_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shader_program.h"
#include "shapes.h"
#include "model.h"
#include "textures.h"

class Resources
{
public:
    Resources();

    // #############
    // SHADERS
    // #############
    ShaderProgram skyboxShader;
    GLint mvpLocationSky;
    // ---
    ShaderProgram tessellation;
    GLint mvpLocationTessellation;
    GLint modelViewLocationTessellation;
    GLint viewWireframeLocationTessellation;
    // ---
    ShaderProgram transformFeedback;
    GLint timeLocationTransformFeedback;
    GLint dtLocationTransformFeedback;
    // ---
    ShaderProgram particule;
    GLint modelViewLocationParticle;
    GLint projectionLocationParticle;
    // #############
    // SIMPLE GEOMETRY MESHES
    // #############
    BasicShapeArrays tesselationPlane;
    GLsizei tesselationPlaneCount;
    BasicShapeArrays skybox;
    // #############
    // MODELS
    // #############
    // ...
    // #############
    // TEXTURES
    // #############
    Texture2D heightmapTexture;
    Texture2D grassTexture;
    Texture2D sandTexture;
    Texture2D snowTexture;
    Texture2D flameTexture;
    TextureCubeMap skyboxTexture;
};

#endif // RESOURCES_H
