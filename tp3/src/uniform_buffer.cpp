#include "uniform_buffer.h"

UniformBuffer::UniformBuffer(const void* data, GLsizeiptr byteSize)
{
    glGenBuffers(1, &m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    glBufferData(GL_UNIFORM_BUFFER, byteSize, data, GL_DYNAMIC_DRAW);
}

UniformBuffer::~UniformBuffer()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glDeleteBuffers(1, &m_ubo);
}

void UniformBuffer::setBindingIndex(GLuint index)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, m_ubo);
}

void UniformBuffer::updateData(const void* data, GLintptr offset, GLsizeiptr byteSize)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, byteSize, data);
}