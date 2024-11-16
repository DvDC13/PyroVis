#pragma once

#include <functional>

namespace Pyro
{
    // http://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
    template <typename T, typename... Rest>
    void hash_combine(std::size_t& seed, const T& v, const Rest&... rest) {
        seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        (hash_combine(seed, rest), ...);
    }
}