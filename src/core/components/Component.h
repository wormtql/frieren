#ifndef FRIEREN_CORE_COMPONENT_H
#define FRIEREN_CORE_COMPONENT_H

#include <common_include.h>
#include <utilities/LazyRef.h>

using namespace std;
using frieren_core::utils::LazyWeakRef;
using frieren_core::utils::LazyRef;

namespace frieren_core {
    class GameObject;

    namespace component {
        class Component {
        protected:
            string id;
            LazyWeakRef<GameObject> game_object;
        public:
            Component();
            virtual ~Component() = default;

            virtual void link_referenced_components(const map<string, shared_ptr<Component>>& components) {}

            optional<shared_ptr<GameObject>> get_game_object() const;

            const string& get_id() const;
        };
    }
}

#endif
