#include "../include/matrix3.hpp"
#include <cassert>
#include <numbers>

const Matrix3 Matrix3::IDENTITY = Matrix3(1.0f);

float Matrix3::operator()(const int row, const int column) const {
    assert(row >= 0 && row < 3 && "Matrix3: row index out of bounds");
    assert(column >= 0 && column < 3 && "Matrix3: column index out of bounds");
    return this->matrix[row][column];
}

float &Matrix3::operator()(const int row, const int column) {
    assert(row >= 0 && row < 3 && "Matrix3: row index out of bounds");
    assert(column >= 0 && column < 3 && "Matrix3: column index out of bounds");
    return this->matrix[row][column];
}

Matrix3 Matrix3::operator+(const Matrix3 &other) const {
    Matrix3 result;
    for (int row = 0; row < 3; row++)
        for (int column = 0; column < 3; column++)
            result.matrix[row][column] = this->matrix[row][column] + other.matrix[row][column];
    return result;
}

Matrix3 Matrix3::operator-(const Matrix3 &other) const {
    Matrix3 result;
    for (int row = 0; row < 3; row++)
        for (int column = 0; column < 3; column++)
            result.matrix[row][column] = this->matrix[row][column] - other.matrix[row][column];
    return result;
}

Matrix3 Matrix3::operator*(const Matrix3 &other) const {
    Matrix3 result;
    for (int row = 0; row < 3; row++)
        for (int column = 0; column < 3; column++)
            for (int index = 0; index < 3; index++)
                result.matrix[row][column] += this->matrix[row][index] * other.matrix[index][column];
    return result;
}

Matrix3 Matrix3::operator*(const float scalar) const {
    Matrix3 result;
    for (int row = 0; row < 3; row++)
        for (int column = 0; column < 3; column++)
            result.matrix[row][column] = this->matrix[row][column] * scalar;
    return result;
}

Vector2 Matrix3::operator*(const Vector2 &vector) const {
    const float x = this->matrix[0][0]*vector.x + this->matrix[0][1]*vector.y + this->matrix[0][2];
    const float y = this->matrix[1][0]*vector.x + this->matrix[1][1]*vector.y + this->matrix[1][2];
    return {x, y};
}

Matrix3 &Matrix3::operator+=(const Matrix3 &other) { *this = *this + other; return *this; }
Matrix3 &Matrix3::operator-=(const Matrix3 &other) { *this = *this - other; return *this; }
Matrix3 &Matrix3::operator*=(const Matrix3 &other) { *this = *this * other; return *this; }
Matrix3 &Matrix3::operator*=(const float scalar)   { *this = *this * scalar; return *this; }

bool Matrix3::operator==(const Matrix3 &other) const {
    for (int row = 0; row < 3; row++)
        for (int column = 0; column < 3; column++)
            if (this->matrix[row][column] != other.matrix[row][column]) return false;
    return true;
}

bool Matrix3::operator!=(const Matrix3 &other) const { return !(*this == other); }

Matrix3 Matrix3::transposed() const {
    Matrix3 result;
    for (int row = 0; row < 3; row++)
        for (int column = 0; column < 3; column++)
            result.matrix[row][column] = this->matrix[column][row];
    return result;
}

Scalar Matrix3::determinant() const {
    return Scalar(
        this->matrix[0][0] * (this->matrix[1][1]*this->matrix[2][2] - this->matrix[1][2]*this->matrix[2][1]) -
        this->matrix[0][1] * (this->matrix[1][0]*this->matrix[2][2] - this->matrix[1][2]*this->matrix[2][0]) +
        this->matrix[0][2] * (this->matrix[1][0]*this->matrix[2][1] - this->matrix[1][1]*this->matrix[2][0])
    );
}

Matrix3 Matrix3::inverted() const {
    const float determinant = static_cast<float>(this->determinant());
    assert(determinant != 0.0f && "Matrix3: cannot invert a singular matrix (determinant is zero)");
    const float reciprocal = 1.0f / determinant;
    Matrix3 result;
    result.matrix[0][0] =  (this->matrix[1][1]*this->matrix[2][2] - this->matrix[1][2]*this->matrix[2][1]) * reciprocal;
    result.matrix[0][1] = -(this->matrix[0][1]*this->matrix[2][2] - this->matrix[0][2]*this->matrix[2][1]) * reciprocal;
    result.matrix[0][2] =  (this->matrix[0][1]*this->matrix[1][2] - this->matrix[0][2]*this->matrix[1][1]) * reciprocal;
    result.matrix[1][0] = -(this->matrix[1][0]*this->matrix[2][2] - this->matrix[1][2]*this->matrix[2][0]) * reciprocal;
    result.matrix[1][1] =  (this->matrix[0][0]*this->matrix[2][2] - this->matrix[0][2]*this->matrix[2][0]) * reciprocal;
    result.matrix[1][2] = -(this->matrix[0][0]*this->matrix[1][2] - this->matrix[0][2]*this->matrix[1][0]) * reciprocal;
    result.matrix[2][0] =  (this->matrix[1][0]*this->matrix[2][1] - this->matrix[1][1]*this->matrix[2][0]) * reciprocal;
    result.matrix[2][1] = -(this->matrix[0][0]*this->matrix[2][1] - this->matrix[0][1]*this->matrix[2][0]) * reciprocal;
    result.matrix[2][2] =  (this->matrix[0][0]*this->matrix[1][1] - this->matrix[0][1]*this->matrix[1][0]) * reciprocal;
    return result;
}

bool Matrix3::approximately(const Matrix3 &other, const float epsilon) const {
    for (int row = 0; row < 3; row++)
        for (int column = 0; column < 3; column++)
            if (fabsf(this->matrix[row][column] - other.matrix[row][column]) > epsilon) return false;
    return true;
}

Vector2 Matrix3::translation() const {
    return {this->matrix[0][2], this->matrix[1][2]};
}

Scalar Matrix3::rotation() const {
    return Scalar(atan2f(this->matrix[1][0], this->matrix[0][0]) * (180.0f / std::numbers::pi_v<float>));
}

Vector2 Matrix3::scale() const {
    return {
        sqrtf(this->matrix[0][0]*this->matrix[0][0] + this->matrix[1][0]*this->matrix[1][0]),
        sqrtf(this->matrix[0][1]*this->matrix[0][1] + this->matrix[1][1]*this->matrix[1][1])
    };
}

Vector2 Matrix3::superior() const { return {this->matrix[0][0], this->matrix[0][1]}; }
Vector2 Matrix3::inferior() const { return {this->matrix[1][0], this->matrix[1][1]}; }
Vector2 Matrix3::dexter()   const { return {this->matrix[0][0], this->matrix[1][0]}; }
Vector2 Matrix3::sinister() const { return {this->matrix[0][1], this->matrix[1][1]}; }

bool Matrix3::decompose(Vector2 &translation, float &rotation, Vector2 &scale) const {
    assert(static_cast<float>(this->determinant()) != 0.0f && "Matrix3::decompose: cannot decompose a singular matrix");
    translation = this->translation();
    const float horizontal = sqrtf(this->matrix[0][0]*this->matrix[0][0] + this->matrix[1][0]*this->matrix[1][0]);
    const float vertical   = sqrtf(this->matrix[0][1]*this->matrix[0][1] + this->matrix[1][1]*this->matrix[1][1]);
    if (horizontal == 0.0f || vertical == 0.0f) return false;
    scale    = {horizontal, vertical};
    rotation = atan2f(this->matrix[1][0] / horizontal, this->matrix[0][0] / horizontal) * (180.0f / std::numbers::pi_v<float>);
    return true;
}

Matrix3 Matrix3::identity() { return Matrix3(1.0f); }

Matrix3 Matrix3::translate(const Vector2 &translation) {
    Matrix3 result = identity();
    result.matrix[0][2] = translation.x;
    result.matrix[1][2] = translation.y;
    return result;
}

Matrix3 Matrix3::rotate(const float angle) {
    const float radians = angle * (std::numbers::pi_v<float> / 180.0f);
    const float cosine  = cosf(radians);
    const float sine    = sinf(radians);
    Matrix3 result = identity();
    result.matrix[0][0] =  cosine;
    result.matrix[0][1] = -sine;
    result.matrix[1][0] =  sine;
    result.matrix[1][1] =  cosine;
    return result;
}

Matrix3 Matrix3::scale(const Vector2 &factors) {
    Matrix3 result = identity();
    result.matrix[0][0] = factors.x;
    result.matrix[1][1] = factors.y;
    return result;
}

Matrix3 Matrix3::compose(const Vector2 &translation, const float rotation, const Vector2 &scale) {
    return translate(translation) * rotate(rotation) * Matrix3::scale(scale);
}

Matrix3 Matrix3::orthographic(const float left, const float right, const float bottom, const float top) {
    assert(right != left && "Matrix3::orthographic: left and right cannot be equal");
    assert(top != bottom && "Matrix3::orthographic: top and bottom cannot be equal");
    Matrix3 result = identity();
    result.matrix[0][0] =  2.0f / (right - left);
    result.matrix[1][1] =  2.0f / (top - bottom);
    result.matrix[0][2] = -(right + left) / (right - left);
    result.matrix[1][2] = -(top + bottom) / (top - bottom);
    return result;
}

std::ostream &operator<<(std::ostream &os, const Matrix3 &other) {
    for (const auto row : other.matrix) {
        os << "[ ";
        for (int column = 0; column < 3; column++)
            os << row[column] << (column < 2 ? ", " : " ");
        os << "]\n";
    }
    return os;
}