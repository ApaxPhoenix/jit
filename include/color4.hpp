#ifndef COLOR4_HPP
#define COLOR4_HPP

#include "color3.hpp"
#include "scalar.hpp"
#include <limits>
#include <ostream>

struct Color4 {
    float red, green, blue, alpha;

    Color4();
    Color4(float red, float green, float blue, float alpha = 1.0f);
    explicit Color4(const Color3 &color, float alpha = 1.0f);

    Color4 operator+(const Color4 &color) const;
    Color4 &operator+=(const Color4 &color);

    Color4 operator-(const Color4 &color) const;
    Color4 &operator-=(const Color4 &color);

    Color4 operator*(float scalar) const;
    Color4 &operator*=(float scalar);

    Color4 operator*(const Color4 &color) const;
    Color4 &operator*=(const Color4 &color);

    Color4 operator/(float scalar) const;
    Color4 &operator/=(float scalar);

    Color4 operator/(const Color4 &color) const;
    Color4 &operator/=(const Color4 &color);

    bool operator==(const Color4 &color) const;
    bool operator!=(const Color4 &color) const;

    [[nodiscard]] Scalar luminance() const;
    [[nodiscard]] Scalar difference(const Color4 &color) const;

    [[nodiscard]] Color4 inverted() const;
    [[nodiscard]] Color4 clamped() const;
    [[nodiscard]] Color4 interpolate(const Color4 &color, float factor) const;
    [[nodiscard]] Color4 gamma(float exponent) const;
    [[nodiscard]] Color4 premultiplied() const;

    [[nodiscard]] bool approximately(const Color4 &color, float epsilon = std::numeric_limits<float>::epsilon()) const;

    static Color4 rgb(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
    static Color4 hsv(float hue, float saturation, float value, float alpha = 1.0f);
    static Color4 hex(unsigned int hex);

    static const Color4 WHITE;
    static const Color4 BLACK;
    static const Color4 RED;
    static const Color4 GREEN;
    static const Color4 BLUE;
    static const Color4 YELLOW;
    static const Color4 CYAN;
    static const Color4 MAGENTA;
    static const Color4 TRANSPARENT;

    friend std::ostream &operator<<(std::ostream &os, const Color4 &color);
};

inline Color4 operator*(float scalar, const Color4 &color) {
    return color * scalar;
}

#endif