#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H


#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

//As funções recebem um vetor 3D e os parâmetros da transformação e retornam um vetor 3D transformado
//Escala
glm::vec3 applyScale(const glm::vec3& vec, float sx, float sy, float sz) {
    glm::mat4 scaleMatrix = glm::mat4(1.0f); //cria a matriz identidade
    scaleMatrix[0][0] = sx;
    scaleMatrix[1][1] = sy;
    scaleMatrix[2][2] = sz;

    return scaleMatrix * glm::vec4(vec, 1.0f);

}

//Translação
glm::vec3 applyTranslation(const glm::vec3& vec, float tx, float ty, float tz) {
    glm::mat4 translationMatrix = glm::mat4(1.0f); //cria a matriz identidade
    translationMatrix[3][0] = tx;
    translationMatrix[3][1] = ty;
    translationMatrix[3][2] = tz;

    return translationMatrix * glm::vec4(vec, 1.0f);

}

//Rotação
glm::vec3 applyRotationX(const glm::vec3& vec, float angle) {
    glm::mat4 rotationMatrix = glm::mat4(1.0f); //cria a matriz identidade
    float radAngle = glm::radians(angle);
    rotationMatrix[1][1] = cos(radAngle);
    rotationMatrix[1][2] = -sin(radAngle);
    rotationMatrix[2][1] = sin(radAngle);
    rotationMatrix[2][2] = cos(radAngle);

     return rotationMatrix * glm::vec4(vec, 1.0f);

}

glm::vec4 applyRotationY(const glm::vec3& vec, float angle) {
    glm::mat4 rotationMatrix = glm::mat4(1.0f); //cria a matriz identidade
    float radAngle = glm::radians(angle);
    rotationMatrix[0][0] = cos(radAngle);
    rotationMatrix[0][2] = sin(radAngle);
    rotationMatrix[2][0] = -sin(radAngle);
    rotationMatrix[2][2] = cos(radAngle);

    return rotationMatrix * glm::vec4(vec, 1.0f);

}

glm::vec4 applyRotationZ(const glm::vec3& vec, float angle) {
    glm::mat4 rotationMatrix = glm::mat4(1.0f); //cria a matriz identidade
    float radAngle = glm::radians(angle);
    rotationMatrix[0][0] = cos(radAngle);
    rotationMatrix[0][1] = -sin(radAngle);
    rotationMatrix[1][0] = sin(radAngle);
    rotationMatrix[1][1] = cos(radAngle);

    return rotationMatrix * glm::vec4(vec, 1.0f);
}


//Função para calcular a matriz de projeção perspectiva
glm::mat4 GetProjectionMatrix(float fovy, float aspectRatio, float near, float far) {
    float fy = 1.0f / tan(glm::radians(fovy) / 2.0f);
    glm::mat4 projectionMatrix = glm::mat4(0.0f);
    projectionMatrix[0][0] = fy / aspectRatio;
    projectionMatrix[1][1] = fy;
    projectionMatrix[2][2] = -(far + near) / (far - near);
    projectionMatrix[2][3] = -1.0f;
    projectionMatrix[3][2] = -(2.0f * far * near) / (far - near);

    return projectionMatrix;
}

#endif