#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(const glm::vec3& position, const glm::vec2& orientation);
    
    glm::mat4 getFirstPersonViewMatrix();
    glm::mat4 getThirdPersonViewMatrix(int distance);
    
private:
    const glm::vec3& m_position;
    const glm::vec2& m_orientation;

};


#endif // CAMERA_H
