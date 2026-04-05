#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include "vector2.hpp"
#include "scalar.hpp"
#include <limits>
#include <ostream>

struct Vector3 : Vector2 {
    float z;

    Vector3();
    Vector3(float x, float y, float z);
    explicit Vector3(const Vector2 &vector, float z = 0.0f);

    Vector3 operator+(const Vector3 &vector) const;
    Vector3 &operator+=(const Vector3 &vector);

    Vector3 operator-(const Vector3 &vector) const;
    Vector3 &operator-=(const Vector3 &vector);

    Vector3 operator*(float scalar) const;
    Vector3 &operator*=(float scalar);

    Vector3 operator*(const Vector3 &vector) const;
    Vector3 &operator*=(const Vector3 &vector);

    friend Vector3 operator*(float scalar, const Vector3 &vector);

    Vector3 operator/(float scalar) const;
    Vector3 &operator/=(float scalar);

    Vector3 operator-() const;

    bool operator==(const Vector3 &vector) const;
    bool operator!=(const Vector3 &vector) const;

    [[nodiscard]] Scalar length() const;
    [[nodiscard]] Scalar dot(const Vector3 &vector) const;
    [[nodiscard]] Scalar distance(const Vector3 &vector) const;
    [[nodiscard]] Scalar angle(const Vector3 &vector, const Vector3 *axis = nullptr) const;

    [[nodiscard]] Vector3 normalized() const;
    [[nodiscard]] Vector3 cross(const Vector3 &vector) const;
    [[nodiscard]] Vector3 interpolate(const Vector3 &vector, float factor) const;
    [[nodiscard]] Vector3 absolute() const;
    [[nodiscard]] Vector3 reflect(const Vector3 &normal) const;
    [[nodiscard]] Vector3 project(const Vector3 &onto) const;
    [[nodiscard]] Vector3 clamped(const Vector3 &minimum, const Vector3 &maximum) const;

    [[nodiscard]] Scalar dominant() const;
    [[nodiscard]] Scalar recessive() const;

    [[nodiscard]] bool approximately(const Vector3 &vector, float epsilon = std::numeric_limits<float>::epsilon()) const;

    static Vector3 minimum(const Vector3 &alpha, const Vector3 &beta);
    static Vector3 maximum(const Vector3 &alpha, const Vector3 &beta);

    friend std::ostream &operator<<(std::ostream &os, const Vector3 &vector);

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