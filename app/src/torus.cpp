#include "torus.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

Torus::Torus() {
    prec = 48;
    inner = 0.5f;
    outer = 0.2f;
    init();
}

Torus::Torus(float innerRadius, float outerRadius, int precIn) {
    prec = precIn;
    inner = innerRadius;
    outer = outerRadius;
    init();
}

float Torus::toRadians(float degress) {
    return (degress * 2.0f * 3.14159f) / 360.0f;
}

void Torus::init() {
    vertices_num = (prec + 1) * (prec + 1);
    indices_num = prec * prec * 6;
    for (int i = 0; i < vertices_num; ++i) {
        vertices.push_back(glm::vec3());
        tex_coords.push_back(glm::vec2());
        normals.push_back(glm::vec3());
        stangents.push_back(glm::vec3());
        ttangents.push_back(glm::vec3());
    }

    for (int i = 0; i < indices_num; ++i) {
        indices.push_back(0);
    }

    for (int i = 0; i < prec + 1; ++i) {
        float amt = toRadians(i * 360.0f / prec);

        glm::mat4 r_mat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec3 init_pos(r_mat * glm::vec4(0.0f, outer, 0.0f, 1.0f));

        vertices[i] = glm::vec3(init_pos + glm::vec3(inner, 0.0f, 0.0f));
        tex_coords[i] = glm::vec2(0.0f, ((float)i / (float)prec));

        r_mat = glm::rotate(glm::mat4(1.0f), amt + (3.14159f / 2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        stangents[i] = glm::vec3(r_mat * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));

        ttangents[i] = glm::vec3(glm::vec3(0.0f, 0.0f, -1.0f));
        normals[i] = glm::cross(ttangents[i], stangents[i]);
    }

    for (int ring = 1; ring < prec + 1; ++ring) {
        for (int i = 0; i < prec + 1; ++i) {
            float amt = (float)(toRadians(ring * 360.0f / prec));
            glm::mat4 r_mat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
            vertices[ring * (prec + 1) + i] = glm::vec3(r_mat * glm::vec4(vertices[i], 1.0f));

            tex_coords[ring * (prec + 1) + i] = glm::vec2((float)ring * 2.0f / (float)prec, tex_coords[i].t);

            r_mat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
            stangents[ring * (prec + 1) + i] = glm::vec3(r_mat * glm::vec4(stangents[i], 1.0f));

            r_mat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
            ttangents[ring * (prec + 1) + i] = glm::vec3(r_mat * glm::vec4(ttangents[i], 1.0f));

            r_mat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
            normals[ring * (prec + 1) + i] = glm::vec3(r_mat * glm::vec4(normals[i], 1.0f));
        }
    }

    for (int ring = 0; ring < prec; ++ring) {
        for (int i = 0; i < prec; ++i) {
            indices[((ring * prec + i) * 2) * 3 + 0] = ring * (prec + 1) + i;
            indices[((ring * prec + i) * 2) * 3 + 1] = (ring + 1) * (prec + 1) + i;
            indices[((ring * prec + i) * 2) * 3 + 2] = ring * (prec + 1) + i + 1;
            indices[((ring * prec + i) * 2 + 1) * 3 + 0] = ring * (prec + 1) + i + 1;
            indices[((ring * prec + i) * 2 + 1) * 3 + 1] = (ring + 1) * (prec + 1) + i;
            indices[((ring * prec + i) * 2 + 1) * 3 + 2] = (ring + 1) * (prec + 1) + i + 1;
        }
    }
}

int Torus::getVerticesNum() {
    return vertices_num;
}

int Torus::getIndicesNum() {
    return indices_num;
}

std::vector<int> Torus::getIndices() {
    return indices;
}

std::vector<glm::vec3> Torus::getVertices() {
    return vertices;
}

std::vector<glm::vec2> Torus::getTexCoords() {
    return tex_coords;
}

std::vector<glm::vec3> Torus::getNormals() {
    return normals;
}

std::vector<glm::vec3> Torus::getStangents() {
    return stangents;
}

std::vector<glm::vec3> Torus::getTtangents() {
    return ttangents;
}
