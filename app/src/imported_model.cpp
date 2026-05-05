#include "imported_model.h"

#include <fstream>
#include <glm/glm.hpp>
#include <sstream>

#include "model_importer.h"

ImportedModel::ImportedModel(const char* file_path) {
    ModelImporter importer;
    importer.parseOBJ(file_path);
    vertices_num = importer.getVerticesNum();
    std::vector<float> verts = importer.getVertices();
    std::vector<float> tcs = importer.getTextureCoordinates();
    std::vector<float> nors = importer.getNormals();

    for (int i = 0; i < vertices_num; ++i) {
        vertices.push_back(glm::vec3(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]));
        tex_coords.push_back(glm::vec2(tcs[i * 2], tcs[i * 2 + 1]));
        normals.push_back(glm::vec3(nors[i * 3], nors[i * 3 + 1], nors[i * 3 + 2]));
    }
}

int ImportedModel::getVerticesNum() {
    return vertices_num;
}

std::vector<glm::vec3> ImportedModel::getVertices() {
    return vertices;
}

std::vector<glm::vec2> ImportedModel::getTexCoords() {
    return tex_coords;
}

std::vector<glm::vec3> ImportedModel::getNormals() {
    return normals;
}
