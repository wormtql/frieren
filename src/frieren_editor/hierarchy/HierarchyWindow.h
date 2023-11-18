#ifndef FRIEREN_CORE_HIERARCHYWINDOW_H
#define FRIEREN_CORE_HIERARCHYWINDOW_H

#include <scene/GameObject.h>
#include <components/Transform.h>
#include <vector>
#include <optional>
#include <functional>
#include <string>

using namespace std;
using namespace frieren_core;

namespace frieren_editor {
    class HierarchyWindow {
    private:
        void draw_node(shared_ptr<GameObject> go);
    public:
        bool show_window;

        optional<string> selected_game_object_id;
        optional<function<void(const string&)>> on_select_game_object;

        void draw(const vector<shared_ptr<GameObject>>& objects);
    };
}

#endif //FRIEREN_CORE_HIERARCHYWINDOW_H
