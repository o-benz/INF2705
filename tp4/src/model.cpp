#include "model.h"

#include "obj_loader.h"
#include <iostream>

Model::Model(const char* path)
: m_count(0)
{
	std::vector<GLfloat> vertexData;
	std::vector<GLuint> indices;
	loadObj(path, vertexData, indices);

	m_shape.setData((GLfloat*)&vertexData[0], vertexData.size() * sizeof(GLfloat),
		&indices[0], indices.size() * sizeof(unsigned int));
	m_shape.enableAttribute(0, 3, 8, 0);
	m_shape.enableAttribute(1, 2, 8, 3);
	m_shape.enableAttribute(2, 3, 8, 5);
	m_count = indices.size();
}

void Model::loadObj(const char* path, std::vector<GLfloat>& vertexData, std::vector<GLuint>& indices)
{
	objl::Loader loader;
	bool loadout = loader.LoadFile(path);
	if (!loadout)
	{
		std::cout << "Unable to load model " << path << std::endl;
		return;
	}

	for (size_t i = 0; i < loader.LoadedVertices.size(); i++)
	{
		objl::Vector3 p = loader.LoadedVertices[i].Position;
		vertexData.push_back(p.X);
		vertexData.push_back(p.Y);
		vertexData.push_back(p.Z);
		objl::Vector2 t = loader.LoadedVertices[i].TextureCoordinate;
		vertexData.push_back(t.X);
		vertexData.push_back(t.Y);
		objl::Vector3 n = loader.LoadedVertices[i].Normal;
		vertexData.push_back(n.X);
		vertexData.push_back(n.Y);
		vertexData.push_back(n.Z);
	}
	indices = loader.LoadedIndices;
}

void Model::draw()
{
	m_shape.draw(GL_TRIANGLES, m_count);
}