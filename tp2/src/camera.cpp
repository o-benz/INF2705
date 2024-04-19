#include "camera.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera(const glm::vec3& position, const glm::vec2& orientation) : m_position(position), m_orientation(orientation)
{
	// Initialisation des attributs de la classe
}

glm::mat4 Camera::getFirstPersonViewMatrix()
{
	// Calcul et retour de la matrice de vue en première personne
	// à partir des attributs de la classe
	float yaw = glm::radians(m_orientation.x);

	glm::mat4 view = glm::mat4(1.0f);

	view = glm::rotate(view, glm::radians(m_orientation.x), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::rotate(view, glm::radians(m_orientation.y), glm::vec3(glm::cos(yaw), 0.0f, glm::sin(yaw)));

	view = glm::translate(view, -m_position);

	return view;
}

 glm::mat4 Camera::getThirdPersonViewMatrix()
 {
 	// Calcul et retour de la matrice de vue en troisième personne
 	// à partir des attributs de la classe
	 float radius = 6.0f;
	 float yaw = glm::radians(m_orientation.x);
	 float pitch = glm::radians(m_orientation.y);

	 glm::vec3 cameraPosition = glm::vec3(
		 radius * glm::cos(pitch) * glm::sin(-yaw),
		 radius * glm::sin(pitch),
		 radius * glm::cos(pitch) * glm::cos(-yaw)
	 );

	 cameraPosition += m_position;

	 glm::mat4 view = glm::lookAt(cameraPosition, m_position, glm::vec3(0.0f, 1.0f, 0.0f));

	 return view;
 }
