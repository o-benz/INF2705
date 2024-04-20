#include "resources.h"

#include <iostream>

#include "vertices_data.h"
#include "utils.h"

const char* pathes[] = {
    "../textures/skybox/Daylight Box_Right.bmp",
    "../textures/skybox/Daylight Box_Left.bmp",
    "../textures/skybox/Daylight Box_Top.bmp",
    "../textures/skybox/Daylight Box_Bottom.bmp",
    "../textures/skybox/Daylight Box_Front.bmp",
    "../textures/skybox/Daylight Box_Back.bmp",
};

Resources::Resources()
: tesselationPlaneCount(0)
, skybox(skyboxVertices, sizeof(skyboxVertices))
, heightmapTexture("../textures/heightmap.png", GL_CLAMP_TO_EDGE)
, grassTexture("../textures/grass.jpg", GL_REPEAT)
, sandTexture("../textures/sand.jpg", GL_REPEAT)
, snowTexture("../textures/snow.jpg", GL_REPEAT)
, flameTexture("../textures/flame.png", GL_CLAMP_TO_EDGE)
, skyboxTexture(pathes)
{
    // Simple meshes
    skybox.enableAttribute(0, 3, 0, 0);

    const int N_DIVISIONS = 9;
    const int N_PRIMITIVES = N_DIVISIONS + 1;
    const int N_COMPONENTS = 3;
    const int N_PER_PRIMITIVE = 4;
    const int DATA_SIZE = N_PRIMITIVES * N_PRIMITIVES * N_PER_PRIMITIVE * N_COMPONENTS;
    const GLfloat SIZE = 256.0f;
    const GLfloat FACTOR = SIZE / N_PRIMITIVES;
    const GLfloat OFFSET = -SIZE / 2;
    GLfloat planeData[DATA_SIZE];
    tesselationPlaneCount = N_PRIMITIVES * N_PRIMITIVES * N_PER_PRIMITIVE;

    for (int i = 0; i < DATA_SIZE;)
    {
        int primitiveIndex = i / (N_PER_PRIMITIVE * N_COMPONENTS);
        int x = primitiveIndex % (N_PRIMITIVES);
        int y = primitiveIndex / (N_PRIMITIVES);

        int X = x + 1;
        int Y = y + 1;

        planeData[i++] = x * FACTOR + OFFSET;
        planeData[i++] = -1.0f;
        planeData[i++] = y * FACTOR + OFFSET;

        planeData[i++] = x * FACTOR + OFFSET;
        planeData[i++] = -1.0f;
        planeData[i++] = Y * FACTOR + OFFSET;

        planeData[i++] = X * FACTOR + OFFSET;
        planeData[i++] = -1.0f;
        planeData[i++] = Y * FACTOR + OFFSET;

        planeData[i++] = X * FACTOR + OFFSET;
        planeData[i++] = -1.0f;
        planeData[i++] = y * FACTOR + OFFSET;
    }

    tesselationPlane.setData(planeData, sizeof(planeData));
    tesselationPlane.enableAttribute(0, 3, 0, 0);

    // Textures
    grassTexture.enableMipmap();
    sandTexture.enableMipmap();
    snowTexture.enableMipmap();

    // Skybox shader
    {
        std::string vertexCode = readFile("shaders/skybox.vs.glsl");
        std::string fragmentCode = readFile("shaders/skybox.fs.glsl");
        Shader vertex(GL_VERTEX_SHADER, vertexCode.c_str());
        Shader fragment(GL_FRAGMENT_SHADER, fragmentCode.c_str());
        skyboxShader.attachShader(vertex);
        skyboxShader.attachShader(fragment);
        skyboxShader.link();

        mvpLocationSky = skyboxShader.getUniformLoc("mvp");
    }

    // Tessellation shader
    {
        std::string vertexCode = readFile("shaders/tessellation.vs.glsl");
        std::string tessCtrlCode = readFile("shaders/tessellation.tcs.glsl");
        std::string tessEvalCode = readFile("shaders/tessellation.tes.glsl");
        std::string geometryCode = readFile("shaders/tessellation.gs.glsl");
        std::string fragmentCode = readFile("shaders/tessellation.fs.glsl");

        Shader vertex(GL_VERTEX_SHADER, vertexCode.c_str());
        Shader tessControl(GL_TESS_CONTROL_SHADER, tessCtrlCode.c_str());
        Shader tessEvaluation(GL_TESS_EVALUATION_SHADER, tessEvalCode.c_str());
        Shader geometry(GL_GEOMETRY_SHADER, geometryCode.c_str());
        Shader fragment(GL_FRAGMENT_SHADER, fragmentCode.c_str());
        tessellation.attachShader(vertex);
        tessellation.attachShader(tessControl);
        tessellation.attachShader(tessEvaluation);
        tessellation.attachShader(geometry);
        tessellation.attachShader(fragment);
        tessellation.link();

        mvpLocationTessellation = tessellation.getUniformLoc("mvp");
        modelViewLocationTessellation = tessellation.getUniformLoc("modelView");
        viewWireframeLocationTessellation = tessellation.getUniformLoc("viewWireframe");

        tessellation.use();
        glUniform1i(tessellation.getUniformLoc("heighmapSampler"), 0);
        glUniform1i(tessellation.getUniformLoc("groundSampler"), 1);
        glUniform1i(tessellation.getUniformLoc("sandSampler"), 2);
        glUniform1i(tessellation.getUniformLoc("snowSampler"), 3);
    }

    // Particule shader
    {
        std::string vertexCode = readFile("shaders/particle.vs.glsl");
        std::string geometryCode = readFile("shaders/particle.gs.glsl");
        std::string fragmentCode = readFile("shaders/particle.fs.glsl");

        Shader vertex(GL_VERTEX_SHADER, vertexCode.c_str());
        Shader geometry(GL_GEOMETRY_SHADER, geometryCode.c_str());
        Shader fragment(GL_FRAGMENT_SHADER, fragmentCode.c_str());
        particule.attachShader(vertex);
        particule.attachShader(geometry);
        particule.attachShader(fragment);
        particule.link();

        modelViewLocationParticle = particule.getUniformLoc("modelView");
        projectionLocationParticle = particule.getUniformLoc("projection");
    }
    
    // Transform feedback shader
    {
        std::string vertexCode = readFile("shaders/transformFeedback.vs.glsl");

        Shader vertex(GL_VERTEX_SHADER, vertexCode.c_str());
        transformFeedback.attachShader(vertex);

		// TODO

        const GLchar* vars[] ={
			"positionMod",
			"velocityMod",
			"colorMod",
			"sizeMod",
			"timeToLiveMod"
		};
		transformFeedback.setTransformFeedbackVaryings(vars, sizeof(vars) / sizeof(vars[0]), GL_INTERLEAVED_ATTRIBS);

        transformFeedback.link();

        timeLocationTransformFeedback = transformFeedback.getUniformLoc("time");
        dtLocationTransformFeedback = transformFeedback.getUniformLoc("dt");
    }
}
