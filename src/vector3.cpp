#include "../include/vector3.hpp"
#include <cassert>
#include <numbers>

const Vector3 Vector3::ZERO    = Vector3( 0.0f,  0.0f,  0.0f);
const Vector3 Vector3::ONE     = Vector3( 1.0f,  1.0f,  1.0f);
const Vector3 Vector3::UP      = Vector3( 0.0f,  1.0f,  0.0f);
const Vector3 Vector3::DOWN    = Vector3( 0.0f, -1.0f,  0.0f);
const Vector3 Vector3::LEFT    = Vector3(-1.0f,  0.0f,  0.0f);
const Vector3 Vector3::RIGHT   = Vector3( 1.0f,  0.0f,  0.0f);
const Vector3 Vector3::FORWARD = Vector3( 0.0f,  0.0f, -1.0f);
const Vector3 Vector3::BACK    = Vector3( 0.0f,  0.0f,  1.0f);

Vector3::Vector3() : Vector2(0.0f, 0.0f), z(0.0f) {}
Vector3::Vector3(const float x, const float y, const float z) : Vector2(x, y), z(z) {}
Vector3::Vector3(const Vector2 &vector, const float z) : Vector2(vector), z(z) {}

Vector3 Vector3::operator+(const Vector3 &vector) const { return {this->x + vector.x, this->y + vector.y, this->z + vector.z}; }
Vector3 &Vector3::operator+=(const Vector3 &vector) { this->x += vector.x; this->y += vector.y; this->z += vector.z; return *this; }
Vector3 Vector3::operator-(const Vector3 &vector) const { return {this->x - vector.x, this->y - vector.y, this->z - vector.z}; }
Vector3 &Vector3::operator-=(const Vector3 &vector) { this->x -= vector.x; this->y -= vector.y; this->z -= vector.z; return *this; }
Vector3 Vector3::operator*(const float scalar) const { return {this->x * scalar, this->y * scalar, this->z * scalar}; }
Vector3 &Vector3::operator*=(const float scalar) { this->x *= scalar; this->y *= scalar; this->z *= scalar; return *this; }
Vector3 Vector3::operator*(const Vector3 &vector) const { return {this->x * vector.x, this->y * vector.y, this->z * vector.z}; }
Vector3 &Vector3::operator*=(const Vector3 &vector) { this->x *= vector.x; this->y *= vector.y; this->z *= vector.z; return *this; }
Vector3 operator*(const float scalar, const Vector3 &vector) { return vector * scalar; }
Vector3 Vector3::operator-() const { return {-this->x, -this->y, -this->z}; }
bool Vector3::operator==(const Vector3 &vector) const { return this->x == vector.x && this->y == vector.y && this->z == vector.z; }
bool Vector3::operator!=(const Vector3 &vector) const { return !(*this == vector); }

Vector3 Vector3::operator/(const float scalar) const {
    assert(scalar != 0.0f && "Vector3: division by zero scalar");
    return {this->x / scalar, this->y / scalar, this->z / scalar};
}

Vector3 &Vector3::operator/=(const float scalar) {
    assert(scalar != 0.0f && "Vector3: division by zero scalar");
    this->x /= scalar; this->y /= scalar; this->z /= scalar;
    return *this;
}

Scalar Vector3::length() const { return Scalar(sqrtf(this->x * this->x + this->y * this->y + this->z * this->z)); }
Scalar Vector3::dot(const Vector3 &vector) const { return Scalar(this->x * vector.x + this->y * vector.y + this->z * vector.z); }
Scalar Vector3::distance(const Vector3 &vector) const { return (*this - vector).length(); }
Scalar Vector3::dominant() const { return Scalar(fmaxf(this->x, fmaxf(this->y, this->z))); }
Scalar Vector3::recessive() const { return Scalar(fminf(this->x, fminf(this->y, this->z))); }

Scalar Vector3::angle(const Vector3 &vector, const Vector3 *axis) const {
    assert(this->length() > Scalar(0.0f) && "Vector3::angle: this vector must be non-zero");
    assert(vector.length() > Scalar(0.0f) && "Vector3::angle: other vector must be non-zero");
    const float clamped = fmaxf(-1.0f, fminf(1.0f, static_cast<float>(this->dot(vector)) / static_cast<float>(this->length() * vector.length())));
    float angle = acosf(clamped) * (180.0f / std::numbers::pi_v<float>);
    if (axis != nullptr) {
        assert(axis->length() > Scalar(0.0f) && "Vector3::angle: axis must be non-zero");
        if (this->cross(vector).dot(*axis) < Scalar(0.0f))
            angle = -angle;
    }
    return Scalar(angle);
}

Vector3 Vector3::normalized() const {
    assert(this->length() != Scalar(0.0f) && "Vector3: cannot normalize zero-length vector");
    return *this / static_cast<float>(this->length());
}

Vector3 Vector3::cross(const Vector3 &vector) const {
    return {
        this->y * vector.z - this->z * vector.y,
        this->z * vector.x - this->x * vector.z,
        this->x * vector.y - this->y * vector.x
    };
}

Vector3 Vector3::interpolate(const Vector3 &vector, float factor) const {
    factor = fmaxf(0.0f, fminf(factor, 1.0f));
    return {this->x + (vector.x - this->x) * factor, this->y + (vector.y - this->y) * factor, this->z + (vector.z - this->z) * factor};
}

Vector3 Vector3::absolute() const { return {fabsf(this->x), fabsf(this->y), fabsf(this->z)}; }

Vector3 Vector3::reflect(const Vector3 &normal) const {
    assert(normal.length() > Scalar(0.0f) && "Vector3::reflect: normal must be non-zero");
    return *this - normal * (2.0f * static_cast<float>(this->dot(normal)));
}

Vector3 Vector3::project(const Vector3 &onto) const {
    assert(onto.length() > Scalar(0.0f) && "Vector3::project: onto vector must be non-zero");
    return onto * (static_cast<float>(this->dot(onto)) / static_cast<float>(onto.dot(onto)));
}

Vector3 Vector3::clamped(const Vector3 &minimum, const Vector3 &maximum) const {
    assert(minimum.x <= maximum.x && "Vector3::clamped: minimum.x must be <= maximum.x");
    assert(minimum.y <= maximum.y && "Vector3::clamped: minimum.y must be <= maximum.y");
    assert(minimum.z <= maximum.z && "Vector3::clamped: minimum.z must be <= maximum.z");
    return {fmaxf(minimum.x, fminf(this->x, maximum.x)), fmaxf(minimum.y, fminf(this->y, maximum.y)), fmaxf(minimum.z, fminf(this->z, maximum.z))};
}

bool Vector3::approximately(const Vector3 &vector, const float epsilon) const {
    return fabsf(this->x - vector.x) <= epsilon && fabsf(this->y - vector.y) <= epsilon && fabsf(this->z - vector.z) <= epsilon;
}

Vector3 Vector3::minimum(const Vector3 &alpha, const Vector3 &beta) { return {fminf(alpha.x, beta.x), fminf(alpha.y, beta.y), fminf(alpha.z, beta.z)}; }
Vector3 Vector3::maximum(const Vector3 &alpha, const Vector3 &beta) { return {fmaxf(alpha.x, beta.x), fmaxf(alpha.y, beta.y), fmaxf(alpha.z, beta.z)}; }

std::ostream &operator<<(std::ostream &os, const Vector3 &vector) {
    os << "Vector3(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
    return os;
}