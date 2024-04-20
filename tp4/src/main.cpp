#include <stdio.h>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "window.h"
#include "camera.h"
#include "scenes.h"

#include "utils.h"

void printGLInfo();

int main(int argc, char* argv[])
{
    Window w;
    if (!w.init())
        return -1;

    GLenum rev = glewInit();
    if (rev != GLEW_OK)
    {
        std::cout << "Could not initialize glew! GLEW_Error: " << glewGetErrorString(rev) << std::endl;
        return -2;
    }
    printGLInfo();

    // RESOURCES

    Resources res;

    // CONTEXT PARAMS
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);  
    glFrontFace(GL_CCW);

    // CAMERA AND PLAYER PROPERTIES

    glm::vec3 playerPosition = glm::vec3(0);
    glm::vec2 playerOrientation = glm::vec2(0);
    Camera c(playerPosition, playerOrientation);
    
    bool isFirstPersonCam = false;
    bool isMouseMotionEnabled = true;
    int scrollLevel = 8;

    // SCENES

    TesselationScene tesselationScene(res);
    ParticleScene particuleScene(res, w);

    enum class AvailableScene
    {
        TESSELATION, PARTICLE
    } sceneChoice = AvailableScene::TESSELATION;
    
    // MAIN LOOP

    bool isRunning = true;
    while (isRunning)
    {
        GL_CHECK_ERROR;
        if (w.shouldResize())
            glViewport(0, 0, w.getWidth(), w.getHeight());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // PROCESS INPUT

        if (w.getKeyPress(Window::Key::T))
        {
            playerPosition = glm::vec3(0);
            playerOrientation = glm::vec2(0);

            switch (sceneChoice)
            {
            case AvailableScene::TESSELATION: sceneChoice = AvailableScene::PARTICLE;    break;
            case AvailableScene::PARTICLE:    sceneChoice = AvailableScene::TESSELATION; break;
            }
        }            

        if (w.getKeyPress(Window::Key::SPACE))
            isMouseMotionEnabled = !isMouseMotionEnabled;

        const int SCROLL_MIN = 1, SCROLL_MAX = 120;
        scrollLevel -= w.getMouseScrollDirection();
        scrollLevel = scrollLevel <= SCROLL_MAX ? scrollLevel >= SCROLL_MIN ? scrollLevel : SCROLL_MIN : SCROLL_MAX; // clamp between SCROLL_MIN and SCROLL_MAX
        isFirstPersonCam = scrollLevel == SCROLL_MIN;
        if (sceneChoice == AvailableScene::TESSELATION)
            isFirstPersonCam = true;

        if (isMouseMotionEnabled) w.hideMouse();
        else w.showMouse();

        int x = 0, y = 0;
        if (isMouseMotionEnabled)
            w.getMouseMotion(x, y);
        playerOrientation.y -= x * 0.01f;
        playerOrientation.x -= y * 0.01f;

        glm::vec3 positionOffset = glm::vec3(0.0);
        const float SPEED = 0.1f;
        if (w.getKeyHold(Window::Key::W))
            positionOffset.z -= SPEED;
        if (w.getKeyHold(Window::Key::S))
            positionOffset.z += SPEED;
        if (w.getKeyHold(Window::Key::A))
            positionOffset.x -= SPEED;
        if (w.getKeyHold(Window::Key::D))
            positionOffset.x += SPEED;
        
        if (w.getKeyHold(Window::Key::Q) && sceneChoice == AvailableScene::TESSELATION)
            positionOffset.y -= SPEED;
        if (w.getKeyHold(Window::Key::E) && sceneChoice == AvailableScene::TESSELATION)
            positionOffset.y += SPEED;

        positionOffset = glm::rotate(glm::mat4(1.0f), playerOrientation.y, glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(positionOffset, 1);
        
        if (sceneChoice != AvailableScene::PARTICLE)
            playerPosition += positionOffset;

        glm::mat4 projPersp = glm::perspective(glm::radians(70.0f), (float)w.getWidth() / (float)w.getHeight(), 0.1f, 200.0f);
        glm::mat4 view = isFirstPersonCam ? c.getFirstPersonViewMatrix() : c.getThirdPersonViewMatrix(scrollLevel);

        switch (sceneChoice)
        {
        case AvailableScene::TESSELATION: tesselationScene.render(view, projPersp); break;
        case AvailableScene::PARTICLE:    particuleScene.render(view, projPersp);   break;
        }
        
        w.swap();
        w.pollEvent();
        isRunning = !w.shouldClose() && !w.getKeyPress(Window::Key::ESC);
    }

    return 0;
}

void printGLInfo()
{
    std::cout << "OpenGL info:" << std::endl;
    std::cout << "    Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "    Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "    Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "    Shading version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}
