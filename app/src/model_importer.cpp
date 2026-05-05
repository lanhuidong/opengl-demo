#include "model_importer.h"

#include <fstream>
#include <sstream>
#include <string>

void ModelImporter::parseOBJ(const char* file_path) {
    float x, y, z;
    std::string content;
    std::ifstream file_stream(file_path, std::ios::in);
    std::string line = "";
    while (!file_stream.eof()) {
        getline(file_stream, line);
        if (line.compare(0, 2, "v ") == 0) {
            std::stringstream ss(line.erase(0, 2));
            ss >> x;
            ss >> y;
            ss >> z;
            vert_vals.push_back(x);
            vert_vals.push_back(y);
            vert_vals.push_back(z);
        }

        if (line.compare(0, 3, "vt ") == 0) {
            std::stringstream ss(line.erase(0, 3));
            ss >> x;
            ss >> y;
            st_vals.push_back(x);
            st_vals.push_back(y);
        }

        if (line.compare(0, 3, "vn ") == 0) {
            std::stringstream ss(line.erase(0, 3));
            ss >> x;
            ss >> y;
            ss >> z;
            nor_vals.push_back(x);
            nor_vals.push_back(y);
        }

        if (line.compare(0, 2, "f ") == 0) {
            std::string one_corner, v, t, n;
            std::stringstream ss(line.erase(0, 2));
            for (int i = 0; i < 3; ++i) {
                getline(ss, one_corner, ' ');
                std::stringstream one_corner_ss(one_corner);
                getline(one_corner_ss, v, '/');
                getline(one_corner_ss, t, '/');
                getline(one_corner_ss, n, '/');

                int vert_ref = (stoi(v) - 1) * 3;
                int tc_ref = (stoi(t) - 1) * 2;
                int norm_ref = (stoi(n) - 1) * 3;

                triganle_vertices.push_back(vert_vals[vert_ref]);
                triganle_vertices.push_back(vert_vals[vert_ref + 1]);
                triganle_vertices.push_back(vert_vals[vert_ref + 2]);

                texture_coords.push_back(st_vals[tc_ref]);
                texture_coords.push_back(st_vals[tc_ref + 1]);

                normals.push_back(nor_vals[norm_ref]);
                normals.push_back(nor_vals[norm_ref + 1]);
                normals.push_back(nor_vals[norm_ref + 2]);
            }
        }
    }
}

int ModelImporter::getVerticesNum() {
    return triganle_vertices.size() / 3;
}

std::vector<float> ModelImporter::getVertices() {
    return triganle_vertices;
}

std::vector<float> ModelImporter::getTextureCoordinates() {
    return texture_coords;
}

std::vector<float> ModelImporter::getNormals() {
    return normals;
}
