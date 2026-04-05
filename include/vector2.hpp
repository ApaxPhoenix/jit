#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include "scalar.hpp"
#include <limits>
#include <ostream>

struct Vector2 {
    float x, y;

    Vector2();
    Vector2(float x, float y);

    Vector2 operator+(const Vector2 &vector) const;
    Vector2 &operator+=(const Vector2 &vector);

    Vector2 operator-(const Vector2 &vector) const;
    Vector2 &operator-=(const Vector2 &vector);

    Vector2 operator*(float scalar) const;
    Vector2 &operator*=(float scalar);

    Vector2 operator*(const Vector2 &vector) const;
    Vector2 &operator*=(const Vector2 &vector);

    friend Vector2 operator*(float scalar, const Vector2 &vector);

    Vector2 operator/(float scalar) const;
    Vector2 &operator/=(float scalar);

    Vector2 operator-() const;

    bool operator==(const Vector2 &vector) const;
    bool operator!=(const Vector2 &vector) const;

    [[nodiscard]] Scalar length() const;
    [[nodiscard]] Scalar dot(const Vector2 &vector) const;
    [[nodiscard]] Scalar distance(const Vector2 &vector) const;
    [[nodiscard]] Scalar angle(const Vector2 &vector) const;
    [[nodiscard]] Scalar cross(const Vector2 &vector) const;

    [[nodiscard]] Vector2 normalized() const;
    [[nodiscard]] Vector2 interpolate(const Vector2 &vector, float factor) const;
    [[nodiscard]] Vector2 absolute() const;
    [[nodiscard]] Vector2 reflect(const Vector2 &normal) const;
    [[nodiscard]] Vector2 project(const Vector2 &onto) const;
    [[nodiscard]] Vector2 clamped(const Vector2 &minimum, const Vector2 &maximum) const;
    [[nodiscard]] Vector2 perpendicular() const;

    [[nodiscard]] Scalar dominant() const;
    [[nodiscard]] Scalar recessive() const;

    [[nodiscard]] bool approximately(const Vector2 &vector, float epsilon = std::numeric_limits<float>::epsilon()) const;

    static Vector2 minimum(const Vector2 &alpha, const Vector2 &beta);
    static Vector2 maximum(const Vector2 &alpha, const Vector2 &beta);

    friend std::ostream &operator<<(std::ostream &os, const Vector2 &vector);

    static const Vector2 ZERO;
    static const Vector2 ONE;
    static const Vector2 UP;
    static const Vector2 DOWN;
    static const Vector2 LEFT;
    static const Vector2 RIGHT;
};

#endif