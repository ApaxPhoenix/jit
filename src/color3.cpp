#include "../include/color3.hpp"
#include <algorithm>
#include <cassert>

const Color3 Color3::WHITE   = Color3(1.0f, 1.0f, 1.0f);
const Color3 Color3::BLACK   = Color3(0.0f, 0.0f, 0.0f);
const Color3 Color3::RED     = Color3(1.0f, 0.0f, 0.0f);
const Color3 Color3::GREEN   = Color3(0.0f, 1.0f, 0.0f);
const Color3 Color3::BLUE    = Color3(0.0f, 0.0f, 1.0f);
const Color3 Color3::YELLOW  = Color3(1.0f, 1.0f, 0.0f);
const Color3 Color3::CYAN    = Color3(0.0f, 1.0f, 1.0f);
const Color3 Color3::MAGENTA = Color3(1.0f, 0.0f, 1.0f);

Color3::Color3() : red(0.0f), green(0.0f), blue(0.0f) {}

Color3::Color3(const float red, const float green, const float blue)
    : red(red), green(green), blue(blue) {}

Color3 Color3::operator+(const Color3 &color) const {
    return {this->red + color.red, this->green + color.green, this->blue + color.blue};
}

Color3 &Color3::operator+=(const Color3 &color) {
    this->red += color.red; this->green += color.green; this->blue += color.blue;
    return *this;
}

Color3 Color3::operator-(const Color3 &color) const {
    return {this->red - color.red, this->green - color.green, this->blue - color.blue};
}

Color3 &Color3::operator-=(const Color3 &color) {
    this->red -= color.red; this->green -= color.green; this->blue -= color.blue;
    return *this;
}

Color3 Color3::operator*(const float scalar) const {
    return {this->red * scalar, this->green * scalar, this->blue * scalar};
}

Color3 &Color3::operator*=(const float scalar) {
    this->red *= scalar; this->green *= scalar; this->blue *= scalar;
    return *this;
}

Color3 Color3::operator*(const Color3 &color) const {
    return {this->red * color.red, this->green * color.green, this->blue * color.blue};
}

Color3 &Color3::operator*=(const Color3 &color) {
    this->red *= color.red; this->green *= color.green; this->blue *= color.blue;
    return *this;
}

Color3 Color3::operator/(const float scalar) const {
    assert(scalar != 0.0f && "Color3: division by zero scalar");
    return {this->red / scalar, this->green / scalar, this->blue / scalar};
}

Color3 &Color3::operator/=(const float scalar) {
    assert(scalar != 0.0f && "Color3: division by zero scalar");
    this->red /= scalar; this->green /= scalar; this->blue /= scalar;
    return *this;
}

Color3 Color3::operator/(const Color3 &color) const {
    assert(color.red   != 0.0f && "Color3: division by zero (red channel)");
    assert(color.green != 0.0f && "Color3: division by zero (green channel)");
    assert(color.blue  != 0.0f && "Color3: division by zero (blue channel)");
    return {this->red / color.red, this->green / color.green, this->blue / color.blue};
}

Color3 &Color3::operator/=(const Color3 &color) {
    assert(color.red   != 0.0f && "Color3: division by zero (red channel)");
    assert(color.green != 0.0f && "Color3: division by zero (green channel)");
    assert(color.blue  != 0.0f && "Color3: division by zero (blue channel)");
    this->red /= color.red; this->green /= color.green; this->blue /= color.blue;
    return *this;
}

bool Color3::operator==(const Color3 &color) const {
    return this->red == color.red && this->green == color.green && this->blue == color.blue;
}

bool Color3::operator!=(const Color3 &color) const { return !(*this == color); }

Scalar Color3::luminance() const {
    return static_cast<Scalar>(0.2126f * this->red + 0.7152f * this->green + 0.0722f * this->blue);
}

Scalar Color3::difference(const Color3 &color) const {
    const float dr = this->red   - color.red;
    const float dg = this->green - color.green;
    const float db = this->blue  - color.blue;
    return static_cast<Scalar>(std::sqrt(dr*dr + dg*dg + db*db));
}

Color3 Color3::inverted() const {
    return {1.0f - this->red, 1.0f - this->green, 1.0f - this->blue};
}

Color3 Color3::clamped() const {
    return {
        std::clamp(this->red,   0.0f, 1.0f),
        std::clamp(this->green, 0.0f, 1.0f),
        std::clamp(this->blue,  0.0f, 1.0f)
    };
}

Color3 Color3::interpolate(const Color3 &color, float factor) const {
    factor = std::clamp(factor, 0.0f, 1.0f);
    return {
        this->red   + (color.red   - this->red)   * factor,
        this->green + (color.green - this->green)  * factor,
        this->blue  + (color.blue  - this->blue)   * factor
    };
}

Color3 Color3::gamma(const float exponent) const {
    assert(exponent > 0.0f && "Color3: gamma exponent must be positive");
    const Color3 safe = this->clamped();
    return {
        std::pow(safe.red,   exponent),
        std::pow(safe.green, exponent),
        std::pow(safe.blue,  exponent)
    };
}

bool Color3::approximately(const Color3 &color, const float epsilon) const {
    return std::abs(this->red   - color.red)   <= epsilon &&
           std::abs(this->green - color.green)  <= epsilon &&
           std::abs(this->blue  - color.blue)   <= epsilon;
}

Color3 Color3::hsv(float hue, float saturation, float value) {
    assert(hue >= 0.0f && hue < 360.0f             && "Color3::hsv: hue must be in [0, 360)");
    assert(saturation >= 0.0f && saturation <= 1.0f && "Color3::hsv: saturation must be in [0, 1]");
    assert(value >= 0.0f && value <= 1.0f           && "Color3::hsv: value must be in [0, 1]");

    hue        = std::clamp(hue,        0.0f, 359.999f);
    saturation = std::clamp(saturation, 0.0f, 1.0f);
    value      = std::clamp(value,      0.0f, 1.0f);

    if (saturation == 0.0f) return {value, value, value};

    hue /= 60.0f;
    const int   index    = static_cast<int>(hue);
    const float fraction = hue - static_cast<float>(index);
    const float base     = value * (1.0f - saturation);
    const float fade     = value * (1.0f - saturation * fraction);
    const float rise     = value * (1.0f - saturation * (1.0f - fraction));

    switch (index % 6) {
        case 0:  return {value, rise,  base};
        case 1:  return {fade,  value, base};
        case 2:  return {base,  value, rise};
        case 3:  return {base,  fade,  value};
        case 4:  return {rise,  base,  value};
        default: return {value, base,  fade};
    }
}

Color3 Color3::hex(const unsigned int hex) {
    return {
        static_cast<float>((hex >> 16) & 0xFF) / 255.0f,
        static_cast<float>((hex >>  8) & 0xFF) / 255.0f,
        static_cast<float>((hex)       & 0xFF) / 255.0f
    };
}

Color3 Color3::rgb(const uint8_t red, const uint8_t green, const uint8_t blue) {
    return {
        static_cast<float>(red)   / 255.0f,
        static_cast<float>(green) / 255.0f,
        static_cast<float>(blue)  / 255.0f
    };
}

std::ostream &operator<<(std::ostream &os, const Color3 &color) {
    os << "Color3(" << color.red << ", " << color.green << ", " << color.blue << ")";
    return os;
}
