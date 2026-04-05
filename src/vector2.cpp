#include "../include/vector2.hpp"
#include <cassert>
#include <numbers>

const Vector2 Vector2::ZERO  = Vector2( 0.0f,  0.0f);
const Vector2 Vector2::ONE   = Vector2( 1.0f,  1.0f);
const Vector2 Vector2::UP    = Vector2( 0.0f,  1.0f);
const Vector2 Vector2::DOWN  = Vector2( 0.0f, -1.0f);
const Vector2 Vector2::LEFT  = Vector2(-1.0f,  0.0f);
const Vector2 Vector2::RIGHT = Vector2( 1.0f,  0.0f);

Vector2::Vector2() : x(0.0f), y(0.0f) {}
Vector2::Vector2(const float x, const float y) : x(x), y(y) {}

Vector2 Vector2::operator+(const Vector2 &vector) const { return {this->x + vector.x, this->y + vector.y}; }
Vector2 &Vector2::operator+=(const Vector2 &vector) { this->x += vector.x; this->y += vector.y; return *this; }
Vector2 Vector2::operator-(const Vector2 &vector) const { return {this->x - vector.x, this->y - vector.y}; }
Vector2 &Vector2::operator-=(const Vector2 &vector) { this->x -= vector.x; this->y -= vector.y; return *this; }
Vector2 Vector2::operator*(const float scalar) const { return {this->x * scalar, this->y * scalar}; }
Vector2 &Vector2::operator*=(const float scalar) { this->x *= scalar; this->y *= scalar; return *this; }
Vector2 Vector2::operator*(const Vector2 &vector) const { return {this->x * vector.x, this->y * vector.y}; }
Vector2 &Vector2::operator*=(const Vector2 &vector) { this->x *= vector.x; this->y *= vector.y; return *this; }
Vector2 operator*(const float scalar, const Vector2 &vector) { return vector * scalar; }
Vector2 Vector2::operator-() const { return {-this->x, -this->y}; }
bool Vector2::operator==(const Vector2 &vector) const { return this->x == vector.x && this->y == vector.y; }
bool Vector2::operator!=(const Vector2 &vector) const { return !(*this == vector); }

Vector2 Vector2::operator/(const float scalar) const {
    assert(scalar != 0.0f && "Vector2: division by zero scalar");
    return {this->x / scalar, this->y / scalar};
}

Vector2 &Vector2::operator/=(const float scalar) {
    assert(scalar != 0.0f && "Vector2: division by zero scalar");
    this->x /= scalar; this->y /= scalar;
    return *this;
}

Scalar Vector2::length() const { return Scalar(sqrtf(this->x * this->x + this->y * this->y)); }
Scalar Vector2::dot(const Vector2 &vector) const { return Scalar(this->x * vector.x + this->y * vector.y); }
Scalar Vector2::distance(const Vector2 &vector) const { return (*this - vector).length(); }
Scalar Vector2::cross(const Vector2 &vector) const { return Scalar(this->x * vector.y - this->y * vector.x); }
Scalar Vector2::dominant() const { return Scalar(fmaxf(this->x, this->y)); }
Scalar Vector2::recessive() const { return Scalar(fminf(this->x, this->y)); }

Scalar Vector2::angle(const Vector2 &vector) const {
    assert(this->length() > Scalar(0.0f) && "Vector2::angle: this vector must be non-zero");
    assert(vector.length() > Scalar(0.0f) && "Vector2::angle: other vector must be non-zero");
    const float clamped = fmaxf(-1.0f, fminf(1.0f, static_cast<float>(this->dot(vector)) / static_cast<float>(this->length() * vector.length())));
    return Scalar(acosf(clamped) * (180.0f / std::numbers::pi_v<float>));
}

Vector2 Vector2::normalized() const {
    assert(this->length() != Scalar(0.0f) && "Vector2: cannot normalize zero-length vector");
    return *this / static_cast<float>(this->length());
}

Vector2 Vector2::interpolate(const Vector2 &vector, float factor) const {
    factor = fmaxf(0.0f, fminf(factor, 1.0f));
    return {this->x + (vector.x - this->x) * factor, this->y + (vector.y - this->y) * factor};
}

Vector2 Vector2::absolute() const { return {fabsf(this->x), fabsf(this->y)}; }
Vector2 Vector2::perpendicular() const { return {-this->y, this->x}; }

Vector2 Vector2::reflect(const Vector2 &normal) const {
    assert(normal.length() > Scalar(0.0f) && "Vector2::reflect: normal must be non-zero");
    return *this - normal * (2.0f * static_cast<float>(this->dot(normal)));
}

Vector2 Vector2::project(const Vector2 &onto) const {
    assert(onto.length() > Scalar(0.0f) && "Vector2::project: onto vector must be non-zero");
    return onto * (static_cast<float>(this->dot(onto)) / static_cast<float>(onto.dot(onto)));
}

Vector2 Vector2::clamped(const Vector2 &minimum, const Vector2 &maximum) const {
    assert(minimum.x <= maximum.x && "Vector2::clamped: minimum.x must be <= maximum.x");
    assert(minimum.y <= maximum.y && "Vector2::clamped: minimum.y must be <= maximum.y");
    return {fmaxf(minimum.x, fminf(this->x, maximum.x)), fmaxf(minimum.y, fminf(this->y, maximum.y))};
}

bool Vector2::approximately(const Vector2 &vector, const float epsilon) const {
    return fabsf(this->x - vector.x) <= epsilon && fabsf(this->y - vector.y) <= epsilon;
}

Vector2 Vector2::minimum(const Vector2 &alpha, const Vector2 &beta) { return {fminf(alpha.x, beta.x), fminf(alpha.y, beta.y)}; }
Vector2 Vector2::maximum(const Vector2 &alpha, const Vector2 &beta) { return {fmaxf(alpha.x, beta.x), fmaxf(alpha.y, beta.y)}; }

std::ostream &operator<<(std::ostream &os, const Vector2 &vector) {
    os << "Vector2(" << vector.x << ", " << vector.y << ")";
    return os;
}