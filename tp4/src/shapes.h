#ifndef SHAPES_H
#define SHAPES_H

#include <GL/glew.h>

class BasicShapeArrays
{
public:
    BasicShapeArrays();
    BasicShapeArrays(const GLfloat* data, GLsizeiptr byteSize);
    ~BasicShapeArrays();

    void setData(const GLfloat* data, GLsizeiptr byteSize);
    
    void enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset);
    void draw(GLenum mode, GLsizei count);
    
private:
    GLuint m_vao;
    GLuint m_vbo;
};


class BasicShapeMultipleArrays
{
public:
    BasicShapeMultipleArrays(const GLfloat* pos, GLsizeiptr posByteSize, const GLfloat* color, GLsizeiptr colorByteSize);
    ~BasicShapeMultipleArrays();
    
    void enablePosAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset);    
    void enableColorAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset);
    
    void updateColorData(const GLfloat* color, GLsizeiptr colorByteSize);
    
    GLfloat* mapPosData();    
    void unmapPosData();
    
    void draw(GLenum mode, GLsizei count);
    
private:
    GLuint m_vao;
    GLuint m_posVbo;
    GLuint m_colorVbo;
};


class BasicShapeElements
{
public:
	BasicShapeElements();
    BasicShapeElements(const GLfloat* data, GLsizeiptr byteSize, const GLuint* indexes, GLsizeiptr indexesByteSize);    
    ~BasicShapeElements();
    
    void setData(const GLfloat* data, GLsizeiptr byteSize, const GLuint* indexes, GLsizeiptr indexesByteSize);    
    
    void enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset);    
    void draw(GLenum mode, GLsizei count);
    
private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
};

#endif // SHAPES_H
