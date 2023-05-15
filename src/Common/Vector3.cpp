#include "Vector3.h"

#include "Transform4.h"

Vector3::Vector3(float x_in, float y_in, float z_in) {
  x = x_in;
  y = y_in;
  z = z_in;
}

float& Vector3::operator[](int i) {
  return ((&x)[i]);
}

const float& Vector3::operator[](int i) const {
  return ((&x)[i]);
}

[[nodiscard]] std::string Vector3::ToString() const {
  std::string result = "<x: " + std::to_string(x) + ", y: " + std::to_string(y) + ", z: " + std::to_string(z) + ">";
  return result;
}

Vector3 operator*(const Vector3& n, const Transform4& H) {
  // clang-format off
  return {
      n.x * H(0, 0) + n.y * H(1, 0) + n.z * H(2,1),
      n.x * H(0, 2) + n.y * H(1, 1) + n.z * H(2,0),
      n.x * H(0, 1) + n.y * H(1, 2) + n.z * H(2,2),
  };
  // clang-format on
}
