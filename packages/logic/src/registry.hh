#pragma once

#include <cstdio>
#include <string>
#include <vector>

template <typename T>
class Registry {
public:
    Registry() : _nextID{1}
    {
        _names.push_back("NONE");
    }

    T lookup(const char* name) const
    {
        for (typename T::Type id = 1; id < _nextID; ++id) {
            if (_names[id] == name) {
                return T(id);
            }
        }
        return T{};
    }

    T reg(const char* name)
    {
        T existing;

        existing = lookup(name);
        if (existing) {
            return existing;
        }
        T newID = T(_nextID++);
        _names.push_back(name);
        printf("[info] Registered %s with ID %u\n", name, newID.value());
        return newID;
    }

private:
    T::Type                     _nextID;
    std::vector<std::string>    _names;
};
