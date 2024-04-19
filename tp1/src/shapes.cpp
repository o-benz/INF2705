#include "shapes.h"

BasicShapeArrays::BasicShapeArrays(const GLfloat* data, GLsizeiptr byteSize)
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);
}

BasicShapeArrays::~BasicShapeArrays()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void BasicShapeArrays::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset)
{
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(index);
}

void BasicShapeArrays::draw(GLenum mode, GLsizei count)
{
    glBindVertexArray(m_vao);
    glDrawArrays(mode, 0, count);
    glBindVertexArray(0);
}

BasicShapeMultipleArrays::BasicShapeMultipleArrays(const GLfloat* pos, GLsizeiptr posByteSize, const GLfloat* color, GLsizeiptr colorByteSize)
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_posVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
    glBufferData(GL_ARRAY_BUFFER, posByteSize, pos, GL_STATIC_DRAW);
    glGenBuffers(1, &m_colorVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_colorVbo);
    glBufferData(GL_ARRAY_BUFFER, colorByteSize, color, GL_STATIC_DRAW);
}

BasicShapeMultipleArrays::~BasicShapeMultipleArrays()
{
    glDeleteBuffers(1, &m_posVbo);
    glDeleteBuffers(1, &m_colorVbo);
    glDeleteVertexArrays(1, &m_vao);
}

void BasicShapeMultipleArrays::enablePosAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(index);
}

void BasicShapeMultipleArrays::enableColorAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_colorVbo);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(index);
}

void BasicShapeMultipleArrays::updateColorData(const GLfloat* color, GLsizeiptr colorByteSize)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_colorVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, colorByteSize, color);
}

GLfloat* BasicShapeMultipleArrays::mapPosData()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
    return (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
}

void BasicShapeMultipleArrays::unmapPosData()
{
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void BasicShapeMultipleArrays::draw(GLenum mode, GLsizei count)
{
    glBindVertexArray(m_vao);
    glDrawArrays(mode, 0, count);
    glBindVertexArray(0);
}

BasicShapeElements::BasicShapeElements(const GLfloat* data, GLsizeiptr byteSize, const GLubyte* indexes, GLsizeiptr indexesByteSize)
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexesByteSize, indexes, GL_STATIC_DRAW);
}

BasicShapeElements::~BasicShapeElements()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}

void BasicShapeElements::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset)
{
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(index);
}

void BasicShapeElements::draw(GLenum mode, GLsizei count)
{
    glBindVertexArray(m_vao);
    glDrawElements(mode, count, GL_UNSIGNED_BYTE, 0);
    glBindVertexArray(0);
}
