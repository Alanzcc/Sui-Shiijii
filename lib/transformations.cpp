#include "Transformations.h"
#include <glm/gtc/matrix_transform.hpp>

//As funções recebem um vetor 3D e os parâmetros da transformação e retornam um vetor 3D transformado
//Escala
glm::vec4 applyScale(const glm::vec3& vec, float sx, float sy, float sz) {
    glm::mat4 scaleMatrix = glm::mat4(1.0f) //cria a matriz identidade
    scaleMatrix[0][0] = sx;
    scaleMatrix[1][1] = sy;
    scaleMatrix[2][2] = sz;

    glm::vec4 scaledVec = scaleMatrix * glm::vec4(vec, 1.0f);
    return glm::vec3(scaledVec);
}

//Translação
glm::vec4 applyTranslation(const glm::vec3& vec, float tx, float ty, float tz) {
    glm::mat4 translationMatrix = glm::mat4(1.0f) //cria a matriz identidade
    translationMatrix[3][0] = tx;
    translationMatrix[3][1] = ty;
    translationMatrix[3][2] = tz;

    glm::vec4 translatedVec = translationMatrix * glm::vec4(vec, 1.0f);
    return glm::vec3(translatedVec);
}

//Rotação
glm::vec4 applyRotationX(const glm::vec3& vec, float angle) {
    glm::mat4 rotationMatrix = glm::mat4(1.0f) //cria a matriz identidade
    float radAngle = glm::radians(angle);
    rotationMatrix[1][1] = cos(radAngle);
    rotationMatrix[1][2] = -sin(radAngle);
    rotationMatrix[2][1] = sin(radAngle);
    rotationMatrix[2][2] = cos(radAngle);

    glm::vec4 rotatedVec = rotationMatrix * glm::vec4(vec, 1.0f);
    return glm::vec3(rotatedVec);
}

glm::vec4 applyRotationY(const glm::vec3& vec, float angle) {
    glm::mat4 rotationMatrix = glm::mat4(1.0f) //cria a matriz identidade
    float radAngle = glm::radians(angle);
    rotationMatrix[0][0] = cos(radAngle);
    rotationMatrix[0][2] = sin(radAngle);
    rotationMatrix[2][0] = -sin(radAngle);
    rotationMatrix[2][2] = cos(radAngle);

    glm::vec4 rotatedVec = rotationMatrix * glm::vec4(vec, 1.0f);
    return glm::vec3(rotatedVec);
}

glm::vec4 applyRotationZ(const glm::vec3& vec, float angle) {
    glm::mat4 rotationMatrix = glm::mat4(1.0f) //cria a matriz identidade
    float radAngle = glm::radians(angle);
    rotationMatrix[0][0] = cos(radAngle);
    rotationMatrix[0][1] = -sin(radAngle);
    rotationMatrix[1][0] = sin(radAngle);
    rotationMatrix[1][1] = cos(radAngle);

    glm::vec4 rotatedVec = rotationMatrix * glm::vec4(vec, 1.0f);
    return glm::vec3(rotatedVec);
}