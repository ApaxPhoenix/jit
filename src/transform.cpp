#include "../include/transform.hpp"
#include <cassert>

const Transform Transform::IDENTITY = Transform(
    Vector3(0.0f, 0.0f, 0.0f),
    Quaternion(0.0f, 0.0f, 0.0f, 1.0f),
    Vector3(1.0f, 1.0f, 1.0f)
);

Transform::Transform()
    : translation(0.0f, 0.0f, 0.0f)
    , rotation(0.0f, 0.0f, 0.0f, 1.0f)
    , scale(1.0f, 1.0f, 1.0f) {}

Transform::Transform(const Vector3 &translation, const Quaternion &rotation, const Vector3 &scale)
    : translation(translation)
    , rotation(rotation)
    , scale(scale) {}

Transform Transform::operator*(const Transform &transform) const {
    return {
        this->translation + this->rotation * (this->scale * transform.translation),
        (this->rotation * transform.rotation).normalized(),
        Vector3(this->scale.x * transform.scale.x, this->scale.y * transform.scale.y, this->scale.z * transform.scale.z)
    };
}

Transform &Transform::operator*=(const Transform &transform) { *this = *this * transform; return *this; }

bool Transform::operator==(const Transform &transform) const {
    return this->translation == transform.translation &&
           this->rotation    == transform.rotation &&
           this->scale       == transform.scale;
}

bool Transform::operator!=(const Transform &transform) const { return !(*this == transform); }

Matrix4 Transform::matrix() const {
    return Matrix4::compose(this->translation, this->rotation, this->scale);
}

Vector3 Transform::forward() const { return this->rotation * Vector3::FORWARD; }
Vector3 Transform::right()   const { return this->rotation * Vector3::RIGHT; }
Vector3 Transform::up()      const { return this->rotation * Vector3::UP; }

Transform Transform::inversed() const {
    assert(this->scale.x != 0.0f && this->scale.y != 0.0f && this->scale.z != 0.0f && "Transform::inversed: scale cannot be zero");
    const Quaternion inverted = this->rotation.inverse();
    const Vector3 reciprocal = Vector3(1.0f / this->scale.x, 1.0f / this->scale.y, 1.0f / this->scale.z);
    const Vector3 position   = inverted * (reciprocal * -this->translation);
    return {position, inverted, reciprocal};
}

Transform Transform::interpolate(const Transform &transform, float factor) const {
    factor = fmaxf(0.0f, fminf(factor, 1.0f));
    return {
        this->translation.interpolate(transform.translation, factor),
        this->rotation.interpolate(transform.rotation, factor),
        this->scale.interpolate(transform.scale, factor)
    };
}

Vector3 Transform::point(const Vector3 &point) const {
    return this->translation + this->rotation * (this->scale * point);
}

Vector3 Transform::direction(const Vector3 &direction) const {
    return this->rotation * direction;
}

bool Transform::approximately(const Transform &transform, const float epsilon) const {
    return this->translation.approximately(transform.translation, epsilon) &&
           this->rotation.approximately(transform.rotation, epsilon) &&
           this->scale.approximately(transform.scale, epsilon);
}

Transform Transform::from(const Matrix4 &matrix) {
    Vector3 translation;
    Quaternion rotation;
    Vector3 scale;
    const bool healthy = matrix.decompose(translation, rotation, scale);
    assert(healthy && "Transform::from: matrix decomposition failed (singular or degenerate matrix)");
    return {translation, rotation, scale};
}

std::ostream &operator<<(std::ostream &os, const Transform &transform) {
    os << "Transform(translation=" << transform.translation
       << ", rotation="            << transform.rotation
       << ", scale="               << transform.scale << ")";
    return os;
}