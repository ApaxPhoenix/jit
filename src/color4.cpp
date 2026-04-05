#include "../include/color4.hpp"
#include <algorithm>
#include <cassert>

const Color4 Color4::WHITE       = Color4(1.0f, 1.0f, 1.0f, 1.0f);
const Color4 Color4::BLACK       = Color4(0.0f, 0.0f, 0.0f, 1.0f);
const Color4 Color4::RED         = Color4(1.0f, 0.0f, 0.0f, 1.0f);
const Color4 Color4::GREEN       = Color4(0.0f, 1.0f, 0.0f, 1.0f);
const Color4 Color4::BLUE        = Color4(0.0f, 0.0f, 1.0f, 1.0f);
const Color4 Color4::YELLOW      = Color4(1.0f, 1.0f, 0.0f, 1.0f);
const Color4 Color4::CYAN        = Color4(0.0f, 1.0f, 1.0f, 1.0f);
const Color4 Color4::MAGENTA     = Color4(1.0f, 0.0f, 1.0f, 1.0f);
const Color4 Color4::TRANSPARENT = Color4(0.0f, 0.0f, 0.0f, 0.0f);

Color4::Color4() : red(0.0f), green(0.0f), blue(0.0f), alpha(1.0f) {}

Color4::Color4(const float red, const float green, const float blue, const float alpha)
    : red(red), green(green), blue(blue), alpha(alpha) {}

Color4::Color4(const Color3 &color, const float alpha)
    : red(color.red), green(color.green), blue(color.blue), alpha(alpha) {}

Color4 Color4::operator+(const Color4 &color) const {
    return {this->red + color.red, this->green + color.green, this->blue + color.blue, this->alpha + color.alpha};
}

Color4 &Color4::operator+=(const Color4 &color) {
    this->red += color.red; this->green += color.green; this->blue += color.blue; this->alpha += color.alpha;
    return *this;
}

Color4 Color4::operator-(const Color4 &color) const {
    return {this->red - color.red, this->green - color.green, this->blue - color.blue, this->alpha - color.alpha};
}

Color4 &Color4::operator-=(const Color4 &color) {
    this->red -= color.red; this->green -= color.green; this->blue -= color.blue; this->alpha -= color.alpha;
    return *this;
}

Color4 Color4::operator*(const float scalar) const {
    return {this->red * scalar, this->green * scalar, this->blue * scalar, this->alpha * scalar};
}

Color4 &Color4::operator*=(const float scalar) {
    this->red *= scalar; this->green *= scalar; this->blue *= scalar; this->alpha *= scalar;
    return *this;
}

Color4 Color4::operator*(const Color4 &color) const {
    return {this->red * color.red, this->green * color.green, this->blue * color.blue, this->alpha * color.alpha};
}

Color4 &Color4::operator*=(const Color4 &color) {
    this->red *= color.red; this->green *= color.green; this->blue *= color.blue; this->alpha *= color.alpha;
    return *this;
}

Color4 Color4::operator/(const float scalar) const {
    assert(scalar != 0.0f && "Color4: division by zero scalar");
    return {this->red / scalar, this->green / scalar, this->blue / scalar, this->alpha / scalar};
}

Color4 &Color4::operator/=(const float scalar) {
    assert(scalar != 0.0f && "Color4: division by zero scalar");
    this->red /= scalar; this->green /= scalar; this->blue /= scalar; this->alpha /= scalar;
    return *this;
}

Color4 Color4::operator/(const Color4 &color) const {
    assert(color.red   != 0.0f && "Color4: division by zero (red channel)");
    assert(color.green != 0.0f && "Color4: division by zero (green channel)");
    assert(color.blue  != 0.0f && "Color4: division by zero (blue channel)");
    assert(color.alpha != 0.0f && "Color4: division by zero (alpha channel)");
    return {this->red / color.red, this->green / color.green, this->blue / color.blue, this->alpha / color.alpha};
}

Color4 &Color4::operator/=(const Color4 &color) {
    assert(color.red   != 0.0f && "Color4: division by zero (red channel)");
    assert(color.green != 0.0f && "Color4: division by zero (green channel)");
    assert(color.blue  != 0.0f && "Color4: division by zero (blue channel)");
    assert(color.alpha != 0.0f && "Color4: division by zero (alpha channel)");
    this->red /= color.red; this->green /= color.green; this->blue /= color.blue; this->alpha /= color.alpha;
    return *this;
}

bool Color4::operator==(const Color4 &color) const {
    return this->red == color.red && this->green == color.green && this->blue == color.blue && this->alpha == color.alpha;
}

bool Color4::operator!=(const Color4 &color) const { return !(*this == color); }

Scalar Color4::luminance() const {
    return Color3(this->red, this->green, this->blue).luminance();
}

Scalar Color4::difference(const Color4 &color) const {
    const float dr = this->red   - color.red;
    const float dg = this->green - color.green;
    const float db = this->blue  - color.blue;
    const float da = this->alpha - color.alpha;
    return static_cast<Scalar>(std::sqrt(dr*dr + dg*dg + db*db + da*da));
}

Color4 Color4::inverted() const {
    return {1.0f - this->red, 1.0f - this->green, 1.0f - this->blue, this->alpha};
}

Color4 Color4::clamped() const {
    return {
        std::clamp(this->red,   0.0f, 1.0f),
        std::clamp(this->green, 0.0f, 1.0f),
        std::clamp(this->blue,  0.0f, 1.0f),
        std::clamp(this->alpha, 0.0f, 1.0f)
    };
}

Color4 Color4::interpolate(const Color4 &color, float factor) const {
    factor = std::clamp(factor, 0.0f, 1.0f);
    return {
        this->red   + (color.red   - this->red)   * factor,
        this->green + (color.green - this->green)  * factor,
        this->blue  + (color.blue  - this->blue)   * factor,
        this->alpha + (color.alpha - this->alpha)  * factor
    };
}

Color4 Color4::gamma(const float exponent) const {
    assert(exponent > 0.0f && "Color4: gamma exponent must be positive");
    const Color4 safe = this->clamped();
    return {
        std::pow(safe.red,   exponent),
        std::pow(safe.green, exponent),
        std::pow(safe.blue,  exponent),
        this->alpha
    };
}

Color4 Color4::premultiplied() const {
    return {this->red * this->alpha, this->green * this->alpha, this->blue * this->alpha, this->alpha};
}

bool Color4::approximately(const Color4 &color, const float epsilon) const {
    return std::abs(this->red   - color.red)   <= epsilon &&
           std::abs(this->green - color.green)  <= epsilon &&
           std::abs(this->blue  - color.blue)   <= epsilon &&
           std::abs(this->alpha - color.alpha)  <= epsilon;
}

Color4 Color4::rgb(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha) {
    return {
        static_cast<float>(red)   / 255.0f,
        static_cast<float>(green) / 255.0f,
        static_cast<float>(blue)  / 255.0f,
        static_cast<float>(alpha) / 255.0f
    };
}

Color4 Color4::hsv(const float hue, const float saturation, const float value, const float alpha) {
    assert(hue >= 0.0f && hue < 360.0f             && "Color4::hsv: hue must be in [0, 360)");
    assert(saturation >= 0.0f && saturation <= 1.0f && "Color4::hsv: saturation must be in [0, 1]");
    assert(value >= 0.0f && value <= 1.0f           && "Color4::hsv: value must be in [0, 1]");
    assert(alpha >= 0.0f && alpha <= 1.0f           && "Color4::hsv: alpha must be in [0, 1]");
    return Color4(Color3::hsv(hue, saturation, value), alpha);
}

Color4 Color4::hex(const unsigned int hex) {
    return {
        static_cast<float>((hex >> 24) & 0xFF) / 255.0f,
        static_cast<float>((hex >> 16) & 0xFF) / 255.0f,
        static_cast<float>((hex >>  8) & 0xFF) / 255.0f,
        static_cast<float>((hex)       & 0xFF) / 255.0f
    };
}

std::ostream &operator<<(std::ostream &os, const Color4 &color) {
    os << "Color4(" << color.red << ", " << color.green << ", " << color.blue << ", " << color.alpha << ")";
    return os;
}