#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H


#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

//As funções recebem um vetor 3D e os parâmetros da transformação e retornam um vetor 3D transformado
//Escala
glm::mat4 applyScale(const glm::mat4& mat, float sx, float sy, float sz) {
    glm::mat4 scaleMatrix = glm::mat4(1.0f); //cria a matriz identidade
    scaleMatrix[0][0] = sx;
    scaleMatrix[1][1] = sy;
    scaleMatrix[2][2] = sz;

    return scaleMatrix * mat;

}

//Translação
glm::mat4 applyTranslation(const glm::mat4& mat, float tx, float ty, float tz) {
    glm::mat4 translationMatrix = glm::mat4(1.0f); //cria a matriz identidade
    translationMatrix[3][0] = tx;
    translationMatrix[3][1] = ty;
    translationMatrix[3][2] = tz;

    return translationMatrix * mat;

}

//Rotação em radianos
glm::mat4 applyRotationX(const glm::mat4& mat, float angle) {
    glm::mat4 rotationMatrix = glm::mat4(1.0f); //cria a matriz identidade
    float radAngle = glm::radians(angle);
    rotationMatrix[1][1] = cos(radAngle);
    rotationMatrix[1][2] = -sin(radAngle);
    rotationMatrix[2][1] = sin(radAngle);
    rotationMatrix[2][2] = cos(radAngle);

     return rotationMatrix * mat;

}

glm::mat4 applyRotationY(const glm::mat4& mat, float angle) {
    glm::mat4 rotationMatrix = glm::mat4(1.0f); //cria a matriz identidade
    float radAngle = glm::radians(angle);
    rotationMatrix[0][0] = cos(radAngle);
    rotationMatrix[0][2] = sin(radAngle);
    rotationMatrix[2][0] = -sin(radAngle);
    rotationMatrix[2][2] = cos(radAngle);

    return rotationMatrix * mat;

}

glm::mat4 applyRotationZ(const glm::mat4& mat, float angle) {
    glm::mat4 rotationMatrix = glm::mat4(1.0f); //cria a matriz identidade
    float radAngle = glm::radians(angle);
    rotationMatrix[0][0] = cos(radAngle);
    rotationMatrix[0][1] = -sin(radAngle);
    rotationMatrix[1][0] = sin(radAngle);
    rotationMatrix[1][1] = cos(radAngle);

    return rotationMatrix * mat;
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