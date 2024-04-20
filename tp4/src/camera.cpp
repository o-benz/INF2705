#include "camera.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& position, const glm::vec2& orientation)
: m_position(position)
, m_orientation(orientation)
{

}

glm::mat4 Camera::getFirstPersonViewMatrix()
{
    glm::mat4 view = glm::mat4(1.0);
    view = glm::rotate(view, -m_orientation.x, glm::vec3(1.0, 0.0, 0.0));
    view = glm::rotate(view, -m_orientation.y, glm::vec3(0.0, 1.0, 0.0));
    view = glm::translate(view, -m_position);
    return view;
}

glm::mat4 Camera::getThirdPersonViewMatrix(int distance)
{
    glm::mat4 view = glm::mat4(1.0);
    
    glm::vec3 position = glm::vec3(0);
    glm::vec3 point = glm::vec3(0);
    glm::vec3 up = glm::vec3(0, 1, 0);
    
    const float dist = distance * 0.75f;
    position.x = dist * sin(m_orientation.x) * cos(-m_orientation.y+M_PI_2);
    position.z = dist * sin(m_orientation.x) * sin(-m_orientation.y+M_PI_2);
    position.y = dist * cos(m_orientation.x);
    
    position += m_position;
    point += m_position;
    
    view = glm::lookAt(position, point, up);
    return view;
}

