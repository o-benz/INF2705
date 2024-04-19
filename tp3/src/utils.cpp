#include "utils.h"

#include <random>
#include <chrono>

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

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


std::string readFile(const char* path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

double rand01()
{
	static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	static std::uniform_real_distribution<double> distribution(0, 1);
	return distribution(generator);
}


void getGroupRandomPos(int groupId, const int N_ROWS, float& x, float& z)
{
	const float GROUP_RADIUS = 3.5;
	const float GROUP_WIDTH = 2 * GROUP_RADIUS;
	const float DELTA_DIST_WITH_MARGIN = GROUP_WIDTH + 1;

	x = (float)(groupId % N_ROWS - N_ROWS / 2);
	z = (float)(groupId / N_ROWS - N_ROWS / 2);

	float randPosX = x * DELTA_DIST_WITH_MARGIN + (-GROUP_RADIUS + rand01() * GROUP_WIDTH);
	float randPosY = z * DELTA_DIST_WITH_MARGIN + (-GROUP_RADIUS + rand01() * GROUP_WIDTH);

	x = randPosX;
	z = randPosY;
}