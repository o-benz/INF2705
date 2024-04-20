#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include <GL/glew.h>

class UniformBuffer
{
public:
    UniformBuffer(const void* data, GLsizeiptr byteSize);
    ~UniformBuffer();
    
    void setBindingIndex(GLuint index);

    void updateData(const void* data, GLintptr offset, GLsizeiptr byteSize);
    
private:
    GLuint m_ubo;
};

#endif // UNIFORM_BUFFER_H
