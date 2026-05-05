#pragma once

#include <vector>

class ModelImporter {
   private:
    std::vector<float> vert_vals;
    std::vector<float> st_vals;
    std::vector<float> nor_vals;

    std::vector<float> triganle_vertices;
    std::vector<float> texture_coords;
    std::vector<float> normals;

   public:
    ModelImporter() = default;
    void parseOBJ(const char* file_path);
    int getVerticesNum();
    std::vector<float> getVertices();
    std::vector<float> getTextureCoordinates();
    std::vector<float> getNormals();
};