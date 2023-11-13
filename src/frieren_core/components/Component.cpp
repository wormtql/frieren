#include "Component.h"
#include <nanoid/nanoid.h>

namespace frieren_core::component {
    Component::Component() {
        this->id = nanoid::generate();
    }

    optional<shared_ptr<GameObject>> Component::get_game_object() const {
        if (shared_ptr<GameObject> ret = this->game_object.object.lock()) {
            return ret;
        }
        return {};
    }

    const string& Component::get_id() const {
        return id;
    }
}
