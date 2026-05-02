#include "sphere.h"

#include <cmath>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

Sphere::Sphere() {
    init(48);
}

Sphere::Sphere(int prec) {
    init(prec);
}

float Sphere::toRadians(float degrees) {
    return (degrees * 2.0f * 3.14159f) / 360.0f;
}

void Sphere::init(int prec) {
    vertices_num = (prec + 1) * (prec + 1);
    indices_num = prec * prec * 6;
    for (int i = 0; i < vertices_num; i++) {
        vertices.push_back(glm::vec3());
    }
    for (int i = 0; i < vertices_num; i++) {
        tex_coords.push_back(glm::vec2());
    }
    for (int i = 0; i < vertices_num; i++) {
        normals.push_back(glm::vec3());
    }
    for (int i = 0; i < indices_num; i++) {
        indices.push_back(0);
    }

    // calculate triangle vertices
    for (int i = 0; i <= prec; i++) {
        for (int j = 0; j <= prec; j++) {
            float y = (float)cos(toRadians(180.0f - i * 180.0f / prec));
            float x = -(float)cos(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
            float z = (float)sin(toRadians(j * 360.0f / (float)(prec))) * (float)abs(cos(asin(y)));
            vertices[i * (prec + 1) + j] = glm::vec3(x, y, z);
            tex_coords[i * (prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec));
            normals[i * (prec + 1) + j] = glm::vec3(x, y, z);
        }
    }
    // calculate triangle indices
    for (int i = 0; i < prec; i++) {
        for (int j = 0; j < prec; j++) {
            indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
            indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
            indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
        }
    }
}

int Sphere::getVerticesNum() {
    return vertices_num;
}

int Sphere::getIndicesNum() {
    return indices_num;
}

std::vector<int> Sphere::getIndices() {
    return indices;
}

std::vector<glm::vec3> Sphere::getVertices() {
    return vertices;
}

std::vector<glm::vec2> Sphere::getTexCoords() {
    return tex_coords;
}

std::vector<glm::vec3> Sphere::getNormals() {
    return normals;
}
