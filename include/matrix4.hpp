#ifndef MATRIX4_HPP
#define MATRIX4_HPP

#include "scalar.hpp"
#include "vector3.hpp"
#include "quaternion.hpp"
#include <limits>
#include <ostream>

struct Matrix4 {
    float matrix[4][4]{};

    Matrix4() = default;

    explicit Matrix4(const float scalar) {
        matrix[0][0] = scalar;
        matrix[1][1] = scalar;
        matrix[2][2] = scalar;
        matrix[3][3] = scalar;
    }

    float operator()(int row, int column) const;
    float &operator()(int row, int column);

    Matrix4 operator+(const Matrix4 &other) const;
    Matrix4 operator-(const Matrix4 &other) const;
    Matrix4 operator*(const Matrix4 &other) const;
    Matrix4 operator*(float scalar) const;
    Vector3 operator*(const Vector3 &vector) const;

    Matrix4 &operator+=(const Matrix4 &other);
    Matrix4 &operator-=(const Matrix4 &other);
    Matrix4 &operator*=(const Matrix4 &other);
    Matrix4 &operator*=(float scalar);

    bool operator==(const Matrix4 &other) const;
    bool operator!=(const Matrix4 &other) const;

    [[nodiscard]] Matrix4 transposed() const;
    [[nodiscard]] Matrix4 inverted() const;
    [[nodiscard]] Scalar determinant() const;

    [[nodiscard]] bool approximately(const Matrix4 &other, float epsilon = std::numeric_limits<float>::epsilon()) const;

    [[nodiscard]] Vector3 translation() const;
    [[nodiscard]] Quaternion rotation() const;
    [[nodiscard]] Vector3 scale() const;

    [[nodiscard]] Vector3 anterior() const;
    [[nodiscard]] Vector3 posterior() const;
    [[nodiscard]] Vector3 superior() const;
    [[nodiscard]] Vector3 inferior() const;
    [[nodiscard]] Vector3 dexter() const;
    [[nodiscard]] Vector3 sinister() const;

    [[nodiscard]] bool decompose(Vector3 &translation, Quaternion &rotation, Vector3 &scale) const;

    static Matrix4 identity();
    static Matrix4 translate(const Vector3 &translation);
    static Matrix4 rotate(const Quaternion &rotation);
    static Matrix4 scale(const Vector3 &scale);
    static Matrix4 compose(const Vector3 &translation, const Quaternion &rotation, const Vector3 &scale);
    static Matrix4 perspective(float fov, float aspect, float zNear, float zFar);
    static Matrix4 orthographic(float left, float right, float bottom, float top, float zNear, float zFar);
    static Matrix4 look(const Vector3 &eye, const Vector3 &target, const Vector3 &up);

    static const Matrix4 IDENTITY;

    friend std::ostream &operator<<(std::ostream &os, const Matrix4 &other);
};

inline Matrix4 operator*(const float scalar, const Matrix4 &other) {
    return other * scalar;
}

#endif