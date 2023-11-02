#ifndef FRIEREN_CORE_LAZY_REF_H
#define FRIEREN_CORE_LAZY_REF_H

#include <common_include.h>

using namespace std;

namespace frieren_core::utils {
    template<typename T>
    class LazyRef {
    public:
        shared_ptr<T> object;
        string referenced_id;

        LazyRef() = default;
        explicit LazyRef(const string& id): referenced_id(id) {}

        shared_ptr<T> get() const {
            return object;
        };
    };

    template<typename T>
    class LazyWeakRef {
    public:
        weak_ptr<T> object;
        string referenced_id;

        LazyWeakRef() = default;
        explicit LazyWeakRef(const string& id): referenced_id(id) {}

        weak_ptr<T> get() const {
            return object;
        }
    };
}

#endif
