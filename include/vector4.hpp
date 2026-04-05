#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include "vector3.hpp"
#include "scalar.hpp"
#include <limits>
#include <ostream>

struct Vector4 : Vector3 {
    float w;

    Vector4();
    Vector4(float x, float y, float z, float w);
    explicit Vector4(const Vector3 &vector, float w = 1.0f);

    Vector4 operator+(const Vector4 &vector) const;
    Vector4 &operator+=(const Vector4 &vector);

    Vector4 operator-(const Vector4 &vector) const;
    Vector4 &operator-=(const Vector4 &vector);

    Vector4 operator*(float scalar) const;
    Vector4 &operator*=(float scalar);

    Vector4 operator*(const Vector4 &vector) const;
    Vector4 &operator*=(const Vector4 &vector);

    friend Vector4 operator*(float scalar, const Vector4 &vector);

    Vector4 operator/(float scalar) const;
    Vector4 &operator/=(float scalar);

    Vector4 operator-() const;

    bool operator==(const Vector4 &vector) const;
    bool operator!=(const Vector4 &vector) const;

    [[nodiscard]] Scalar length() const;
    [[nodiscard]] Scalar dot(const Vector4 &vector) const;
    [[nodiscard]] Scalar distance(const Vector4 &vector) const;

    [[nodiscard]] Vector4 normalized() const;
    [[nodiscard]] Vector4 interpolate(const Vector4 &vector, float factor) const;
    [[nodiscard]] Vector4 absolute() const;
    [[nodiscard]] Vector4 clamped(const Vector4 &minimum, const Vector4 &maximum) const;

    [[nodiscard]] Scalar dominant() const;
    [[nodiscard]] Scalar recessive() const;

    [[nodiscard]] bool approximately(const Vector4 &vector, float epsilon = std::numeric_limits<float>::epsilon()) const;

    static Vector4 minimum(const Vector4 &alpha, const Vector4 &beta);
    static Vector4 maximum(const Vector4 &alpha, const Vector4 &beta);

    friend std::ostream &operator<<(std::ostream &os, const Vector4 &vector);

    static const Vector4 ZERO;
    static const Vector4 ONE;
};

#endif