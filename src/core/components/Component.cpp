#include "Component.h"

namespace frieren_core::component {
    int Component::next_id = 0;

    Component::Component() {
        this->id = next_id++;
    }

    void Component::set_parent(shared_ptr<GameObject> parent) {
        this->game_object = parent;
    }

    optional<shared_ptr<GameObject>> Component::get_game_object() const {
        if (!game_object.has_value()) {
            return {};
        }
        if (shared_ptr<GameObject> ret = this->game_object.value().lock()) {
            return ret;
        }
        return {};
    }
}
