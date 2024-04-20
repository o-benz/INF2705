#include "scenes.h"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"

#include "utils.h"

Scene::Scene(Resources& resources)
: m_res(resources)
{

}

Scene::~Scene()
{

}

void Scene::drawSky(glm::mat4& mvp)
{
    // DRAW SKYBOX
    glDepthFunc(GL_LEQUAL);
    m_res.skyboxShader.use();
    m_res.skyboxTexture.use();
    glUniformMatrix4fv(m_res.mvpLocationSky, 1, GL_FALSE, &mvp[0][0]);
    m_res.skybox.draw(GL_TRIANGLES, 6 * 6);
    glDepthFunc(GL_LESS);
}

TesselationScene::TesselationScene(Resources& resources)
: Scene(resources)
, m_viewWireframe(0)
{
    // TODO
    glPatchParameteri(GL_PATCH_VERTICES, 4);
}

TesselationScene::~TesselationScene()
{

}

void TesselationScene::render(glm::mat4& view, glm::mat4& projPersp)
{
    drawMenu();

    glm::mat4 mvp;
    glm::mat4 projView = projPersp * view;
    glm::mat4 modelView;

    m_res.tessellation.use();
    m_res.heightmapTexture.use(0);
    m_res.grassTexture.use(1);
    m_res.sandTexture.use(2);
    m_res.snowTexture.use(3);

    mvp = projView;
    glUniformMatrix4fv(m_res.mvpLocationTessellation, 1, GL_FALSE, &mvp[0][0]);
    modelView = view;
    glUniformMatrix4fv(m_res.modelViewLocationTessellation, 1, GL_FALSE, &modelView[0][0]);

    glUniform1i(m_res.viewWireframeLocationTessellation, m_viewWireframe);

    // TODO: To remove, only for debug
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//GL_FILL

    // TODO
    m_res.tesselationPlane.draw(GL_PATCHES, m_res.tesselationPlaneCount);

    mvp = projPersp * glm::mat4(glm::mat3(view));
    drawSky(mvp);
}

void TesselationScene::drawMenu()
{
    ImGui::Begin("Tessellation Parameters");

    ImGui::Checkbox("View wireframe?", (bool*)&m_viewWireframe);

    ImGui::End();
}


static const unsigned int MAX_N_PARTICULES = 10000;
static Particle particles[MAX_N_PARTICULES] = { {{0,0,0},{0,0,0},{0,0,0,0}, {0,0},0} };

ParticleScene::ParticleScene(Resources& resources, Window& w)
: Scene(resources)
, m_w(w)
, m_oldTime(m_w.getTick() / 1000.0f)
, m_cumulativeTime(0.0f)
, m_nParticles(1)
, m_nMaxParticles(1000)
{
    glEnable(GL_PROGRAM_POINT_SIZE);
    
    // TODO
    // create VAO, VBOs and TFO
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(2, m_vbo);
    glGenTransformFeedbacks(1, &m_tfo);

    // Pass particles data to input VBO
    auto numBytes = MAX_N_PARTICULES * sizeof(Particle);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, numBytes, particles, GL_DYNAMIC_COPY);

    // Configure output VBO with the same size (no data needed since it's filled with results)
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, numBytes, nullptr, GL_DYNAMIC_COPY);

    // link TFO to VAO
    glBindVertexArray(m_vao);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_tfo);

    // Load particule VBO
    m_res.particule.use();

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(10 * sizeof(GLfloat)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(12 * sizeof(GLfloat)));
    
    // Load Modified Value VBO
    m_res.transformFeedback.use();

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_STREAM_READ);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(10 * sizeof(GLfloat)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(12 * sizeof(GLfloat)));

    // Remove all links
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

ParticleScene::~ParticleScene()
{
    // TODO
    glUseProgram(0);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(2, m_vbo);
    glDeleteBuffers(1, &m_tfo);
}

void ParticleScene::render(glm::mat4& view, glm::mat4& projPersp)
{
    glm::mat4 mvp;
    glm::mat4 projView = projPersp * view;
    glm::mat4 modelView;

    float time = m_w.getTick() / 1000.0;
    float dt = time - m_oldTime;
    m_oldTime = time;
    m_cumulativeTime += dt;
    if (dt > 1.0f)
        m_nParticles = 1;

    m_res.transformFeedback.use();
    
    // TODO: buffer binding
    glUniform1f(m_res.timeLocationTransformFeedback, time);
    glUniform1f(m_res.dtLocationTransformFeedback, dt);

    glBindVertexArray(m_vao);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[1]);

    // TODO: update particles
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    // Redo attributs configuration since binding a different VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(10 * sizeof(GLfloat)));
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(12 * sizeof(GLfloat)));
    glBeginTransformFeedback(GL_POINTS);
    glEnable(GL_RASTERIZER_DISCARD);
    glDrawArrays(GL_POINTS, 0, m_nParticles);
    glDisable(GL_RASTERIZER_DISCARD);
    glEndTransformFeedback();

    // TODO: swap buffers
    std::swap(m_vbo[0], m_vbo[1]);

    // Draw skybox first without the function to change some parameter on the depth test.
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    m_res.skyboxShader.use();
    m_res.skyboxTexture.use();
    mvp = projPersp * glm::mat4(glm::mat3(view));
    glUniformMatrix4fv(m_res.mvpLocationSky, 1, GL_FALSE, &mvp[0][0]);
    m_res.skybox.draw(GL_TRIANGLES, 6 * 6);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);


    m_res.particule.use();
    m_res.flameTexture.use();

    // TODO: buffer binding
    modelView = view;
    glUniformMatrix4fv(m_res.modelViewLocationParticle, 1, GL_FALSE, &modelView[0][0]);
    glUniformMatrix4fv(m_res.projectionLocationParticle, 1, GL_FALSE, &projPersp[0][0]);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(10 * sizeof(GLfloat)));
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 13 * sizeof(GLfloat), (GLvoid*)(12 * sizeof(GLfloat)));

    // TODO: Draw particles without depth write and with blending
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_POINTS, 0, m_nParticles);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);


    if (m_cumulativeTime > 1.0f / 60.0f)
    {
        m_cumulativeTime = 0.0f;
        if (++m_nParticles > m_nMaxParticles)
            m_nParticles = m_nMaxParticles;
    }    
}
