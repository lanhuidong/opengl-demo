#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <vector>

class Torus {
   private:
    int vertices_num;
    int indices_num;
    int prec;
    float inner;
    float outer;
    std::vector<int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> tex_coords;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> stangents;
    std::vector<glm::vec3> ttangents;
    void init();
    float toRadians(float degrees);

   public:
    Torus();
    Torus(float innerRadius, float outerRadius, int prec);
    int getVerticesNum();
    int getIndicesNum();
    std::vector<int> getIndices();
    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec2> getTexCoords();
    std::vector<glm::vec3> getNormals();
    std::vector<glm::vec3> getStangents();
    std::vector<glm::vec3> getTtangents();
};