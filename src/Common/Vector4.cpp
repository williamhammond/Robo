#include "Vector4.h"

Vector4::Vector4(float x_in, float y_in, float z_in, float w_in) {
  x = x_in;
  y = y_in;
  z = z_in;
  w = w_in;
}

float& Vector4::operator[](int i) {
  return ((&x)[i]);
}

const float& Vector4::operator[](int i) const {
  return ((&x)[i]);
}

[[nodiscard]] std::string Vector4::ToString() const {
  std::string result = "<x: " + std::to_string(x) + ", y: " + std::to_string(y) + ", z: " + std::to_string(z) +
                       +", w: " + std::to_string(w) + ">";
  return result;
}
