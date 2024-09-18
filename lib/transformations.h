#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <glm/glm.hpp>

//transformação de escala
glm::vec4 applyScale(const glm::vec3& vec, float sx, float sy, float sz);

//transformação de translação 
glm::vec4 applyTranslation(const glm::vec3& vec, float tx, float ty, float tz);

//transformação de rotação 
glm::vec4 applyRotationX(const glm::vec3& vec, float angle);
glm::vec4 applyRotationY(const glm::vec3& vec, float angle);
glm::vec4 applyRotationZ(const glm::vec3& vec, float angle);


