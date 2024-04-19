
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "window.h"
#include "shader_program.h"
#include "vertices_data.h"
#include "shapes.h"
#include "camera.h"
#include "model.h"
#include "utils.h"
#include "textures.h"


const int GROUPS_ROWS_SIZE = 7;
const int GROUPS_TABLE_SIZE = GROUPS_ROWS_SIZE * GROUPS_ROWS_SIZE;

void printGLInfo();

#define GL_CHECK_ERROR checkGLError(__FILE__, __LINE__)
void checkGLError(const char* file, int line);

std::string readFile(const char* path);

int main(int argc, char* argv[])
{
    Window w;
    if (!w.init())
        return -1;

    GLenum rev = glewInit();
    if ( rev != GLEW_OK )
    {
        std::cout << "Could not initialize glew! GLEW_Error: " << glewGetErrorString(rev) << std::endl;
        return -2;
    }

    printGLInfo();

    // Instantiate shaders
    ShaderProgram modelShader;
    GLint modelMvpLoc;
    {
        std::string modelVertexCode   = readFile("./shaders/model.vs.glsl");
        std::string modelFragmentCode = readFile("./shaders/model.fs.glsl");

        Shader vertexShader(GL_VERTEX_SHADER, modelVertexCode.c_str());
        Shader fragmentShader(GL_FRAGMENT_SHADER, modelFragmentCode.c_str());
        modelShader.attachShader(vertexShader);
        modelShader.attachShader(fragmentShader);
        modelShader.link();
        modelMvpLoc = modelShader.getUniformLoc("mvp");
    }

    ShaderProgram riverShader;
    GLint riverMvpLoc;
    GLint riverTimeLoc;
    {
        std::string riverVertexCode = readFile("./shaders/water.vs.glsl");
        std::string riverFragmentCode = readFile("./shaders/water.fs.glsl");

        Shader vertexShader(GL_VERTEX_SHADER, riverVertexCode.c_str());
        Shader fragmentShader(GL_FRAGMENT_SHADER, riverFragmentCode.c_str());
        riverShader.attachShader(vertexShader);
        riverShader.attachShader(fragmentShader);
        riverShader.link();
        riverMvpLoc = riverShader.getUniformLoc("mvp");
        riverTimeLoc = riverShader.getUniformLoc("dtime");
    }

    ShaderProgram skyboxShader;
    GLint skyboxMvpLoc;
    {
        std::string skyboxVertexCode = readFile("./shaders/skybox.vs.glsl");
        std::string skyboxFragmentCode = readFile("./shaders/skybox.fs.glsl");

        Shader vertexShader(GL_VERTEX_SHADER, skyboxVertexCode.c_str());
        Shader fragmentShader(GL_FRAGMENT_SHADER, skyboxFragmentCode.c_str());
        skyboxShader.attachShader(vertexShader);
        skyboxShader.attachShader(fragmentShader);
        skyboxShader.link();
        skyboxMvpLoc = skyboxShader.getUniformLoc("mvp");
    }

    // Instantiate elements
    BasicShapeElements ground;
    ground.setData(groundVertices, sizeof(groundVertices), groundIndexes, sizeof(groundIndexes));
    ground.enableAttribute(0, 3, 5, 0);
    ground.enableAttribute(1, 2, 5, 3);

    BasicShapeElements river;
    river.setData(riverVertices, sizeof(riverVertices), riverIndexes, sizeof(riverIndexes));
    river.enableAttribute(0, 3, 5, 0);
    river.enableAttribute(1, 2, 5, 3);

    BasicShapeElements hud;
    hud.setData(hudVertices, sizeof(hudVertices), hudIndexes, sizeof(hudIndexes));
    hud.enableAttribute(0, 3, 5, 0);
    hud.enableAttribute(1, 2, 5, 3);

    BasicShapeArrays skybox(skyboxVertices, sizeof(skyboxVertices));
    skybox.enableAttribute(0, 3, 3, 0);


    // Models
    Model suzanne("../models/suzanne.obj");
    Model tree("../models/tree.obj");
    Model rock("../models/rock.obj");
    Model mushroom("../models/mushroom.obj");

    // Textures
    Texture2D suzanneTexture("../models/suzanneTexture.png", GL_CLAMP_TO_BORDER);
    Texture2D treeTexture("../models/treeTexture.png", GL_CLAMP_TO_BORDER);
    Texture2D rockTexture("../models/rockTexture.png", GL_CLAMP_TO_BORDER);
    Texture2D mushroomTexture("../models/mushroomTexture.png", GL_CLAMP_TO_BORDER);
    Texture2D groundTexture("../textures/groundSeamless.jpg", GL_REPEAT);
    Texture2D riverTexture("../textures/waterSeamless.jpg", GL_REPEAT);
    Texture2D hudTexture("../textures/heart.png", GL_CLAMP_TO_BORDER);

    TextureCubeMap skyboxTexture(skyboxPathsindexes);

    // Activate mipmap for repeated textures
    groundTexture.enableMipmap();
    riverTexture.enableMipmap();

    // Groups
    glm::mat4 treeModel[GROUPS_TABLE_SIZE];
    glm::mat4 rockModel[GROUPS_TABLE_SIZE];
    glm::mat4 mushroomModel[GROUPS_TABLE_SIZE];
    glm::vec3 groupsRepartitionMatrix[GROUPS_TABLE_SIZE];

    // Group instances
    {
        glm::mat4 groupsModel[GROUPS_TABLE_SIZE];
        for (int i = 0; i < GROUPS_TABLE_SIZE; ++i) {
            // Random translation transformation for each group
            float randPosX, randPosZ;
            getGroupRandomPos(i, GROUPS_ROWS_SIZE, randPosX, randPosZ);
            groupsRepartitionMatrix[i] = glm::vec3(randPosX, -1.0f, randPosZ);

            // Random scale [0.7; 1.3]
            float groupScale = float(rand01() * 0.6f + 0.7f);

            // Random rotation [0; 2π]
            float groupRotation = float(rand01() * float(2*M_PI));

            // Define group model for all objects in the group
            glm::mat4 groupModel = glm::mat4(1.0f);
            groupModel = glm::translate(groupModel, glm::vec3(randPosX, -1.0f, randPosZ)); // -1.0 in z axis for the ground
            groupModel = glm::scale(groupModel, glm::vec3(groupScale));
            groupModel = glm::rotate(groupModel, groupRotation, glm::vec3(0.0f, 1.0f, 0.0f));
            groupsModel[i] = groupModel;

            // Tree model specific transformation
            float treeScale = float(rand01() * 0.6f + 0.7f);
            groupModel = glm::scale(groupsModel[i], glm::vec3(treeScale));
            treeModel[i] = groupModel;

            // Rock model specific transformation
            float rockRotation = float(rand01() * float(2 * M_PI));
            float rockRadius = float(rand01() + 1.0f);
            groupModel = glm::rotate(groupsModel[i], rockRotation, glm::vec3(0.0f, 1.0f, 0.0f));
            groupModel = glm::translate(groupModel, glm::vec3(0.0f, 0.2f, rockRadius));
            groupModel = glm::scale(groupModel, glm::vec3(0.3f));
            rockModel[i] = groupModel;

            // Mushroom model specific transformation
            groupModel = glm::translate(treeModel[i], glm::vec3(0.3f, 0.0f, 0.3f));
            groupModel = glm::scale(groupModel, glm::vec3(0.05f));
            mushroomModel[i] = groupModel;
        }
    }

    // Instantiate camera
    glm::vec3 pos(0, 0, 0);
    glm::vec2 ori(0, 0);
    Camera camera(pos, ori);

    // Camera orientation and movement variables
    int mouseX, mouseY;
    bool isthirdPerson = false;
    float posSpeed = 0.1f;
    float oriSpeed = 0.05f;

    // Hud scaling and translation variables
    float hudScaleX = 50.0f;
    float hudScaleY = 50.0f;
    float hudTranslationRatio = 6.0f / 4.0f;

    // Enable face culling
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);

    bool isRunning = true;
    while (isRunning)
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        if (w.shouldResize())
            glViewport(0, 0, w.getWidth(), w.getHeight());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Camera movement
        float theta = glm::radians(ori.x);        
        glm::vec3 front(glm::sin(theta), 0., -glm::cos(theta));
        glm::vec3 right = glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f));
        if (w.getKeyHold(Window::Key::W)){
            pos += posSpeed * front;
        }
        if (w.getKeyHold(Window::Key::S)){
            pos -= posSpeed * front;
        }
        if (w.getKeyHold(Window::Key::A)){
            pos -= posSpeed * right;
        }
        if (w.getKeyHold(Window::Key::D)){
            pos += posSpeed * right;
        }

        // Camera orientation
        w.getMouseMotion(mouseX, mouseY);
        ori.x += mouseX * oriSpeed;
        ori.y += mouseY * oriSpeed;
        
        // Camera view matrix change based on mouse scroll direction
        glm::mat4 view = glm::mat4(1.0f);
        switch (w.getMouseScrollDirection()) {
            case 1:
                view = camera.getFirstPersonViewMatrix();
                isthirdPerson = false;
                break;
            case -1:
                view = camera.getThirdPersonViewMatrix();
                isthirdPerson = true;
                break;
            default:
                if (isthirdPerson) {
                    view = camera.getThirdPersonViewMatrix();
                }
                else {
                    view = camera.getFirstPersonViewMatrix();
                }
                break;
        }

        // Projection matrix for most scene objects
        glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)w.getWidth()/(float)w.getHeight(), 0.1f, 200.0f);
        
        // Display matrix for most scene objects
        glm::mat4 displayMatrix = proj * view;

        // Base mvp matrix
        glm::mat4 mvp = displayMatrix;

        // Display river
        riverShader.use();
        glUniform1f(riverTimeLoc, (float)w.getTick() / 1000.f);
        glUniformMatrix4fv(riverMvpLoc, 1, GL_FALSE, &mvp[0][0]);
        riverTexture.use();
        river.draw(GL_TRIANGLES, 6);
        riverTexture.unuse();

        modelShader.use();
        // Display ground
        glUniformMatrix4fv(modelMvpLoc, 1, GL_FALSE, &mvp[0][0]);
        groundTexture.use();
        ground.draw(GL_TRIANGLES, 6);
        groundTexture.unuse();

        // Display suzanne model if in third person view
        glm::mat4 model = glm::mat4(1.0f);
        if (isthirdPerson) {
            model = glm::translate(model, pos + glm::vec3(0.0f, -1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f));
            model = glm::rotate(model, -glm::radians(ori.x) - float(M_PI), glm::vec3(0.0f, 1.0f, 0.0f));
            mvp = displayMatrix * model;
            glUniformMatrix4fv(modelMvpLoc, 1, GL_FALSE, &mvp[0][0]);

            suzanneTexture.use();
            suzanne.draw();
            suzanneTexture.unuse();
        }

        // Display objects groups
        // Display trees
        treeTexture.use();
        for (int i = 0; i < GROUPS_TABLE_SIZE; i++) {
            mvp = displayMatrix * treeModel[i];
            glUniformMatrix4fv(modelMvpLoc, 1, GL_FALSE, &mvp[0][0]);
            tree.draw();            
        }
        treeTexture.unuse();

        // Display rocks
        rockTexture.use();
        for (int i = 0; i < GROUPS_TABLE_SIZE; i++) {            
            mvp = displayMatrix * rockModel[i];
            glUniformMatrix4fv(modelMvpLoc, 1, GL_FALSE, &mvp[0][0]);            
            rock.draw();            
        }
        rockTexture.unuse();

        // Display mushroom
        mushroomTexture.use();
        for (int i = 0; i < GROUPS_TABLE_SIZE; i++) {            
            mvp = displayMatrix * mushroomModel[i];
            glUniformMatrix4fv(modelMvpLoc, 1, GL_FALSE, &mvp[0][0]);            
            mushroom.draw();            
        }
        mushroomTexture.unuse();

        // Display hud
        glDepthFunc(GL_ALWAYS);
        model = glm::translate(glm::mat4(1.0f), glm::vec3(hudScaleX * hudTranslationRatio, hudScaleY * hudTranslationRatio, 0.0f));
        model = glm::scale(model, glm::vec3(hudScaleX, hudScaleY, 1.0f));
        glm::mat4 hudProj = glm::ortho(0.0f, float(w.getWidth()), 0.0f, float(w.getHeight()));
        mvp = hudProj * model;
        glUniformMatrix4fv(modelMvpLoc, 1, GL_FALSE, &mvp[0][0]);
        hudTexture.use();
        hud.draw(GL_TRIANGLES, 6);
        hudTexture.unuse();

        // Display skybox
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        mvp = proj * glm::mat4(glm::mat3(view));
        glUniformMatrix4fv(skyboxMvpLoc, 1, GL_FALSE, &mvp[0][0]);
        skyboxTexture.use();
        skybox.draw(GL_TRIANGLES, 36);
        glDepthFunc(GL_LESS);

        w.swap();
        w.pollEvent();
        isRunning = !w.shouldClose() && !w.getKeyPress(Window::Key::ESC);
    }

    return 0;
}



void checkGLError(const char* file, int line)
{
    GLenum error;
    while ( ( error = glGetError() ) != GL_NO_ERROR )
    {
        std::cerr << "GL_ERROR, File " << file << " (Line " << line << "): ";
        switch ( error )
        {
        case GL_INVALID_ENUM:
            std::cerr << "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            std::cerr << "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            std::cerr << "GL_INVALID_OPERATION";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            std::cerr << "GL_OUT_OF_MEMORY";
            break;
        case GL_STACK_UNDERFLOW:
            std::cerr << "GL_STACK_UNDERFLOW";
            break;
        case GL_STACK_OVERFLOW:
            std::cerr << "GL_STACK_OVERFLOW";
            break;
        default:
            std::cerr << "Unknown gl error occured!";
            break;
        }
        std::cerr << std::endl;
    }
}

void printGLInfo()
{
    std::cout << "OpenGL info:" << std::endl;
    std::cout << "    Vendor: "   << glGetString(GL_VENDOR)   << std::endl;
    std::cout << "    Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "    Version: "  << glGetString(GL_VERSION)  << std::endl;
    std::cout << "    Shading version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

std::string readFile(const char* path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}