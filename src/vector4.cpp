#include "../include/vector4.hpp"
#include <cassert>

const Vector4 Vector4::ZERO = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::ONE  = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

Vector4::Vector4() : Vector3(0.0f, 0.0f, 0.0f), w(0.0f) {}
Vector4::Vector4(const float x, const float y, const float z, const float w) : Vector3(x, y, z), w(w) {}
Vector4::Vector4(const Vector3 &vector, const float w) : Vector3(vector), w(w) {}

Vector4 Vector4::operator+(const Vector4 &vector) const { return {this->x + vector.x, this->y + vector.y, this->z + vector.z, this->w + vector.w}; }
Vector4 &Vector4::operator+=(const Vector4 &vector) { this->x += vector.x; this->y += vector.y; this->z += vector.z; this->w += vector.w; return *this; }
Vector4 Vector4::operator-(const Vector4 &vector) const { return {this->x - vector.x, this->y - vector.y, this->z - vector.z, this->w - vector.w}; }
Vector4 &Vector4::operator-=(const Vector4 &vector) { this->x -= vector.x; this->y -= vector.y; this->z -= vector.z; this->w -= vector.w; return *this; }
Vector4 Vector4::operator*(const float scalar) const { return {this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar}; }
Vector4 &Vector4::operator*=(const float scalar) { this->x *= scalar; this->y *= scalar; this->z *= scalar; this->w *= scalar; return *this; }
Vector4 Vector4::operator*(const Vector4 &vector) const { return {this->x * vector.x, this->y * vector.y, this->z * vector.z, this->w * vector.w}; }
Vector4 &Vector4::operator*=(const Vector4 &vector) { this->x *= vector.x; this->y *= vector.y; this->z *= vector.z; this->w *= vector.w; return *this; }
Vector4 operator*(const float scalar, const Vector4 &vector) { return vector * scalar; }
Vector4 Vector4::operator-() const { return {-this->x, -this->y, -this->z, -this->w}; }
bool Vector4::operator==(const Vector4 &vector) const { return this->x == vector.x && this->y == vector.y && this->z == vector.z && this->w == vector.w; }
bool Vector4::operator!=(const Vector4 &vector) const { return !(*this == vector); }

Vector4 Vector4::operator/(const float scalar) const {
    assert(scalar != 0.0f && "Vector4: division by zero scalar");
    return {this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar};
}

Vector4 &Vector4::operator/=(const float scalar) {
    assert(scalar != 0.0f && "Vector4: division by zero scalar");
    this->x /= scalar; this->y /= scalar; this->z /= scalar; this->w /= scalar;
    return *this;
}

Scalar Vector4::length() const { return Scalar(sqrtf(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w)); }
Scalar Vector4::dot(const Vector4 &vector) const { return Scalar(this->x * vector.x + this->y * vector.y + this->z * vector.z + this->w * vector.w); }
Scalar Vector4::distance(const Vector4 &vector) const { return (*this - vector).length(); }
Scalar Vector4::dominant() const { return Scalar(fmaxf(this->x, fmaxf(this->y, fmaxf(this->z, this->w)))); }
Scalar Vector4::recessive() const { return Scalar(fminf(this->x, fminf(this->y, fminf(this->z, this->w)))); }

Vector4 Vector4::normalized() const {
    assert(this->length() != Scalar(0.0f) && "Vector4: cannot normalize zero-length vector");
    return *this / static_cast<float>(this->length());
}

Vector4 Vector4::interpolate(const Vector4 &vector, float factor) const {
    factor = fmaxf(0.0f, fminf(factor, 1.0f));
    return {this->x + (vector.x - this->x) * factor, this->y + (vector.y - this->y) * factor, this->z + (vector.z - this->z) * factor, this->w + (vector.w - this->w) * factor};
}

Vector4 Vector4::absolute() const { return {fabsf(this->x), fabsf(this->y), fabsf(this->z), fabsf(this->w)}; }

Vector4 Vector4::clamped(const Vector4 &minimum, const Vector4 &maximum) const {
    assert(minimum.x <= maximum.x && "Vector4::clamped: minimum.x must be <= maximum.x");
    assert(minimum.y <= maximum.y && "Vector4::clamped: minimum.y must be <= maximum.y");
    assert(minimum.z <= maximum.z && "Vector4::clamped: minimum.z must be <= maximum.z");
    assert(minimum.w <= maximum.w && "Vector4::clamped: minimum.w must be <= maximum.w");
    return {fmaxf(minimum.x, fminf(this->x, maximum.x)), fmaxf(minimum.y, fminf(this->y, maximum.y)), fmaxf(minimum.z, fminf(this->z, maximum.z)), fmaxf(minimum.w, fminf(this->w, maximum.w))};
}

bool Vector4::approximately(const Vector4 &vector, const float epsilon) const {
    return fabsf(this->x - vector.x) <= epsilon && fabsf(this->y - vector.y) <= epsilon &&
           fabsf(this->z - vector.z) <= epsilon && fabsf(this->w - vector.w) <= epsilon;
}

Vector4 Vector4::minimum(const Vector4 &alpha, const Vector4 &beta) { return {fminf(alpha.x, beta.x), fminf(alpha.y, beta.y), fminf(alpha.z, beta.z), fminf(alpha.w, beta.w)}; }
Vector4 Vector4::maximum(const Vector4 &alpha, const Vector4 &beta) { return {fmaxf(alpha.x, beta.x), fmaxf(alpha.y, beta.y), fmaxf(alpha.z, beta.z), fmaxf(alpha.w, beta.w)}; }

std::ostream &operator<<(std::ostream &os, const Vector4 &vector) {
    os << "Vector4(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
    return os;
}