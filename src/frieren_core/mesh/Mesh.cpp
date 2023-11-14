#include "Mesh.h"
#include <webgpu/webgpu.hpp>
#include <nanoid/nanoid.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace frieren_core {
    void from_json(const json& j, MeshDescriptor& desc) {
        desc.mesh_path = j["mesh_path"].template get<string>();
        desc.name = j["name"].template get<string>();
        desc.id = j["id"].template get<string>();
    }

    Mesh::Mesh(const string& name): name(name) {
        this->id = nanoid::generate();
    }

    Mesh::Mesh(WGPUDevice device, WGPUQueue queue, const MeshDescriptor& desc) {
        this->name = desc.name;
        if (desc.id.empty()) {
            this->id = nanoid::generate();
        } else {
            this->id = desc.id;
        }

        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(desc.mesh_path,
            aiProcess_CalcTangentSpace
            | aiProcess_Triangulate
            | aiProcess_JoinIdenticalVertices
            | aiProcess_SortByPType
        );

        if (nullptr == scene) {
            cerr << "cannot load mesh: " << desc.mesh_path << endl;
            assert("false");
        }

        int mesh_number = scene->mNumMeshes;
        // consider one mesh
        for (int i = 0; i < mesh_number && i < 1; i++) {
            auto mesh = scene->mMeshes[i];
            int vertex_count = mesh->mNumVertices;
            for (int j = 0; j < vertex_count; j++) {
                auto mesh_vertex = mesh->mVertices[j];
                auto normal = mesh->mNormals[j];
                // assume uv in the first channel
                auto uv = mesh->mTextureCoords[0][j];
                Vertex vert{};
                vert.position = { mesh_vertex.x, mesh_vertex.y, mesh_vertex.z };
                vert.normal = { normal.x, normal.y, normal.z };
                vert.uv = { uv.x, uv.y };
                if (mesh->HasTangentsAndBitangents()) {
                    auto tangent = mesh->mTangents[j];
                    vert.tangent = { tangent.x, tangent.y, tangent.z };
                    auto bit = mesh->mBitangents[j];
                    vert.bitangent = { bit.x, bit.y, bit.z };
                }

                this->vertices.emplace_back(vert);
            }

            int face_count = mesh->mNumFaces;
            for (int j = 0; j < face_count; j++) {
                auto face = mesh->mFaces[j];
                assert(face.mNumIndices == 3);
                this->indices.push_back(face.mIndices[0]);
                this->indices.push_back(face.mIndices[1]);
                this->indices.push_back(face.mIndices[2]);
            }
        }

        this->create_gpu_buffer(device);
        this->write_gpu_buffer(queue);
    }

    bool Mesh::is_gpu_buffer_created() {
        return vertex_buffer != nullptr && index_buffer != nullptr;
    }

    void Mesh::create_gpu_buffer(WGPUDevice device) {
        if (vertex_buffer) {
            wgpuBufferDestroy(vertex_buffer);
            wgpuBufferRelease(vertex_buffer);
            vertex_buffer = nullptr;
        }
        if (index_buffer) {
            wgpuBufferDestroy(index_buffer);
            wgpuBufferRelease(index_buffer);
            index_buffer = nullptr;
        }

        WGPUBufferDescriptor vertex_buffer_desc{};
        vertex_buffer_desc.nextInChain = nullptr;
        vertex_buffer_desc.size = vertices.size() * sizeof(Vertex);
        vertex_buffer_desc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex;
        vertex_buffer_desc.mappedAtCreation = false;
        vertex_buffer = wgpuDeviceCreateBuffer(device, &vertex_buffer_desc);

        WGPUBufferDescriptor index_buffer_desc{};
        index_buffer_desc.nextInChain = nullptr;
        index_buffer_desc.size = indices.size() * sizeof(uint32_t);
        index_buffer_desc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Index;
        index_buffer_desc.mappedAtCreation = false;
        index_buffer = wgpuDeviceCreateBuffer(device, &index_buffer_desc);
    }

    void Mesh::write_gpu_buffer(WGPUQueue queue) {
        wgpuQueueWriteBuffer(queue, vertex_buffer, 0, vertices.data(), vertices.size() * sizeof(Vertex));
        wgpuQueueWriteBuffer(queue, index_buffer, 0, indices.data(), indices.size() * sizeof(uint32_t));
    }

    void Mesh::set_buffer_for_render_pass(WGPURenderPassEncoder render_pass) const {
        wgpuRenderPassEncoderSetVertexBuffer(render_pass, 0, vertex_buffer, 0, sizeof(Vertex) * vertices.size());
        wgpuRenderPassEncoderSetIndexBuffer(render_pass, index_buffer, WGPUIndexFormat_Uint32, 0, sizeof(uint32_t) * indices.size());
    }
}
