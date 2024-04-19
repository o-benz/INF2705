#ifndef UTILS_H
#define UTILS_H

#include <string>

#define GL_CHECK_ERROR checkGLError(__FILE__, __LINE__)
void checkGLError(const char* file, int line);

std::string readFile(const char* path);

double rand01();

void getGroupRandomPos(int groupId, const int N_ROWS, float& x, float& z);

#endif // UTILS_H