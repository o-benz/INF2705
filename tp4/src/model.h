#ifndef MODEL_H
#define MODEL_H

#include "shapes.h"
#include <vector>

class Model
{
public:
	Model(const char* path);
	void draw();

private:
	void loadObj(const char* path, std::vector<GLfloat>& vertexData, std::vector<GLuint>& indices);

private:
	BasicShapeElements m_shape;
	GLsizei m_count;
};


#endif // MODEL_H