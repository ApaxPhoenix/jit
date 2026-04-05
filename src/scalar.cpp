#include "../include/scalar.hpp"
#include <ostream>

std::ostream &operator<<(std::ostream &os, const Scalar &scalar) {
    return os << scalar.value;
}