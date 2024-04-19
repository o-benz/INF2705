#include "scenes.h"

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


StencilTestScene::StencilTestScene(Resources& resources, bool& isFirstPersonCam, glm::vec3& position, glm::vec2& orientation)
: Scene(resources)
, m_isFirstPersonCam(isFirstPersonCam)
, m_position(position)
, m_orientation(orientation)
{
    const GLfloat GROUND_SIZE = 30.0f/2.0f;
    // CALCULATE MATRIX TRANSFORMS FOR GROUPS AND OBJECTS
    for (int i = 0; i < N_ALLY_MONKEE; ++i)
    {
        float x = -GROUND_SIZE + 2 * GROUND_SIZE * rand01();
        float y = -GROUND_SIZE + 2 * GROUND_SIZE * rand01();
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(x, -1, y));
        model = glm::rotate(model, float(M_PI * 2 * rand01()), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(0.5));
        allyTransform[i] = model;
    }

    for (int i = 0; i < N_ENEMY_MONKEE; ++i)
    {
        float x = -GROUND_SIZE + 2 * GROUND_SIZE * rand01();
        float y = -GROUND_SIZE + 2 * GROUND_SIZE * rand01();
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(x, -1, y));
        model = glm::rotate(model, float(M_PI * 2 * rand01()), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(0.5));
        enemyTransform[i] = model;
    }
}

StencilTestScene::~StencilTestScene()
{

}

void StencilTestScene::render(glm::mat4& view, glm::mat4& projPersp)
{
    glm::mat4 mvp;
    glm::mat4 projView = projPersp * view;

    // DRAW CHARACTER

    m_res.model.use();
    m_res.suzanneTexture.use();

    if (!m_isFirstPersonCam)
    {
        glm::mat4 playerMat = glm::mat4(1);
        playerMat = glm::translate(playerMat, glm::vec3(0, -1.0f, 0));
        playerMat = glm::translate(playerMat, m_position);
        playerMat = glm::rotate(playerMat, m_orientation.y + float(M_PI), glm::vec3(0, 1, 0));
        playerMat = glm::scale(playerMat, glm::vec3(0.5));
        mvp = projView * playerMat;
        glUniformMatrix4fv(m_res.mvpLocationModel, 1, GL_FALSE, &mvp[0][0]);
        m_res.suzanne.draw();
    }

    m_res.rockTexture.use();

    glm::mat4 rockMat = glm::mat4(1);
    rockMat = glm::translate(rockMat, glm::vec3(-10, -0.2f, 0));
    mvp = projView * rockMat;
    glUniformMatrix4fv(m_res.mvpLocationModel, 1, GL_FALSE, &mvp[0][0]);
    m_res.rock.draw();


    // DRAW GROUND
    mvp = projView * glm::mat4(1.0);
    glUniformMatrix4fv(m_res.mvpLocationModel, 1, GL_FALSE, &mvp[0][0]);
    m_res.groundTexture.use();
    m_res.ground.draw(GL_TRIANGLES, 6);


    // Précalcul des matrices mvp des singes,
	// utilisable pour les modèles et halos.
    glm::mat4 allyMvp[N_ALLY_MONKEE];
    for (int i = 0; i < N_ALLY_MONKEE; i++) {
        allyMvp[i] = projView * allyTransform[i];
    }

    glm::mat4 enemyMvp[N_ENEMY_MONKEE];
    for (int i = 0; i < N_ENEMY_MONKEE; i++) {
        enemyMvp[i] = projView * enemyTransform[i];
    }
	
	m_res.suzanneTexture.use();
    // Remplir le stencil en dessinant les singes
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 0xFF, 0xFF);
    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
    for (size_t i = 0; i < N_ALLY_MONKEE; ++i) {
        glStencilMask(1 << i);
        glUniformMatrix4fv(m_res.mvpLocationModel, 1, GL_FALSE, &allyMvp[i][0][0]);
        m_res.suzanne.draw();
    }

    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    for (size_t i = 0; i < N_ENEMY_MONKEE; ++i) {
        glStencilMask(1 << (i + N_ALLY_MONKEE));
        glUniformMatrix4fv(m_res.mvpLocationModel, 1, GL_FALSE, &enemyMvp[i][0][0]);
        m_res.suzanne.draw();
    }
    glDisable(GL_STENCIL_TEST);
	
	
	// On dessine le ciel un peu plus tôt
    mvp = projPersp * glm::mat4(glm::mat3(view));
    drawSky(mvp);

    m_res.model.use();
    m_res.glassTexture.use();
    
	// Dessin du mur vitrée
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    m_res.model.use();
    m_res.glassTexture.use();

    mvp = projView * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    glDisable(GL_CULL_FACE);
    glUniformMatrix4fv(m_res.mvpLocationModel, 1, GL_FALSE, &mvp[0][0]);
    m_res.glass.draw();
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    // Dessiner les halos  
    m_res.simple.use();
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilMask(0xFF);

    glUniform3f(m_res.colorLocationSimple, 1.0f, 0.0f, 0.0f);
    for (size_t i = 0; i < N_ENEMY_MONKEE; ++i) {
        glUniformMatrix4fv(m_res.mvpLocationSimple, 1, GL_FALSE, &enemyMvp[i][0][0]);
        glStencilFunc(GL_EQUAL, 0, 1 << (i + N_ALLY_MONKEE));
        m_res.suzanne.draw();
    }

    glDepthFunc(GL_ALWAYS);
    glUniform3f(m_res.colorLocationSimple, 0.0f, 1.0f, 1.0f);
    for (size_t i = 0; i < N_ALLY_MONKEE; i++) {
        glUniformMatrix4fv(m_res.mvpLocationSimple, 1, GL_FALSE, &allyMvp[i][0][0]);
        glStencilFunc(GL_EQUAL, 0, 1 << i);
        m_res.suzanne.draw();

    }

    glDepthFunc(GL_LESS);
    
    glDisable(GL_STENCIL_TEST);

}


LightingTestScene::LightingTestScene(Resources& resources)
: Scene(resources)
, m_lightingData(nullptr, sizeof(m_lightModel) + sizeof(m_material) + sizeof(m_lights))
, m_currentModel(0)
, m_currentShading(2)
{
    m_lightModel =
    {
        glm::vec3(0.2f),
        false,
        false,
        false,
        1.0f,
        20.0f
    };

    m_material =
    {
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.8f, 0.8f, 0.8f, 0.0f},
        {0.8f, 0.8f, 0.8f, 0.0f},
        {1.0f, 1.0f, 1.0f},
        100.0f
    };

    m_lights[0] =
    {
        {0.1f, 0.0f, 0.0f, 0.0f},
        {1.0f, 0.2f, 0.2f, 0.0f},
        {0.5f, 0.1f, 0.1f, 0.0f},
        {1.5f, 2.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f}
    };    
    m_lights[1] =
    {
        {0.0f, 0.1f, 0.0f, 0.0f},
        {0.2f, 1.0f, 0.2f, 0.0f},
        {0.1f, 0.5f, 0.1f, 0.0f},
        {-1.5f, 1.0f, 1.5f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f}
    };
    m_lights[2] =
    {
        {0.0f, 0.0f, 0.1f, 0.0f},
        {0.2f, 0.2f, 1.0f, 0.0f},
        {0.1f, 0.1f, 0.5f, 0.0f},
        {0.0f, 1.0f, -2.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f}
    };

    orientation[0] = glm::vec2(45.0f, 55.0f);
    orientation[1] = glm::vec2(45.0f, -45.0f);
    orientation[2] = glm::vec2(45.0f, 180.0f);

    m_res.phong.setUniformBlockBinding("LightingBlock", 0);
    m_lightingData.setBindingIndex(0);
}

LightingTestScene::~LightingTestScene()
{

}

void LightingTestScene::render(glm::mat4& view, glm::mat4& projPersp)
{
    drawMenu();

    GLintptr offset = 0;
    m_lightingData.updateData(&m_material  , offset, sizeof(m_material));   offset += sizeof(m_material);
    m_lightingData.updateData(m_lights     , offset, sizeof(m_lights));     offset += sizeof(m_lights);
    m_lightingData.updateData(&m_lightModel, offset, sizeof(m_lightModel)); offset += sizeof(m_lightModel);

    glm::mat4 mvp;
    glm::mat4 projView = projPersp * view;
    glm::mat4 modelView;

    GLint mvpMatrixLocation = -1;
    GLint viewMatrixLocation = -1;
    GLint modelViewMatrixLocation = -1;
    GLint normalMatrixLocation = -1;

    switch (m_currentShading)
    {
    case 0: 
        m_res.flat.use();
        viewMatrixLocation = m_res.viewLocationFlat;
        mvpMatrixLocation = m_res.mvpLocationFlat;
        modelViewMatrixLocation = m_res.modelViewLocationFlat;
        normalMatrixLocation = m_res.normalLocationFlat;
        break;
    case 1:
        m_res.gouraud.use();
        viewMatrixLocation = m_res.viewLocationGouraud;
        mvpMatrixLocation = m_res.mvpLocationGouraud;
        modelViewMatrixLocation = m_res.modelViewLocationGouraud;
        normalMatrixLocation = m_res.normalLocationGouraud;
        break;
    case 2: 
        m_res.phong.use();
        viewMatrixLocation = m_res.viewLocationPhong;
        mvpMatrixLocation = m_res.mvpLocationPhong;
        modelViewMatrixLocation =  m_res.modelViewLocationPhong;
        normalMatrixLocation = m_res.normalLocationPhong;
        break;
    }
    m_res.diffuseMapTexture.use(0);
    m_res.specularMapTexture.use(1);

    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &view[0][0]);
    glm::mat4 sphereModel = glm::mat4(1.0f);
    mvp = projView * sphereModel;
    modelView = view * sphereModel;
    glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, &mvp[0][0]);
    glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, &modelView[0][0]);
    glUniformMatrix3fv(normalMatrixLocation, 1, GL_TRUE, glm::value_ptr(glm::inverse(glm::mat3(modelView))));

    switch (m_currentModel)
    {
    case 0: m_res.sphere.draw(); break;
    case 1: m_res.cube.draw(); break;
    case 2: m_res.suzanne.draw(); break;
    }

    m_res.whiteTexture.use(0);
    m_res.whiteTexture.use(1);
    for (size_t i = 0; i < 3; ++i)
    {
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, glm::vec3(m_lights[i].position));
        lightModel = glm::rotate(lightModel, glm::radians(orientation[i].y), glm::vec3(0.0f, 1.0f, 0.0f));
        lightModel = glm::rotate(lightModel, glm::radians(orientation[i].x), glm::vec3(1.0f, 0.0f, 0.0f));
        m_lights[i].spotDirection = lightModel * glm::vec4(0, -1, 0, 0);

        mvp = projView * lightModel;
        modelView = view * lightModel;
        glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, &modelView[0][0]);
        glUniformMatrix3fv(normalMatrixLocation, 1, GL_TRUE, glm::value_ptr(glm::inverse(glm::mat3(modelView))));

        Material lightMaterial =
        {
            m_lights[i].diffuse,
            glm::vec4(0.0f),
            glm::vec4(0.0f),
            glm::vec3(0.0f),
            1.0f
        };
        m_lightingData.updateData(&lightMaterial, 0, sizeof(lightMaterial));
        m_res.spotlight.draw();
    }

    mvp = projPersp * glm::mat4(glm::mat3(view));
    drawSky(mvp);
}

void LightingTestScene::drawMenu()
{
    const char* modelList[] = { "Sphere", "Cube", "Monkey" };
    const char* shadingList[] = { "Flat", "Gouraud", "Phong" };

    ImGui::Begin("Scene Parameters");

    ImGui::Combo("Model", &m_currentModel, modelList, sizeof(modelList) / sizeof(modelList[0]));
    ImGui::SeparatorText("Material");
    ImGui::ColorEdit3("Emission##m", &m_material.emission[0]);
    ImGui::ColorEdit3("Ambient##m", &m_material.ambient[0]);
    ImGui::ColorEdit3("Diffuse##m", &m_material.diffuse[0]);
    ImGui::ColorEdit3("Specular##m", &m_material.specular[0]);
    ImGui::DragFloat("Shininess##m", &m_material.shininess, 1.0f, 0.0f, 1000.0f);

    ImGui::SeparatorText("Light Model");
    ImGui::Combo("Shading", &m_currentShading, shadingList, sizeof(shadingList) / sizeof(shadingList[0]));
    ImGui::ColorEdit3("Global ambient", &m_lightModel.lightModelAmbient[0]);
    ImGui::Checkbox("Use blinn?", (bool*)&m_lightModel.useBlinn);
    ImGui::Checkbox("Use spotlight?", (bool*)&m_lightModel.useSpotlight); ImGui::SameLine();
    ImGui::Checkbox("Use Direct3D?", (bool*)&m_lightModel.useDirect3D);
    ImGui::DragFloat("Spot Exponent", &m_lightModel.spotExponent, 0.5f, 0.0f, 500.0f);
    ImGui::DragFloat("Spot Opening", &m_lightModel.spotOpeningAngle, 0.5f, 0.0f, 360.0f);
    if (ImGui::TreeNode("Light 1"))
    {
        ImGui::ColorEdit3("Ambient##l1", &m_lights[0].ambient[0]);
        ImGui::ColorEdit3("Diffuse##l1", &m_lights[0].diffuse[0]);
        ImGui::ColorEdit3("Specular##l1", &m_lights[0].specular[0]);
        ImGui::DragFloat3("Position##l1", &m_lights[0].position[0], 0.1f, 0.0f, 0.0f);
        ImGui::DragFloat2("Orientation##l1", &orientation[0][0], 0.1f, -360.0f, 360.0f);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Light 2"))
    {
        ImGui::ColorEdit3("Ambient##l2", &m_lights[1].ambient[0]);
        ImGui::ColorEdit3("Diffuse##l2", &m_lights[1].diffuse[0]);
        ImGui::ColorEdit3("Specular##l2", &m_lights[1].specular[0]);
        ImGui::DragFloat3("Position##l2", &m_lights[1].position[0], 0.1f, 0.0f, 0.0f);
        ImGui::DragFloat2("Orientation##l2", &orientation[1][0], 0.1f, -360.0f, 360.0f);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Light 3"))
    {
        ImGui::ColorEdit3("Ambient##l3", &m_lights[2].ambient[0]);
        ImGui::ColorEdit3("Diffuse##l3", &m_lights[2].diffuse[0]);
        ImGui::ColorEdit3("Specular##l3", &m_lights[2].specular[0]);
        ImGui::DragFloat3("Position##l3", &m_lights[2].position[0], 0.1f, 0.0f, 0.0f);
        ImGui::DragFloat2("Orientation##l3", &orientation[2][0], 0.1f, -360.0f, 360.0f);
        ImGui::TreePop();
    }

    ImGui::End();
}
