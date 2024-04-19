#include "resources.h"

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
: ground(groundVertices, sizeof(groundVertices), groundIndexes, sizeof(groundIndexes))
, skybox(skyboxVertices, sizeof(skyboxVertices))
, rock("../models/rock.obj")
, suzanne("../models/suzanne.obj")
, glass("../models/glass.obj")
, sphere("../models/icosphere.obj")
, cube("../models/cube.obj")
, spotlight("../models/spotlight.obj")
, rockTexture("../models/rockTexture.png", GL_CLAMP_TO_EDGE)
, suzanneTexture("../models/suzanneTexture.png", GL_CLAMP_TO_EDGE)
, glassTexture("../models/glass.png", GL_CLAMP_TO_EDGE)
, groundTexture("../textures/groundSeamless.jpg", GL_REPEAT)
, whiteTexture("../textures/white.png", GL_CLAMP_TO_EDGE)
, diffuseMapTexture("../textures/metal_0029_color_1k.jpg", GL_CLAMP_TO_EDGE)
, specularMapTexture("../textures/metal_0029_metallic_1k.jpg", GL_CLAMP_TO_EDGE)
, skyboxTexture(pathes)
{
    // Simple meshes
    ground.enableAttribute(0, 3, 5, 0);
    ground.enableAttribute(1, 2, 5, 3);

    skybox.enableAttribute(0, 3, 0, 0);

    // Textures
    groundTexture.enableMipmap();

    // Model shader
    {
        std::string vertexCode = readFile("shaders/model.vs.glsl");
        std::string fragmentCode = readFile("shaders/model.fs.glsl");

        Shader vertex(GL_VERTEX_SHADER, vertexCode.c_str());
        Shader fragment(GL_FRAGMENT_SHADER, fragmentCode.c_str());
        model.attachShader(vertex);
        model.attachShader(fragment);
        model.link();
        mvpLocationModel = model.getUniformLoc("mvp");
    }

    // Phong shader
    {
        std::string vertexCode = readFile("shaders/phong.vs.glsl");
        std::string fragmentCode = readFile("shaders/phong.fs.glsl");

        Shader vertex(GL_VERTEX_SHADER, vertexCode.c_str());
        Shader fragment(GL_FRAGMENT_SHADER, fragmentCode.c_str());
        phong.attachShader(vertex);
        phong.attachShader(fragment);
        phong.link();
        mvpLocationPhong = phong.getUniformLoc("mvp");
        modelViewLocationPhong = phong.getUniformLoc("modelView");
        viewLocationPhong = phong.getUniformLoc("view");
        normalLocationPhong = phong.getUniformLoc("normalMatrix");
        phong.use();
        glUniform1i(phong.getUniformLoc("diffuseSampler"), 0);
        glUniform1i(phong.getUniformLoc("specularSampler"), 1);
    }

    // Gouraud shader
    {
        std::string vertexCode = readFile("shaders/gouraud.vs.glsl");
        std::string fragmentCode = readFile("shaders/gouraud.fs.glsl");

        Shader vertex(GL_VERTEX_SHADER, vertexCode.c_str());
        Shader fragment(GL_FRAGMENT_SHADER, fragmentCode.c_str());
        gouraud.attachShader(vertex);
        gouraud.attachShader(fragment);
        gouraud.link();
        mvpLocationGouraud = gouraud.getUniformLoc("mvp");
        modelViewLocationGouraud = gouraud.getUniformLoc("modelView");
        viewLocationGouraud = gouraud.getUniformLoc("view");
        normalLocationGouraud = gouraud.getUniformLoc("normalMatrix");
        gouraud.use();
        glUniform1i(gouraud.getUniformLoc("diffuseSampler"), 0);
        glUniform1i(gouraud.getUniformLoc("specularSampler"), 1);
    }

    // Flat shader
    {
        std::string vertexCode = readFile("shaders/flat.vs.glsl");
        std::string geometryCode = readFile("shaders/flat.gs.glsl");
        std::string fragmentCode = readFile("shaders/gouraud.fs.glsl");

        Shader vertex(GL_VERTEX_SHADER, vertexCode.c_str());
        Shader geometry(GL_GEOMETRY_SHADER, geometryCode.c_str());
        Shader fragment(GL_FRAGMENT_SHADER, fragmentCode.c_str());
        flat.attachShader(vertex);
        flat.attachShader(geometry);
        flat.attachShader(fragment);
        flat.link();
        mvpLocationFlat = flat.getUniformLoc("mvp");
        modelViewLocationFlat = flat.getUniformLoc("modelView");
        viewLocationFlat = flat.getUniformLoc("view");
        normalLocationFlat = flat.getUniformLoc("normalMatrix");
        flat.use();
        glUniform1i(flat.getUniformLoc("diffuseSampler"), 0);
        glUniform1i(flat.getUniformLoc("specularSampler"), 1);
    }

    // Simple shader
    {
        std::string vertexCode = readFile("shaders/simple.vs.glsl");
        std::string fragmentCode = readFile("shaders/simple.fs.glsl");
        Shader vertex(GL_VERTEX_SHADER, vertexCode.c_str());
        Shader fragment(GL_FRAGMENT_SHADER, fragmentCode.c_str());
        simple.attachShader(vertex);
        simple.attachShader(fragment);
        simple.link();

        mvpLocationSimple = simple.getUniformLoc("mvp");
        colorLocationSimple = simple.getUniformLoc("color");
    }

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
}
