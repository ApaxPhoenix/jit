#include "vector3.hpp"
#include <algorithm>

Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

Vector3 Vector3::operator+(const Vector3 &other) const {
    return {x + other.x, y + other.y, z + other.z};
}

Vector3 Vector3::operator-(const Vector3 &other) const {
    return {x - other.x, y - other.y, z - other.z};
}

Vector3 Vector3::operator*(const float scalar) const {
    return {x * scalar, y * scalar, z * scalar};
}

Vector3 Vector3::operator*(const Vector3 &other) const {
    return {x * other.x, y * other.y, z * other.z};
}

Vector3 operator*(const float scalar, const Vector3 &v) {
    return v * scalar;
}

Vector3 Vector3::operator/(const float scalar) const {
    return {x / scalar, y / scalar, z / scalar};
}

Vector3 Vector3::operator-() const {
    return {-x, -y, -z};
}

bool Vector3::operator==(const Vector3 &other) const {
    return x == other.x && y == other.y && z == other.z;
}

bool Vector3::operator!=(const Vector3 &other) const {
    return !(*this == other);
}

float Vector3::length() const {
    return sqrtf(x * x + y * y + z * z);
}

float Vector3::dot(const Vector3 &other) const {
    return x * other.x + y * other.y + z * other.z;
}

float Vector3::distance(const Vector3 &other) const {
    return (*this - other).length();
}

float Vector3::angle(const Vector3 &other, const Vector3 *axis) const {
    const float cosine = dot(other) / (length() * other.length());
    const float base = acosf(std::clamp(cosine, -1.0f, 1.0f));

    if (axis == nullptr) return base;

    const Vector3 crossed = cross(other);
    const float sign = crossed.dot(*axis) < 0.0f ? -1.0f : 1.0f;
    return base * sign;
}

Vector3 Vector3::normalized() const {
    const float len = length();
    if (len == 0) return ZERO;
    return *this / len;
}

Vector3 Vector3::cross(const Vector3 &other) const {
    return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    };
}

Vector3 Vector3::interpolate(const Vector3 &other, float factor) const {
    factor = std::clamp(factor, 0.0f, 1.0f);
    return *this + (other - *this) * factor;
}

Vector3 Vector3::absolute() const {
    return {fabsf(x), fabsf(y), fabsf(z)};
}

Vector3 Vector3::reflect(const Vector3 &normal) const {
    return *this - normal * 2.0f * dot(normal);
}

Vector3 Vector3::project(const Vector3 &onto) const {
    const float magnitude = onto.dot(onto);
    if (magnitude == 0) return ZERO;
    return onto * (dot(onto) / magnitude);
}

bool Vector3::approximately(const Vector3 &other, const float epsilon) const {
    return fabsf(x - other.x) <= epsilon &&
           fabsf(y - other.y) <= epsilon &&
           fabsf(z - other.z) <= epsilon;
}

Vector3 Vector3::clamp(const Vector3 &minimum, const Vector3 &maximum) const {
    return {
        std::clamp(x, minimum.x, maximum.x),
        std::clamp(y, minimum.y, maximum.y),
        std::clamp(z, minimum.z, maximum.z)
    };
}

std::ostream &operator<<(std::ostream &os, const Vector3 &v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

const Vector3 Vector3::ZERO    = {0,  0,  0};
const Vector3 Vector3::ONE     = {1,  1,  1};
const Vector3 Vector3::UP      = {0,  1,  0};
const Vector3 Vector3::DOWN    = {0, -1,  0};
const Vector3 Vector3::LEFT    = {-1, 0,  0};
const Vector3 Vector3::RIGHT   = {1,  0,  0};
const Vector3 Vector3::FORWARD = {0,  0,  1};
const Vector3 Vector3::BACK    = {0,  0, -1};