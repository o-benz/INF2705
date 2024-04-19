#ifndef SCENES_H
#define SCENES_H

#include <GL/glew.h>

#include "window.h"
#include "resources.h"
#include "uniform_buffer.h"

class Scene
{
public:
    Scene(Resources& resources);
    virtual ~Scene();
    
    virtual void render(glm::mat4& view, glm::mat4& projPersp) = 0;

    void drawSky(glm::mat4& mvp);

protected:
    Resources& m_res;
};


class StencilTestScene : public Scene
{
public:
    StencilTestScene(Resources& resources, bool& isFirstPersonCam, glm::vec3& position, glm::vec2& orientation);
    virtual ~StencilTestScene();

    virtual void render(glm::mat4& view, glm::mat4& projPersp);

private:
    bool& m_isFirstPersonCam;
    glm::vec3& m_position;
    glm::vec2& m_orientation;

    // MATRICES
    static const size_t N_ALLY_MONKEE = 1;
    static const size_t N_ENEMY_MONKEE = 2;
    glm::mat4 allyTransform[N_ALLY_MONKEE];
    glm::mat4 enemyTransform[N_ENEMY_MONKEE];
};




struct Material
{
    glm::vec4 emission; // vec3, but padded
    glm::vec4 ambient;  // vec3, but padded
    glm::vec4 diffuse;  // vec3, but padded
    glm::vec3 specular;
    GLfloat shininess;
};

struct UniversalLight
{
    glm::vec4 ambient;  // vec3, but padded
    glm::vec4 diffuse;  // vec3, but padded
    glm::vec4 specular; // vec3, but padded
    glm::vec4 position; // vec3, but padded
    glm::vec4 spotDirection; // vec3, but padded
};

struct LightModel
{
    glm::vec3 lightModelAmbient;
    GLuint useBlinn;
    GLuint useSpotlight;
    GLuint useDirect3D;
    GLfloat spotExponent;
    GLfloat spotOpeningAngle;
};

class LightingTestScene : public Scene
{
public:
    LightingTestScene(Resources& resources);
    virtual ~LightingTestScene();

    virtual void render(glm::mat4& view, glm::mat4& projPersp);

    void drawMenu();

private:
    LightModel m_lightModel;
    Material m_material;
    UniversalLight m_lights[3];
    UniformBuffer m_lightingData;

    glm::vec2 orientation[3];

    // IMGUI VARIABLE
    int m_currentModel;
    int m_currentShading;
};

#endif // SCENES_H
