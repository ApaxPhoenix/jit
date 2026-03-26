#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <ostream>

struct Vector3 {
    float x, y, z;

    Vector3();

    Vector3(float x, float y, float z);

    Vector3 operator+(const Vector3 &other) const;

    Vector3 operator-(const Vector3 &other) const;

    Vector3 operator*(float scalar) const;

    Vector3 operator*(const Vector3 &other) const;

    friend Vector3 operator*(float scalar, const Vector3 &v);

    Vector3 operator/(float scalar) const;

    Vector3 operator-() const;

    bool operator==(const Vector3 &other) const;

    bool operator!=(const Vector3 &other) const;

    [[nodiscard]] float length() const;

    [[nodiscard]] float dot(const Vector3 &other) const;

    [[nodiscard]] float distance(const Vector3 &other) const;

    [[nodiscard]] float angle(const Vector3 &other, const Vector3 *axis = nullptr) const;

    [[nodiscard]] Vector3 normalized() const;

    [[nodiscard]] Vector3 cross(const Vector3 &other) const;

    [[nodiscard]] Vector3 interpolate(const Vector3 &other, float factor) const;

    [[nodiscard]] Vector3 absolute() const;

    [[nodiscard]] Vector3 reflect(const Vector3 &normal) const;

    [[nodiscard]] Vector3 project(const Vector3 &onto) const;

    [[nodiscard]] bool approximately(const Vector3 &other, float epsilon = 1e-5f) const;

    [[nodiscard]] Vector3 clamp(const Vector3 &minimum, const Vector3 &maximum) const;

    friend std::ostream &operator<<(std::ostream &os, const Vector3 &v);

    static const Vector3 ZERO;
    static const Vector3 ONE;
    static const Vector3 UP;
    static const Vector3 DOWN;
    static const Vector3 LEFT;
    static const Vector3 RIGHT;
    static const Vector3 FORWARD;
    static const Vector3 BACK;
};

#endif