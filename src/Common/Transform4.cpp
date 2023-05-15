#include "Transform4.h"

Transform4 Transform4::Inverse(const Transform4& H) {
  const Vector3& a = H[0];
  const Vector3& b = H[1];
  const Vector3& c = H[2];
  const Vector3& d = H[3];

  Vector3 s = a.Cross(b);
  Vector3 t = c.Cross(d);

  float invDet = 1.0f / s.Dot(c);
  s *= invDet;
  t *= invDet;
  Vector3 v = c * invDet;

  Vector3 r0 = b.Cross(v);
  Vector3 r1 = v.Cross(a);

  // clang-format off
  return {
      r0.x, r0.y, r0.z, -b.Dot(t),
      r1.x, r1.y, r1.z,  a.Dot(t),
      s.x,  s.y,  s.z, -d.Dot(s),
  };
  // clang-format on
}