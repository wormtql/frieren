#include "BuiltinMesh.h"
#include "Vertex.h"
#include <common_include_glm.h>

namespace frieren_core {
    Mesh BuiltinMesh::create_cube_mesh(float size) {
        vector<Vertex> vertices;
        float s = size / 2;
        vertices.push_back(Vertex {
            .position = glm::vec3{s, -s, -s},
            .normal = glm::vec3{0, 0, 0}, // todo
            .uv = glm::vec2{0, 0},
            .tangent = glm::vec3{0, 0, 0},
            .bitangent = glm::vec3{0, 0, 0}
        });
        vertices.push_back(Vertex {
            .position = glm::vec3{s, -s, s}
        });
        vertices.push_back(Vertex {
            .position = glm::vec3{s, s, s}
        });
        vertices.push_back(Vertex {
            .position = glm::vec3{s, s, -s}
        });
        vertices.push_back(Vertex {
            .position = glm::vec3{-s, -s, s}
        });
        vertices.push_back(Vertex {
            .position = glm::vec3{-s, -s, -s}
        });
        vertices.push_back(Vertex {
            .position = glm::vec3{-s, s, -s}
        });
        vertices.push_back(Vertex {
            .position = glm::vec3{-s, s, s}
        });

        vector<uint32_t> indices{{
            0, 2, 1,
            0, 3, 2,
            2, 3, 6,
            2, 6, 7,
            5, 4, 7,
            5, 7, 6,
            1, 4, 5,
            1, 5, 0,
            1, 2, 4,
            4, 2, 7,
            0, 5, 6,
            0, 6, 3
        }};

        Mesh mesh{"frieren::cube"};
        mesh.id = "frieren::cube";
        mesh.vertices = std::move(vertices);
        mesh.indices = std::move(indices);

        return std::move(mesh);
    }
}