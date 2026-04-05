#include "../include/matrix4.hpp"
#include <cassert>

const Matrix4 Matrix4::IDENTITY = Matrix4(1.0f);

float Matrix4::operator()(const int row, const int column) const {
    assert(row >= 0 && row < 4 && "Matrix4: row index out of bounds");
    assert(column >= 0 && column < 4 && "Matrix4: column index out of bounds");
    return this->matrix[row][column];
}

float &Matrix4::operator()(const int row, const int column) {
    assert(row >= 0 && row < 4 && "Matrix4: row index out of bounds");
    assert(column >= 0 && column < 4 && "Matrix4: column index out of bounds");
    return this->matrix[row][column];
}

Matrix4 Matrix4::operator+(const Matrix4 &other) const {
    Matrix4 result;
    for (int row = 0; row < 4; row++)
        for (int column = 0; column < 4; column++)
            result.matrix[row][column] = this->matrix[row][column] + other.matrix[row][column];
    return result;
}

Matrix4 Matrix4::operator-(const Matrix4 &other) const {
    Matrix4 result;
    for (int row = 0; row < 4; row++)
        for (int column = 0; column < 4; column++)
            result.matrix[row][column] = this->matrix[row][column] - other.matrix[row][column];
    return result;
}

Matrix4 Matrix4::operator*(const Matrix4 &other) const {
    Matrix4 result;
    for (int row = 0; row < 4; row++)
        for (int column = 0; column < 4; column++)
            for (int k = 0; k < 4; k++)
                result.matrix[row][column] += this->matrix[row][k] * other.matrix[k][column];
    return result;
}

Matrix4 Matrix4::operator*(const float scalar) const {
    Matrix4 result;
    for (int row = 0; row < 4; row++)
        for (int column = 0; column < 4; column++)
            result.matrix[row][column] = this->matrix[row][column] * scalar;
    return result;
}

Vector3 Matrix4::operator*(const Vector3 &vector) const {
    const float x = this->matrix[0][0]*vector.x + this->matrix[0][1]*vector.y + this->matrix[0][2]*vector.z + this->matrix[0][3];
    const float y = this->matrix[1][0]*vector.x + this->matrix[1][1]*vector.y + this->matrix[1][2]*vector.z + this->matrix[1][3];
    const float z = this->matrix[2][0]*vector.x + this->matrix[2][1]*vector.y + this->matrix[2][2]*vector.z + this->matrix[2][3];
    const float w = this->matrix[3][0]*vector.x + this->matrix[3][1]*vector.y + this->matrix[3][2]*vector.z + this->matrix[3][3];
    assert(w != 0.0f && "Matrix4: perspective divide by zero (w = 0)");
    return {x / w, y / w, z / w};
}

Matrix4 &Matrix4::operator+=(const Matrix4 &other) { *this = *this + other; return *this; }
Matrix4 &Matrix4::operator-=(const Matrix4 &other) { *this = *this - other; return *this; }
Matrix4 &Matrix4::operator*=(const Matrix4 &other) { *this = *this * other; return *this; }
Matrix4 &Matrix4::operator*=(const float scalar)   { *this = *this * scalar; return *this; }

bool Matrix4::operator==(const Matrix4 &other) const {
    for (int row = 0; row < 4; row++)
        for (int column = 0; column < 4; column++)
            if (this->matrix[row][column] != other.matrix[row][column]) return false;
    return true;
}

bool Matrix4::operator!=(const Matrix4 &other) const { return !(*this == other); }

Matrix4 Matrix4::transposed() const {
    Matrix4 result;
    for (int row = 0; row < 4; row++)
        for (int column = 0; column < 4; column++)
            result.matrix[row][column] = this->matrix[column][row];
    return result;
}

Scalar Matrix4::determinant() const {
    float result = 0.0f;
    for (int i = 0; i < 4; i++) {
        float sub[3][3];
        for (int j = 1; j < 4; j++) {
            int column = 0;
            for (int k = 0; k < 4; k++) {
                if (k == i) continue;
                sub[j - 1][column++] = this->matrix[j][k];
            }
        }
        const float cofactor = sub[0][0] * (sub[1][1]*sub[2][2] - sub[1][2]*sub[2][1])
                             - sub[0][1] * (sub[1][0]*sub[2][2] - sub[1][2]*sub[2][0])
                             + sub[0][2] * (sub[1][0]*sub[2][1] - sub[1][1]*sub[2][0]);
        result += (i % 2 == 0 ? 1.0f : -1.0f) * this->matrix[0][i] * cofactor;
    }
    return Scalar(result);
}

Matrix4 Matrix4::inverted() const {
    const float determinant = static_cast<float>(this->determinant());
    assert(determinant != 0.0f && "Matrix4: cannot invert a singular matrix (determinant is zero)");

    float inverse[4][4];
    inverse[0][0] =  this->matrix[1][1]*this->matrix[2][2]*this->matrix[3][3] - this->matrix[1][1]*this->matrix[2][3]*this->matrix[3][2] - this->matrix[2][1]*this->matrix[1][2]*this->matrix[3][3] + this->matrix[2][1]*this->matrix[1][3]*this->matrix[3][2] + this->matrix[3][1]*this->matrix[1][2]*this->matrix[2][3] - this->matrix[3][1]*this->matrix[1][3]*this->matrix[2][2];
    inverse[1][0] = -this->matrix[1][0]*this->matrix[2][2]*this->matrix[3][3] + this->matrix[1][0]*this->matrix[2][3]*this->matrix[3][2] + this->matrix[2][0]*this->matrix[1][2]*this->matrix[3][3] - this->matrix[2][0]*this->matrix[1][3]*this->matrix[3][2] - this->matrix[3][0]*this->matrix[1][2]*this->matrix[2][3] + this->matrix[3][0]*this->matrix[1][3]*this->matrix[2][2];
    inverse[2][0] =  this->matrix[1][0]*this->matrix[2][1]*this->matrix[3][3] - this->matrix[1][0]*this->matrix[2][3]*this->matrix[3][1] - this->matrix[2][0]*this->matrix[1][1]*this->matrix[3][3] + this->matrix[2][0]*this->matrix[1][3]*this->matrix[3][1] + this->matrix[3][0]*this->matrix[1][1]*this->matrix[2][3] - this->matrix[3][0]*this->matrix[1][3]*this->matrix[2][1];
    inverse[3][0] = -this->matrix[1][0]*this->matrix[2][1]*this->matrix[3][2] + this->matrix[1][0]*this->matrix[2][2]*this->matrix[3][1] + this->matrix[2][0]*this->matrix[1][1]*this->matrix[3][2] - this->matrix[2][0]*this->matrix[1][2]*this->matrix[3][1] - this->matrix[3][0]*this->matrix[1][1]*this->matrix[2][2] + this->matrix[3][0]*this->matrix[1][2]*this->matrix[2][1];
    inverse[0][1] = -this->matrix[0][1]*this->matrix[2][2]*this->matrix[3][3] + this->matrix[0][1]*this->matrix[2][3]*this->matrix[3][2] + this->matrix[2][1]*this->matrix[0][2]*this->matrix[3][3] - this->matrix[2][1]*this->matrix[0][3]*this->matrix[3][2] - this->matrix[3][1]*this->matrix[0][2]*this->matrix[2][3] + this->matrix[3][1]*this->matrix[0][3]*this->matrix[2][2];
    inverse[1][1] =  this->matrix[0][0]*this->matrix[2][2]*this->matrix[3][3] - this->matrix[0][0]*this->matrix[2][3]*this->matrix[3][2] - this->matrix[2][0]*this->matrix[0][2]*this->matrix[3][3] + this->matrix[2][0]*this->matrix[0][3]*this->matrix[3][2] + this->matrix[3][0]*this->matrix[0][2]*this->matrix[2][3] - this->matrix[3][0]*this->matrix[0][3]*this->matrix[2][2];
    inverse[2][1] = -this->matrix[0][0]*this->matrix[2][1]*this->matrix[3][3] + this->matrix[0][0]*this->matrix[2][3]*this->matrix[3][1] + this->matrix[2][0]*this->matrix[0][1]*this->matrix[3][3] - this->matrix[2][0]*this->matrix[0][3]*this->matrix[3][1] - this->matrix[3][0]*this->matrix[0][1]*this->matrix[2][3] + this->matrix[3][0]*this->matrix[0][3]*this->matrix[2][1];
    inverse[3][1] =  this->matrix[0][0]*this->matrix[2][1]*this->matrix[3][2] - this->matrix[0][0]*this->matrix[2][2]*this->matrix[3][1] - this->matrix[2][0]*this->matrix[0][1]*this->matrix[3][2] + this->matrix[2][0]*this->matrix[0][2]*this->matrix[3][1] + this->matrix[3][0]*this->matrix[0][1]*this->matrix[2][2] - this->matrix[3][0]*this->matrix[0][2]*this->matrix[2][1];
    inverse[0][2] =  this->matrix[0][1]*this->matrix[1][2]*this->matrix[3][3] - this->matrix[0][1]*this->matrix[1][3]*this->matrix[3][2] - this->matrix[1][1]*this->matrix[0][2]*this->matrix[3][3] + this->matrix[1][1]*this->matrix[0][3]*this->matrix[3][2] + this->matrix[3][1]*this->matrix[0][2]*this->matrix[1][3] - this->matrix[3][1]*this->matrix[0][3]*this->matrix[1][2];
    inverse[1][2] = -this->matrix[0][0]*this->matrix[1][2]*this->matrix[3][3] + this->matrix[0][0]*this->matrix[1][3]*this->matrix[3][2] + this->matrix[1][0]*this->matrix[0][2]*this->matrix[3][3] - this->matrix[1][0]*this->matrix[0][3]*this->matrix[3][2] - this->matrix[3][0]*this->matrix[0][2]*this->matrix[1][3] + this->matrix[3][0]*this->matrix[0][3]*this->matrix[1][2];
    inverse[2][2] =  this->matrix[0][0]*this->matrix[1][1]*this->matrix[3][3] - this->matrix[0][0]*this->matrix[1][3]*this->matrix[3][1] - this->matrix[1][0]*this->matrix[0][1]*this->matrix[3][3] + this->matrix[1][0]*this->matrix[0][3]*this->matrix[3][1] + this->matrix[3][0]*this->matrix[0][1]*this->matrix[1][3] - this->matrix[3][0]*this->matrix[0][3]*this->matrix[1][1];
    inverse[3][2] = -this->matrix[0][0]*this->matrix[1][1]*this->matrix[3][2] + this->matrix[0][0]*this->matrix[1][2]*this->matrix[3][1] + this->matrix[1][0]*this->matrix[0][1]*this->matrix[3][2] - this->matrix[1][0]*this->matrix[0][2]*this->matrix[3][1] - this->matrix[3][0]*this->matrix[0][1]*this->matrix[1][2] + this->matrix[3][0]*this->matrix[0][2]*this->matrix[1][1];
    inverse[0][3] = -this->matrix[0][1]*this->matrix[1][2]*this->matrix[2][3] + this->matrix[0][1]*this->matrix[1][3]*this->matrix[2][2] + this->matrix[1][1]*this->matrix[0][2]*this->matrix[2][3] - this->matrix[1][1]*this->matrix[0][3]*this->matrix[2][2] - this->matrix[2][1]*this->matrix[0][2]*this->matrix[1][3] + this->matrix[2][1]*this->matrix[0][3]*this->matrix[1][2];
    inverse[1][3] =  this->matrix[0][0]*this->matrix[1][2]*this->matrix[2][3] - this->matrix[0][0]*this->matrix[1][3]*this->matrix[2][2] - this->matrix[1][0]*this->matrix[0][2]*this->matrix[2][3] + this->matrix[1][0]*this->matrix[0][3]*this->matrix[2][2] + this->matrix[2][0]*this->matrix[0][2]*this->matrix[1][3] - this->matrix[2][0]*this->matrix[0][3]*this->matrix[1][2];
    inverse[2][3] = -this->matrix[0][0]*this->matrix[1][1]*this->matrix[2][3] + this->matrix[0][0]*this->matrix[1][3]*this->matrix[2][1] + this->matrix[1][0]*this->matrix[0][1]*this->matrix[2][3] - this->matrix[1][0]*this->matrix[0][3]*this->matrix[2][1] - this->matrix[2][0]*this->matrix[0][1]*this->matrix[1][3] + this->matrix[2][0]*this->matrix[0][3]*this->matrix[1][1];
    inverse[3][3] =  this->matrix[0][0]*this->matrix[1][1]*this->matrix[2][2] - this->matrix[0][0]*this->matrix[1][2]*this->matrix[2][1] - this->matrix[1][0]*this->matrix[0][1]*this->matrix[2][2] + this->matrix[1][0]*this->matrix[0][2]*this->matrix[2][1] + this->matrix[2][0]*this->matrix[0][1]*this->matrix[1][2] - this->matrix[2][0]*this->matrix[0][2]*this->matrix[1][1];

    Matrix4 result;
    for (int row = 0; row < 4; row++)
        for (int column = 0; column < 4; column++)
            result.matrix[row][column] = inverse[row][column] / determinant;
    return result;
}

bool Matrix4::approximately(const Matrix4 &other, const float epsilon) const {
    for (int row = 0; row < 4; row++)
        for (int column = 0; column < 4; column++)
            if (fabsf(this->matrix[row][column] - other.matrix[row][column]) > epsilon) return false;
    return true;
}

Vector3 Matrix4::translation() const {
    return {this->matrix[0][3], this->matrix[1][3], this->matrix[2][3]};
}

Quaternion Matrix4::rotation() const {
    const Vector3 scale = this->scale();
    assert(scale.x != 0.0f && scale.y != 0.0f && scale.z != 0.0f && "Matrix4::rotation: degenerate matrix (zero scale component)");

    const float r00 = this->matrix[0][0] / scale.x, r01 = this->matrix[0][1] / scale.y, r02 = this->matrix[0][2] / scale.z;
    const float r10 = this->matrix[1][0] / scale.x, r11 = this->matrix[1][1] / scale.y, r12 = this->matrix[1][2] / scale.z;
    const float r20 = this->matrix[2][0] / scale.x, r21 = this->matrix[2][1] / scale.y, r22 = this->matrix[2][2] / scale.z;
    const float trace = r00 + r11 + r22;

    float qw, qx, qy, qz;
    if (trace > 0.0f) {
        const float s = 0.5f / sqrtf(trace + 1.0f);
        qw = 0.25f / s;
        qx = (r21 - r12) * s;
        qy = (r02 - r20) * s;
        qz = (r10 - r01) * s;
    } else if (r00 > r11 && r00 > r22) {
        const float s = 2.0f * sqrtf(1.0f + r00 - r11 - r22);
        qw = (r21 - r12) / s;
        qx = 0.25f * s;
        qy = (r01 + r10) / s;
        qz = (r02 + r20) / s;
    } else if (r11 > r22) {
        const float s = 2.0f * sqrtf(1.0f + r11 - r00 - r22);
        qw = (r02 - r20) / s;
        qx = (r01 + r10) / s;
        qy = 0.25f * s;
        qz = (r12 + r21) / s;
    } else {
        const float s = 2.0f * sqrtf(1.0f + r22 - r00 - r11);
        qw = (r10 - r01) / s;
        qx = (r02 + r20) / s;
        qy = (r12 + r21) / s;
        qz = 0.25f * s;
    }
    return {qx, qy, qz, qw};
}

Vector3 Matrix4::scale() const {
    return {
        sqrtf(this->matrix[0][0]*this->matrix[0][0] + this->matrix[1][0]*this->matrix[1][0] + this->matrix[2][0]*this->matrix[2][0]),
        sqrtf(this->matrix[0][1]*this->matrix[0][1] + this->matrix[1][1]*this->matrix[1][1] + this->matrix[2][1]*this->matrix[2][1]),
        sqrtf(this->matrix[0][2]*this->matrix[0][2] + this->matrix[1][2]*this->matrix[1][2] + this->matrix[2][2]*this->matrix[2][2])
    };
}

Vector3 Matrix4::anterior()  const { return {-this->matrix[0][2], -this->matrix[1][2], -this->matrix[2][2]}; }
Vector3 Matrix4::posterior() const { return { this->matrix[0][2],  this->matrix[1][2],  this->matrix[2][2]}; }
Vector3 Matrix4::superior()  const { return { this->matrix[0][1],  this->matrix[1][1],  this->matrix[2][1]}; }
Vector3 Matrix4::inferior()  const { return {-this->matrix[0][1], -this->matrix[1][1], -this->matrix[2][1]}; }
Vector3 Matrix4::dexter()    const { return { this->matrix[0][0],  this->matrix[1][0],  this->matrix[2][0]}; }
Vector3 Matrix4::sinister()  const { return {-this->matrix[0][0], -this->matrix[1][0], -this->matrix[2][0]}; }

bool Matrix4::decompose(Vector3 &translation, Quaternion &rotation, Vector3 &scale) const {
    assert(static_cast<float>(this->determinant()) != 0.0f && "Matrix4::decompose: cannot decompose a singular matrix");
    translation = this->translation();
    scale = this->scale();
    rotation = this->rotation();
    return true;
}

Matrix4 Matrix4::identity() { return Matrix4(1.0f); }

Matrix4 Matrix4::translate(const Vector3 &translation) {
    Matrix4 result = identity();
    result.matrix[0][3] = translation.x;
    result.matrix[1][3] = translation.y;
    result.matrix[2][3] = translation.z;
    return result;
}

Matrix4 Matrix4::rotate(const Quaternion &rotation) {
    assert(fabsf(rotation.x*rotation.x + rotation.y*rotation.y + rotation.z*rotation.z + rotation.w*rotation.w - 1.0f) < 1e-4f && "Matrix4::rotate: quaternion must be normalized");
    Matrix4 result = identity();
    result.matrix[0][0] = 1.0f - 2.0f*rotation.y*rotation.y - 2.0f*rotation.z*rotation.z;
    result.matrix[0][1] = 2.0f*rotation.x*rotation.y - 2.0f*rotation.z*rotation.w;
    result.matrix[0][2] = 2.0f*rotation.x*rotation.z + 2.0f*rotation.y*rotation.w;
    result.matrix[1][0] = 2.0f*rotation.x*rotation.y + 2.0f*rotation.z*rotation.w;
    result.matrix[1][1] = 1.0f - 2.0f*rotation.x*rotation.x - 2.0f*rotation.z*rotation.z;
    result.matrix[1][2] = 2.0f*rotation.y*rotation.z - 2.0f*rotation.x*rotation.w;
    result.matrix[2][0] = 2.0f*rotation.x*rotation.z - 2.0f*rotation.y*rotation.w;
    result.matrix[2][1] = 2.0f*rotation.y*rotation.z + 2.0f*rotation.x*rotation.w;
    result.matrix[2][2] = 1.0f - 2.0f*rotation.x*rotation.x - 2.0f*rotation.y*rotation.y;
    return result;
}

Matrix4 Matrix4::scale(const Vector3 &scale) {
    Matrix4 result = identity();
    result.matrix[0][0] = scale.x;
    result.matrix[1][1] = scale.y;
    result.matrix[2][2] = scale.z;
    return result;
}

Matrix4 Matrix4::compose(const Vector3 &translation, const Quaternion &rotation, const Vector3 &scale) {
    return translate(translation) * rotate(rotation) * Matrix4::scale(scale);
}

Matrix4 Matrix4::perspective(const float fov, const float aspect, const float zNear, const float zFar) {
    assert(fov > 0.0f && "Matrix4::perspective: fov must be positive");
    assert(aspect != 0.0f && "Matrix4::perspective: aspect ratio cannot be zero");
    assert(zNear > 0.0f && "Matrix4::perspective: zNear must be positive");
    assert(zNear != zFar && "Matrix4::perspective: zNear and zFar cannot be equal");
    const float tangent = tanf(fov * 0.5f);
    assert(tangent != 0.0f && "Matrix4::perspective: fov produces zero tangent");
    Matrix4 result;
    result.matrix[0][0] = 1.0f / (aspect * tangent);
    result.matrix[1][1] = 1.0f / tangent;
    result.matrix[2][2] = -(zFar + zNear) / (zFar - zNear);
    result.matrix[2][3] = -(2.0f * zFar * zNear) / (zFar - zNear);
    result.matrix[3][2] = -1.0f;
    return result;
}

Matrix4 Matrix4::orthographic(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar) {
    assert(right != left && "Matrix4::orthographic: left and right cannot be equal");
    assert(top != bottom && "Matrix4::orthographic: top and bottom cannot be equal");
    assert(zNear != zFar && "Matrix4::orthographic: zNear and zFar cannot be equal");
    Matrix4 result = identity();
    result.matrix[0][0] = 2.0f / (right - left);
    result.matrix[1][1] = 2.0f / (top - bottom);
    result.matrix[2][2] = -2.0f / (zFar - zNear);
    result.matrix[0][3] = -(right + left) / (right - left);
    result.matrix[1][3] = -(top + bottom) / (top - bottom);
    result.matrix[2][3] = -(zFar + zNear) / (zFar - zNear);
    return result;
}

Matrix4 Matrix4::look(const Vector3 &eye, const Vector3 &target, const Vector3 &up) {
    assert(eye != target && "Matrix4::look: eye and target cannot be the same point");
    assert(static_cast<float>(up.length()) > 0.0f && "Matrix4::look: up vector must be non-zero");
    const Vector3 forward = (target - eye).normalized();
    const Vector3 right = forward.cross(up).normalized();
    const Vector3 posterior = right.cross(forward);
    Matrix4 result = identity();
    result.matrix[0][0] = right.x;
    result.matrix[0][1] = right.y;
    result.matrix[0][2] = right.z;
    result.matrix[1][0] = posterior.x;
    result.matrix[1][1] = posterior.y;
    result.matrix[1][2] = posterior.z;
    result.matrix[2][0] = -forward.x;
    result.matrix[2][1] = -forward.y;
    result.matrix[2][2] = -forward.z;
    result.matrix[0][3] = -static_cast<float>(right.dot(eye));
    result.matrix[1][3] = -static_cast<float>(posterior.dot(eye));
    result.matrix[2][3] = static_cast<float>(forward.dot(eye));
    return result;
}

std::ostream &operator<<(std::ostream &os, const Matrix4 &other) {
    for (const auto row : other.matrix)
        os << "[ " << row[0] << ", " << row[1] << ", " << row[2] << ", " << row[3] << " ]\n";
    return os;
}