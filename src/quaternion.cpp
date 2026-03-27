#include "quaternion.hpp"
#include <algorithm>

const Quaternion Quaternion::IDENTITY = {0, 0, 0, 1};

Quaternion::Quaternion() : x(0), y(0), z(0), w(1) {}

Quaternion::Quaternion(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}

Quaternion Quaternion::operator+(const Quaternion &other) const {
    return {x + other.x, y + other.y, z + other.z, w + other.w};
}

Quaternion Quaternion::operator-(const Quaternion &other) const {
    return {x - other.x, y - other.y, z - other.z, w - other.w};
}

Quaternion Quaternion::operator*(const Quaternion &other) const {
    return {
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w,
        w * other.w - x * other.x - y * other.y - z * other.z
    };
}

Quaternion Quaternion::operator*(const float scalar) const {
    return {x * scalar, y * scalar, z * scalar, w * scalar};
}

bool Quaternion::operator==(const Quaternion &other) const {
    return x == other.x && y == other.y && z == other.z && w == other.w;
}

bool Quaternion::operator!=(const Quaternion &other) const {
    return !(*this == other);
}

Vector3 Quaternion::operator*(const Vector3 &vector) const {
    const Vector3 q(x, y, z);
    const Vector3 crossed = q.cross(vector);
    return vector + crossed * (2.0f * w) + q.cross(crossed) * 2.0f;
}

float Quaternion::length() const {
    return sqrtf(x * x + y * y + z * z + w * w);
}

float Quaternion::dot(const Quaternion &other) const {
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

Quaternion Quaternion::normalized() const {
    const float len = length();
    if (len == 0) return IDENTITY;
    return {x / len, y / len, z / len, w / len};
}

Quaternion Quaternion::conjugate() const {
    return {-x, -y, -z, w};
}

Quaternion Quaternion::inverse() const {
    const float magnitude = dot(*this);
    if (magnitude == 0) return IDENTITY;
    return {-x / magnitude, -y / magnitude, -z / magnitude, w / magnitude};
}

Quaternion Quaternion::interpolate(const Quaternion &other, float factor) const {
    factor = std::clamp(factor, 0.0f, 1.0f);
    float cosine = dot(other);
    Quaternion target = other;

    if (cosine < 0.0f) {
        cosine = -cosine;
        target = {-other.x, -other.y, -other.z, -other.w};
    }

    if (cosine > 0.9995f) {
        return (*this + (target - *this) * factor).normalized();
    }

    const float base  = acosf(cosine);
    const float scale = sinf(base);
    return (*this * sinf(base * (1.0f - factor)) + target * sinf(base * factor)) * (1.0f / scale);
}

bool Quaternion::approximately(const Quaternion &other, const float epsilon) const {
    return fabsf(dot(other)) >= 1.0f - epsilon;
}

Vector3 Quaternion::euler() const {
    Vector3 angles;

    { const float sine   = 2.0f * (w * x + y * z);
      const float cosine = 1.0f - 2.0f * (x * x + y * y);
      angles.x = atan2f(sine, cosine) * (180.0f / static_cast<float>(M_PI)); }

    { const float sine = 2.0f * (w * y - z * x);
      angles.y = fabsf(sine) >= 1.0f
          ? copysignf(90.0f, sine)
          : asinf(sine) * (180.0f / static_cast<float>(M_PI)); }

    { const float sine   = 2.0f * (w * z + x * y);
      const float cosine = 1.0f - 2.0f * (y * y + z * z);
      angles.z = atan2f(sine, cosine) * (180.0f / static_cast<float>(M_PI)); }

    return angles;
}

Quaternion Quaternion::euler(const Vector3 &degrees) {
    constexpr float conversion = static_cast<float>(M_PI) / 180.0f;
    const float hx = degrees.x * conversion * 0.5f;
    const float hy = degrees.y * conversion * 0.5f;
    const float hz = degrees.z * conversion * 0.5f;

    const float sx = sinf(hx), cx = cosf(hx);
    const float sy = sinf(hy), cy = cosf(hy);
    const float sz = sinf(hz), cz = cosf(hz);

    return {
        cx * sy * sz + sx * cy * cz,
        cx * sy * cz - sx * cy * sz,
        cx * cy * sz + sx * sy * cz,
        cx * cy * cz - sx * sy * sz
    };
}

Quaternion Quaternion::between(const Vector3 &from, const Vector3 &to) {
    const Vector3 source = from.normalized();
    const Vector3 target = to.normalized();
    const float cosine = source.dot(target);

    if (cosine >= 1.0f - 1e-6f) return IDENTITY;

    if (cosine <= -1.0f + 1e-6f) {
        Vector3 axis = Vector3::RIGHT.cross(source);
        if (axis.length() < 1e-6f) axis = Vector3::UP.cross(source);
        return around(180.0f, axis.normalized());
    }

    const Vector3 axis = source.cross(target).normalized();
    const float angle  = acosf(cosine) * (180.0f / static_cast<float>(M_PI));
    return around(angle, axis);
}

Quaternion Quaternion::look(const Vector3 &forward, const Vector3 &up) {
    const Vector3 front = forward.normalized();
    const Vector3 right = up.cross(front).normalized();
    const Vector3 above = front.cross(right);

    if (const float trace = right.x + above.y + front.z; trace > 0.0f) {
        const float root = sqrtf(trace + 1.0f) * 2.0f;
        return {
            (above.z - front.y) / root,
            (front.x - right.z) / root,
            (right.y - above.x) / root,
            root * 0.25f
        };
    }

    if (right.x > above.y && right.x > front.z) {
        const float root = sqrtf(1.0f + right.x - above.y - front.z) * 2.0f;
        return { root * 0.25f, (right.y + above.x) / root, (front.x + right.z) / root, (above.z - front.y) / root };
    }

    if (above.y > front.z) {
        const float root = sqrtf(1.0f + above.y - right.x - front.z) * 2.0f;
        return { (right.y + above.x) / root, root * 0.25f, (above.z + front.y) / root, (front.x - right.z) / root };
    }

    const float root = sqrtf(1.0f + front.z - right.x - above.y) * 2.0f;
    return { (front.x + right.z) / root, (above.z + front.y) / root, root * 0.25f, (right.y - above.x) / root };
}

Quaternion Quaternion::around(const float angle, const Vector3 &axis) {
    const float radians = angle * (static_cast<float>(M_PI) / 180.0f) * 0.5f;
    const float sine    = sinf(radians);
    const Vector3 normalized = axis.normalized();
    return {normalized.x * sine, normalized.y * sine, normalized.z * sine, cosf(radians)};
}

std::ostream &operator<<(std::ostream &os, const Quaternion &quaternion) {
    os << "(" << quaternion.x << ", " << quaternion.y << ", " << quaternion.z << ", " << quaternion.w << ")";
    return os;
}