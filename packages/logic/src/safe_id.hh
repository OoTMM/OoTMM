#pragma once

#include <cstdint>

template <typename T, typename Tag>
class SafeID {
public:
    using Type = T;

    explicit SafeID() : _value{} {}
    explicit SafeID(T value) : _value(value) {}

    T value() const { return _value; }

    constexpr bool valid() const { return _value != T{}; }
    operator bool() const { return valid(); }

    constexpr bool operator==(const SafeID& other) const { return _value == other._value; }
    constexpr bool operator!=(const SafeID& other) const { return _value != other._value; }
    constexpr bool operator<(const SafeID& other) const { return _value < other._value; }
    constexpr bool operator>(const SafeID& other) const { return _value > other._value; }
    constexpr bool operator<=(const SafeID& other) const { return _value <= other._value; }
    constexpr bool operator>=(const SafeID& other) const { return _value >= other._value; }

private:
    T _value;
};
