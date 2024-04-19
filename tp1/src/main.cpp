
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


void printGLInfo();

#define GL_CHECK_ERROR checkGLError(__FILE__, __LINE__)
void checkGLError(const char* file, int line);

void changeRGB(GLfloat* color);
void changePos(GLfloat* pos, float& cx, float& cy, float& dx, float& dy);

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

    //Instanciation des shaders
    ShaderProgram basicShaderProgram;
    {
        std::string basicVertexShaderSource = readFile("shaders/basic.vs.glsl");
        std::string basicFragmentShaderSource = readFile("shaders/basic.fs.glsl");
        auto src = basicVertexShaderSource.c_str();
        Shader vertexShader(GL_VERTEX_SHADER, src);
        basicShaderProgram.attachShader(vertexShader);
        src = basicFragmentShaderSource.c_str();
        Shader fragmentShader(GL_FRAGMENT_SHADER, src);
        basicShaderProgram.attachShader(fragmentShader);
        basicShaderProgram.link();
    }

    ShaderProgram colorShaderProgram;
    {
        std::string colorVertexShaderSource = readFile("shaders/color.vs.glsl");
        std::string colorFragmentShaderSource = readFile("shaders/color.fs.glsl");
        auto src = colorVertexShaderSource.c_str();
        Shader vertexShader(GL_VERTEX_SHADER, src);
        colorShaderProgram.attachShader(vertexShader);
        src = colorFragmentShaderSource.c_str();
        Shader fragmentShader(GL_FRAGMENT_SHADER, src);
        colorShaderProgram.attachShader(fragmentShader);
        colorShaderProgram.link();
    }

    ShaderProgram transformShaderProgram;
    {
        std::string transformVertexShaderSource = readFile("shaders/transform.vs.glsl");
        std::string transformFragmentShaderSource = readFile("shaders/transform.fs.glsl");
        auto src = transformVertexShaderSource.c_str();
        Shader vertexShader(GL_VERTEX_SHADER, src);
        transformShaderProgram.attachShader(vertexShader);
        src = transformFragmentShaderSource.c_str();
        Shader fragmentShader(GL_FRAGMENT_SHADER, src);
        transformShaderProgram.attachShader(fragmentShader);
        transformShaderProgram.link();
    }

    // Variables pour la mise à jour, ne pas modifier.
    float cx = 0, cy = 0;
    float dx = 0.019;
    float dy = 0.0128;

    float angleDeg = 0.0f;

    GLfloat brightRed[] = {1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat brightGreen[] = {0.0f, 1.0f, 0.0f, 1.0f};
    GLfloat brightBlue[] = {0.2f, 0.2f, 1.0f, 1.0f};


    // Tableau non constant de la couleur
    GLfloat onlyColorTriVertices[] = {
        brightRed[0], brightRed[1], brightRed[2],
        brightGreen[0], brightGreen[1], brightGreen[2],
        brightRed[0], brightRed[1], brightRed[2],
    };

    // Instanciation des formes
    BasicShapeArrays onlyColorTri(triVertices, sizeof(triVertices));
    onlyColorTri.enableAttribute(0, 3, 3*sizeof(GLfloat), 0);

    BasicShapeArrays onlyColorSquare(squareVertices, sizeof(squareVertices));
    onlyColorSquare.enableAttribute(0, 3, 3*sizeof(GLfloat), 0);

    BasicShapeArrays coloredtri(colorTriVertices, sizeof(colorTriVertices));
    coloredtri.enableAttribute(0, 3, 6*sizeof(GLfloat), 0);
    coloredtri.enableAttribute(1, 3, 6*sizeof(GLfloat), 3*sizeof(GLfloat));

    BasicShapeArrays coloredSquare(colorSquareVertices, sizeof(colorSquareVertices));
    coloredSquare.enableAttribute(0, 3, 6*sizeof(GLfloat), 0);
    coloredSquare.enableAttribute(1, 3, 6*sizeof(GLfloat), 3*sizeof(GLfloat));

    BasicShapeMultipleArrays multColoredTri(triVertices, sizeof(triVertices), onlyColorTriVertices, sizeof(onlyColorTriVertices));
    multColoredTri.enablePosAttribute(0, 3, 3*sizeof(GLfloat), 0);
    multColoredTri.enableColorAttribute(1, 3, 3*sizeof(GLfloat), 0);

    BasicShapeElements elementsSquare(colorSquareVerticesReduced, sizeof(colorSquareVerticesReduced), indexes, sizeof(indexes));
    elementsSquare.enableAttribute(0, 3, 6*sizeof(GLfloat), 0);
    elementsSquare.enableAttribute(1, 3, 6*sizeof(GLfloat), 3*sizeof(GLfloat));

    // Instanciation du cube
    BasicShapeElements cube(cubeVertices, sizeof(cubeVertices), cubeIndexes, sizeof(cubeIndexes));
    cube.enableAttribute(0, 3, 6*sizeof(GLfloat), 0);
    cube.enableAttribute(1, 3, 6*sizeof(GLfloat), 3*sizeof(GLfloat));

    // Couleur de remplissage aux fonds.
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClearDepth(1.0f);

    // Activer le depth test.
    glEnable(GL_DEPTH_TEST);

    int selectShape = 0;
    bool isRunning = true;
    while (isRunning)
    {
        if (w.shouldResize())
            glViewport(0, 0, w.getWidth(), w.getHeight());

        // Nettoyer les tampons appropriées.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        if (w.getKey(Window::Key::T))
        {
            selectShape = ++selectShape < 7 ? selectShape : 0;
            std::cout << "Selected shape: " << selectShape << std::endl;
        }

        // Mise à jour des données du triangle
        changeRGB(&onlyColorTriVertices[0]);
        changeRGB(&onlyColorTriVertices[3]);
        changeRGB(&onlyColorTriVertices[6]);
        multColoredTri.updateColorData(onlyColorTriVertices, sizeof(onlyColorTriVertices));

        GLfloat* posPtr = multColoredTri.mapPosData();
        changePos(posPtr, cx, cy, dx, dy);
        multColoredTri.unmapPosData();

        // Utiliser le bon shader programme selon la forme.
        // N'hésiter pas à utiliser le fallthrough du switch case.
        switch (selectShape)
        {
            case 0:
            case 1:
            {
                basicShaderProgram.use();
                GLint globalColorLocation = basicShaderProgram.getUniformLoc("globalColor");
                if (globalColorLocation == -1) {
                    std::cerr << "Could not find uniform variable 'globalColor'\n";
                } else {
                glUniform4f(globalColorLocation, brightBlue[0], brightBlue[1], brightBlue[2], brightBlue[3]);
                }
                break;
            }
            case 2:
            case 3:
            case 4:
            case 5:
            {
                colorShaderProgram.use();
                break;
            }
            case 6:
            {
                transformShaderProgram.use();
                break;
            }
        }

        // Calcul des matrices et envoyer une matrice résultante mvp au shader.
        if (selectShape == 6)
        {
            angleDeg += 0.5f;
            // Utiliser glm pour les calculs de matrices.
            glm::mat4 mvpMatrix(1.0f);
            glm::mat4 modelMatrix(1.0f);
            glm::mat4 viewMatrix(1.0f);
            glm::mat4 projectionMatrix(1.0f);

            modelMatrix = glm::rotate(modelMatrix, glm::radians(angleDeg), glm::vec3(0.1f, 1.0f, 0.1f));
            viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, -0.5f, -2.0f));
            projectionMatrix = glm::perspective(glm::radians(70.0f), (float)w.getWidth() / (float)w.getHeight(), 0.1f, 10.0f);
            mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
            GLint mvpMatrixLocation = transformShaderProgram.getUniformLoc("mvpMatrix");
            if (mvpMatrixLocation == -1) {
                std::cerr << "Could not find uniform variable 'mvpMatrix'\n";
            } else {
                glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, &mvpMatrix[0][0]);
            }
        }

        // Dessiner la forme sélectionnée.
        switch (selectShape)
        {
            case 0:
                onlyColorTri.draw(GL_TRIANGLES, 3);
                break;
            case 1:
                onlyColorSquare.draw(GL_TRIANGLES, 6);
                break;
            case 2:
                coloredtri.draw(GL_TRIANGLES, 3);
                break;
            case 3:
                coloredSquare.draw(GL_TRIANGLES, 6);
                break;
            case 4:
                multColoredTri.draw(GL_TRIANGLES, 3);
                break;
            case 5:
                elementsSquare.draw(GL_TRIANGLES, 6);
                break;
            case 6:
                cube.draw(GL_TRIANGLES, 36);
                break;
        }

        w.swap();
        w.pollEvent();
        isRunning = !w.shouldClose() && !w.getKey(Window::Key::ESC);
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


void changeRGB(GLfloat* color)
{
    unsigned char r = color[0]*255;
    unsigned char g = color[1]*255;
    unsigned char b = color[2]*255;

    if(r > 0 && b == 0)
    {
        r--;
        g++;
    }
    if(g > 0 && r == 0)
    {
        g--;
        b++;
    }
    if(b > 0 && g == 0)
    {
        r++;
        b--;
    }
    color[0] = r/255.0f;
    color[1] = g/255.0f;
    color[2] = b/255.0f;
}

void changePos(GLfloat* pos, float& cx, float& cy, float& dx, float& dy)
{
    if ((cx < -1 && dx < 0) || (cx > 1 && dx > 0))
        dx = -dx;
    pos[0] += dx;
    pos[3] += dx;
    pos[6] += dx;
    cx += dx;
    if ((cy < -1 && dy < 0) || (cy > 1 && dy > 0))
        dy = -dy;
    pos[1] += dy;
    pos[4] += dy;
    pos[7] += dy;
    cy += dy;
}


std::string readFile(const char* path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
