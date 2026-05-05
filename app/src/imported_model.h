#pragma once

#include <glm/glm.hpp>
#include <vector>

class ImportedModel {
   private:
    int vertices_num;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> tex_coords;
    std::vector<glm::vec3> normals;

   public:
    ImportedModel(const char* file_path);
    int getVerticesNum();
    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec2> getTexCoords();
    std::vector<glm::vec3> getNormals();
};