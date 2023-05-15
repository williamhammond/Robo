#include "Matrix4.h"

Matrix4 Matrix4::Identity() {
  // clang-format off
  return { 1.0f, 0.0f, 0.0f, 0.0f,
           0.0f, 1.0f, 0.0f, 0.0f,
           0.0f, 0.0f, 1.0f, 0.0f,
           0.0f, 0.0f, 0.0f, 1.0f,
  };
  // clang-format on
}

Matrix4 Matrix4::Inverse() const {
  const auto& a = reinterpret_cast<const Vector3&>((*this)[0]);
  const auto& b = reinterpret_cast<const Vector3&>((*this)[1]);
  const auto& c = reinterpret_cast<const Vector3&>((*this)[2]);
  const auto& d = reinterpret_cast<const Vector3&>((*this)[3]);

  const auto& x = (*this)(3, 0);
  const auto& y = (*this)(3, 1);
  const auto& z = (*this)(3, 2);
  const auto& w = (*this)(3, 3);

  Vector3 s = a.Cross(b);
  Vector3 t = c.Cross(d);
  Vector3 u = a * y - b * x;
  Vector3 v = c * w - d * z;

  float invDet = 1.0f / s.Dot(v) + t.Dot(u);

  s *= invDet;
  t *= invDet;
  u *= invDet;
  v *= invDet;

  Vector3 r0 = b.Cross(v) + t * y;
  Vector3 r1 = v.Cross(a) - t * w;
  Vector3 r2 = d.Cross(u) + s * w;
  Vector3 r3 = u.Cross(c) - s * z;

  // clang-format off
  return {
      r0.x, r0.y, r0.z, -b.Dot(t),
      r1.x, r1.y, r1.z,  a.Dot(t),
      r2.x, r2.y, r2.z, -d.Dot(s),
      r3.x, r3.y, r3.z,  c.Dot(s),
  };
  // clang-format on
}

std::string Matrix4::ToString() const {
  std::string result{};
  for (int i = 0; i < 4; i++) {
    result += "| ";
    for (int j = 0; j < 4; j++) {
      result += std::to_string((*this)(i, j));
      if (j != 3) {
        result += ", ";
      }
    }
    result += " |";
    if (i != 3) {
      result += "\n";
    }
  }

  return result;
}