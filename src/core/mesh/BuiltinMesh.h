#ifndef FRIEREN_CORE_BUILTIN_MESH_H
#define FRIEREN_CORE_BUILTIN_MESH_H

#include "Mesh.h"

namespace frieren_core {
    class BuiltinMesh {
    public:
        static Mesh create_cube_mesh(float size);
    };
}

#endif
