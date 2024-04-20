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


class TesselationScene : public Scene
{
public:
    TesselationScene(Resources& resources);
    virtual ~TesselationScene();

    virtual void render(glm::mat4& view, glm::mat4& projPersp);

    void drawMenu();

private:
    // IMGUI VARIABLE
    bool m_viewWireframe;
};


struct Particle
{
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    glm::vec2 size;
    GLfloat timeToLive;
};


class ParticleScene : public Scene
{
public:
    ParticleScene(Resources& resources, Window& w);
    virtual ~ParticleScene();

    virtual void render(glm::mat4& view, glm::mat4& projPersp);

private:
    Window& m_w;

    float m_oldTime;
    float m_cumulativeTime;

    GLuint m_tfo, m_vao, m_vbo[2];
    unsigned int m_nParticles;
    unsigned int m_nMaxParticles;
};


#endif // SCENES_H
