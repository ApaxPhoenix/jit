#ifndef COLOR3_HPP
#define COLOR3_HPP

#include "scalar.hpp"
#include <limits>
#include <ostream>

struct Color3 {
    float red, green, blue;

    Color3();
    Color3(float red, float green, float blue);

    Color3 operator+(const Color3 &color) const;
    Color3 &operator+=(const Color3 &color);

    Color3 operator-(const Color3 &color) const;
    Color3 &operator-=(const Color3 &color);

    Color3 operator*(float scalar) const;
    Color3 &operator*=(float scalar);

    Color3 operator*(const Color3 &color) const;
    Color3 &operator*=(const Color3 &color);

    Color3 operator/(float scalar) const;
    Color3 &operator/=(float scalar);

    Color3 operator/(const Color3 &color) const;
    Color3 &operator/=(const Color3 &color);

    bool operator==(const Color3 &color) const;
    bool operator!=(const Color3 &color) const;

    [[nodiscard]] Scalar luminance() const;
    [[nodiscard]] Scalar difference(const Color3 &color) const;

    [[nodiscard]] Color3 inverted() const;
    [[nodiscard]] Color3 clamped() const;
    [[nodiscard]] Color3 interpolate(const Color3 &color, float factor) const;
    [[nodiscard]] Color3 gamma(float exponent) const;

    [[nodiscard]] bool approximately(const Color3 &color, float epsilon = std::numeric_limits<float>::epsilon()) const;

    static Color3 rgb(uint8_t red, uint8_t green, uint8_t blue);
    static Color3 hsv(float hue, float saturation, float value);
    static Color3 hex(unsigned int hex);

    static const Color3 WHITE;
    static const Color3 BLACK;
    static const Color3 RED;
    static const Color3 GREEN;
    static const Color3 BLUE;
    static const Color3 YELLOW;
    static const Color3 CYAN;
    static const Color3 MAGENTA;

    friend std::ostream &operator<<(std::ostream &os, const Color3 &color);
};

inline Color3 operator*(float scalar, const Color3 &color) {
    return color * scalar;
}

#endif