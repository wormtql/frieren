#ifndef FRIEREN_CORE_COMPONENT_H
#define FRIEREN_CORE_COMPONENT_H

#include <common_include.h>

using namespace std;

namespace frieren_core {
    class GameObject;

    namespace component {
        class Component {
        private:
            static int next_id;
        protected:
            int id;
            std::optional<std::weak_ptr<GameObject>> game_object = {};
        public:
            Component();

            void set_parent(shared_ptr<GameObject> parent);

            optional<shared_ptr<GameObject>> get_game_object() const;
        };
    }
}

#endif
