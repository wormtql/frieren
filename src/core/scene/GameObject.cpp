#include "GameObject.h"

namespace frieren_core {
    GameObject::GameObject(const string& name): name(name) {
        this->id = next_id++;
    }
}
