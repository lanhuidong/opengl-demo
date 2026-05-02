#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <vector>

class Sphere {
   private:
    int vertices_num;
    int indices_num;
    std::vector<int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> tex_coords;
    std::vector<glm::vec3> normals;
    void init(int);
    float toRadians(float degrees);

   public:
    Sphere();
    Sphere(int prec);
    int getVerticesNum();
    int getIndicesNum();
    std::vector<int> getIndices();
    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec2> getTexCoords();
    std::vector<glm::vec3> getNormals();
};