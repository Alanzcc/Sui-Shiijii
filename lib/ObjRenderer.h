#ifndef OBJRENDERER_H
#define OBJRENDERER_H

#include <glad.h>
#include "stb_image.h"
#include <glm/glm.hpp>

#include "Shader.h"
#include "ObjLoader.h"
#include "Vertex.h"

#include <iostream>
#include <vector>

class ObjRenderer {
  public:
      unsigned int VAO, VBO;
      std::vector<Vertex> data;
      std::vector<float> vertices;
      unsigned int texture;
      glm::vec3 specular;
      float shininess;

  ObjRenderer(const std::string &objPath, const std::string &texturePath, glm::vec3 specular, float shininess)
   {
      data = loadObj(objPath.c_str());
      for (auto vert : data) {
          vertices.push_back(vert.position.x);
          vertices.push_back(vert.position.y);
          vertices.push_back(vert.position.z);
          vertices.push_back(vert.normal.x);
          vertices.push_back(vert.normal.y);
          vertices.push_back(vert.normal.z);
          vertices.push_back(vert.texcoord.x);
          vertices.push_back(vert.texcoord.y);
      }

      texture = loadTexture(texturePath.c_str());
      setupMesh();
  }

  void setupMesh() {
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

      glBindVertexArray(VAO);

      // Position attribute
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);

      // Normal attribute
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);

      // Texture coordinate attribute
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
      glEnableVertexAttribArray(2);
  }

  void render(Shader &shader) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);

      shader.setVec3("material.specular", specular);
      shader.setFloat("material.shininess", shininess);

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / 8));
  }

  unsigned int loadTexture(const char *path) {
      unsigned int textureID;
      glGenTextures(1, &textureID);

      int width, height, nrComponents;
      unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
      if (data) {
          GLenum format;
          if (nrComponents == 1)
              format = GL_RED;
          else if (nrComponents == 3)
              format = GL_RGB;
          else if (nrComponents == 4)
              format = GL_RGBA;

          glBindTexture(GL_TEXTURE_2D, textureID);
          glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
          glGenerateMipmap(GL_TEXTURE_2D);

          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      } else {
          std::cout << "Failed to load texture: " << path << std::endl;
      }
      stbi_image_free(data);

      return textureID;
  }

};

#endif //OBJRENDERER_H