#include "../include/quaternion.hpp"
#include <cassert>

const Quaternion Quaternion::IDENTITY = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

Quaternion::Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

Quaternion::Quaternion(const float x, const float y, const float z, const float w)
    : x(x), y(y), z(z), w(w) {}

Quaternion Quaternion::operator+(const Quaternion &quaternion) const {
    return {x + quaternion.x, y + quaternion.y, z + quaternion.z, w + quaternion.w};
}

Quaternion Quaternion::operator-(const Quaternion &quaternion) const {
    return {x - quaternion.x, y - quaternion.y, z - quaternion.z, w - quaternion.w};
}

Quaternion Quaternion::operator*(const Quaternion &quaternion) const {
    return {
        w * quaternion.x + x * quaternion.w + y * quaternion.z - z * quaternion.y,
        w * quaternion.y - x * quaternion.z + y * quaternion.w + z * quaternion.x,
        w * quaternion.z + x * quaternion.y - y * quaternion.x + z * quaternion.w,
        w * quaternion.w - x * quaternion.x - y * quaternion.y - z * quaternion.z
    };
}

Quaternion Quaternion::operator*(const float scalar) const {
    return {x * scalar, y * scalar, z * scalar, w * scalar};
}

Quaternion &Quaternion::operator+=(const Quaternion &quaternion) {
    x += quaternion.x; y += quaternion.y; z += quaternion.z; w += quaternion.w;
    return *this;
}

Quaternion &Quaternion::operator-=(const Quaternion &quaternion) {
    x -= quaternion.x; y -= quaternion.y; z -= quaternion.z; w -= quaternion.w;
    return *this;
}

Quaternion &Quaternion::operator*=(const Quaternion &quaternion) {
    *this = *this * quaternion;
    return *this;
}

Quaternion &Quaternion::operator*=(const float scalar) {
    x *= scalar; y *= scalar; z *= scalar; w *= scalar;
    return *this;
}

bool Quaternion::operator==(const Quaternion &quaternion) const {
    return x == quaternion.x && y == quaternion.y && z == quaternion.z && w == quaternion.w;
}

bool Quaternion::operator!=(const Quaternion &quaternion) const {
    return !(*this == quaternion);
}

Vector3 Quaternion::operator*(const Vector3 &vector) const {
    const Vector3 imaginary(x, y, z);
    const Vector3 cross1 = imaginary.cross(vector);
    const Vector3 cross2 = imaginary.cross(cross1);
    return vector + ((cross1 * w) + cross2) * 2.0f;
}

Scalar Quaternion::length() const {
    return Scalar(sqrtf(x * x + y * y + z * z + w * w));
}

Scalar Quaternion::dot(const Quaternion &quaternion) const {
    return Scalar(x * quaternion.x + y * quaternion.y + z * quaternion.z + w * quaternion.w);
}

Quaternion Quaternion::normalized() const {
    const float magnitude = static_cast<float>(length());
    assert(magnitude != 0.0f && "Quaternion: cannot normalize zero-length quaternion");
    return *this * (1.0f / magnitude);
}

Quaternion Quaternion::conjugate() const {
    return {-x, -y, -z, w};
}

Quaternion Quaternion::inverse() const {
    const float squared = x * x + y * y + z * z + w * w;
    assert(squared != 0.0f && "Quaternion: cannot invert zero-length quaternion");
    return conjugate() * (1.0f / squared);
}

Quaternion Quaternion::interpolate(const Quaternion &quaternion, float factor) const {
    factor = fmaxf(0.0f, fminf(factor, 1.0f));

    float cosine = static_cast<float>(dot(quaternion));

    Quaternion target = quaternion;
    if (cosine < 0.0f) {
        target = Quaternion(-quaternion.x, -quaternion.y, -quaternion.z, -quaternion.w);
        cosine = -cosine;
    }

    if (cosine > 0.9995f) {
        return Quaternion(
            x + factor * (target.x - x),
            y + factor * (target.y - y),
            z + factor * (target.z - z),
            w + factor * (target.w - w)
        ).normalized();
    }

    const float theta = acosf(cosine);
    const float sine = sinf(theta);
    const float alpha = sinf((1.0f - factor) * theta) / sine;
    const float beta = sinf(factor * theta) / sine;

    return {
        alpha * x + beta * target.x,
        alpha * y + beta * target.y,
        alpha * z + beta * target.z,
        alpha * w + beta * target.w
    };
}

bool Quaternion::approximately(const Quaternion &quaternion, const float epsilon) const {
    return fabsf(x - quaternion.x) <= epsilon &&
           fabsf(y - quaternion.y) <= epsilon &&
           fabsf(z - quaternion.z) <= epsilon &&
           fabsf(w - quaternion.w) <= epsilon;
}

Vector3 Quaternion::euler() const {
    const float roll = atan2f(
        2.0f * (w * x + y * z),
        1.0f - 2.0f * (x * x + y * y)
    );

    const float pitch = fabsf(2.0f * (w * y - z * x)) >= 1.0f
                        ? copysignf(std::numbers::pi_v<float> / 2.0f, 2.0f * (w * y - z * x))
                        : asinf(2.0f * (w * y - z * x));

    const float yaw = atan2f(
        2.0f * (w * z + x * y),
        1.0f - 2.0f * (y * y + z * z)
    );

    constexpr float degrees = 180.0f / std::numbers::pi_v<float>;
    return {roll * degrees, pitch * degrees, yaw * degrees};
}

Quaternion Quaternion::euler(const Vector3 &degrees) {
    constexpr float radians = std::numbers::pi_v<float> / 180.0f;
    const float roll  = degrees.x * radians * 0.5f;
    const float pitch = degrees.y * radians * 0.5f;
    const float yaw   = degrees.z * radians * 0.5f;

    return {
        sinf(roll) * cosf(pitch) * cosf(yaw) - cosf(roll) * sinf(pitch) * sinf(yaw),
        cosf(roll) * sinf(pitch) * cosf(yaw) + sinf(roll) * cosf(pitch) * sinf(yaw),
        cosf(roll) * cosf(pitch) * sinf(yaw) - sinf(roll) * sinf(pitch) * cosf(yaw),
        cosf(roll) * cosf(pitch) * cosf(yaw) + sinf(roll) * sinf(pitch) * sinf(yaw)
    };
}

Quaternion Quaternion::between(const Vector3 &from, const Vector3 &to) {
    assert(static_cast<float>(from.length()) > 0.0f && "Quaternion::between: from vector must be non-zero");
    assert(static_cast<float>(to.length()) > 0.0f && "Quaternion::between: to vector must be non-zero");

    const Vector3 origin = from.normalized();
    const Vector3 destination = to.normalized();
    const float cosine = static_cast<float>(origin.dot(destination));

    if (cosine < -0.9999f) {
        Vector3 perpendicular = Vector3(1.0f, 0.0f, 0.0f).cross(origin);
        if (static_cast<float>(perpendicular.length()) < 0.001f)
            perpendicular = Vector3(0.0f, 1.0f, 0.0f).cross(origin);
        return around(180.0f, perpendicular.normalized());
    }

    const Vector3 axis = origin.cross(destination);
    return Quaternion(axis.x, axis.y, axis.z, 1.0f + cosine).normalized();
}

Quaternion Quaternion::look(const Vector3 &forward, const Vector3 &up) {
    assert(static_cast<float>(forward.length()) > 0.0f && "Quaternion::look: forward vector must be non-zero");
    assert(static_cast<float>(up.length()) > 0.0f && "Quaternion::look: up vector must be non-zero");
    return between(Vector3::FORWARD, forward.normalized());
}

Quaternion Quaternion::around(const float angle, const Vector3 &axis) {
    assert(static_cast<float>(axis.length()) > 0.0f && "Quaternion::around: axis must be non-zero");
    constexpr float radians = std::numbers::pi_v<float> / 180.0f;
    const float half = angle * radians * 0.5f;
    const Vector3 norm = axis.normalized();

    return {
        norm.x * sinf(half),
        norm.y * sinf(half),
        norm.z * sinf(half),
        cosf(half)
    };
}

std::ostream &operator<<(std::ostream &os, const Quaternion &quaternion) {
    os << "Quaternion(" << quaternion.x << ", " << quaternion.y << ", " << quaternion.z << ", " << quaternion.w << ")";
    return os;
}