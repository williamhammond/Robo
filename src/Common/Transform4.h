#ifndef ROBO_TRANSFORM4_H
#define ROBO_TRANSFORM4_H

#include "Matrix4.h"
#include "Vector3.h"

class Transform4 : Matrix4 {
 public:
  Transform4() = default;
  using Matrix4::operator();

  Transform4(float n00, float n01, float n02, float n03, float n10, float n11, float n12, float n13, float n20,
             float n21, float n22, float n23)
      : Matrix4() {
    n[0][0] = n00;
    n[1][0] = n01;
    n[2][0] = n02;
    n[3][0] = n03;

    n[0][1] = n10;
    n[1][1] = n11;
    n[2][1] = n12;
    n[3][1] = n13;

    n[0][2] = n20;
    n[1][2] = n21;
    n[2][2] = n22;
    n[3][2] = n23;

    n[0][3] = 0.0f;
    n[1][3] = 0.0f;
    n[2][3] = 0.0f;
    n[3][3] = 1.0f;
  }

  Transform4(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& p) {
    n[0][0] = a.x;
    n[0][1] = a.y;
    n[0][2] = a.z;
    n[0][3] = 0.0f;

    n[1][0] = b.x;
    n[1][1] = b.y;
    n[1][2] = b.z;
    n[1][3] = 0.0f;

    n[2][0] = c.x;
    n[2][1] = c.y;
    n[2][2] = c.z;
    n[2][3] = 0.0f;

    n[3][0] = p.x;
    n[3][1] = p.y;
    n[3][2] = p.z;
    n[3][3] = 1.0f;
  }

  Vector3& operator[](int j) {
    return (*reinterpret_cast<Vector3*>(n[j]));
  }

  const Vector3& operator[](int j) const {
    return (*reinterpret_cast<const Vector3*>(n[j]));
  }

  [[nodiscard]] const Vector3& GetTranslation() const {
    return (*reinterpret_cast<const Vector3*>(n[3]));
  }

  void SetTranslation(const Vector3& p) {
    n[3][0] = p.x;
    n[3][1] = p.y;
    n[3][2] = p.z;
  }

  Transform4 Inverse(const Transform4& H);

  Transform4 operator*(const Transform4 B) const {
    return {
        (*this)(0, 0) * B(0, 0) + (*this)(0, 1) * B(1, 0) + (*this)(0, 2) * B(2, 0),
        (*this)(0, 0) * B(0, 1) + (*this)(0, 1) * B(1, 1) + (*this)(0, 2) * B(2, 1),
        (*this)(0, 0) * B(0, 2) + (*this)(0, 1) * B(1, 2) + (*this)(0, 2) * B(2, 2),
        (*this)(0, 0) * B(0, 3) + (*this)(0, 1) * B(1, 3) + (*this)(0, 2) * B(2, 3) + (*this)(0, 3),

        (*this)(1, 0) * B(0, 0) + (*this)(1, 1) * B(1, 0) + (*this)(1, 2) * B(2, 0),
        (*this)(1, 0) * B(0, 1) + (*this)(1, 1) * B(1, 1) + (*this)(1, 2) * B(2, 1),
        (*this)(1, 0) * B(0, 2) + (*this)(1, 1) * B(1, 2) + (*this)(1, 2) * B(2, 2),
        (*this)(1, 0) * B(0, 3) + (*this)(1, 1) * B(1, 3) + (*this)(1, 2) * B(2, 3) + (*this)(1, 3),

        (*this)(2, 0) * B(0, 0) + (*this)(2, 1) * B(1, 0) + (*this)(2, 2) * B(2, 0),
        (*this)(2, 0) * B(0, 1) + (*this)(2, 1) * B(1, 1) + (*this)(2, 2) * B(2, 1),
        (*this)(2, 0) * B(0, 2) + (*this)(2, 1) * B(1, 2) + (*this)(2, 2) * B(2, 2),
        (*this)(2, 0) * B(0, 3) + (*this)(2, 1) * B(1, 3) + (*this)(2, 2) * B(2, 3) + (*this)(2, 3),
    };
  }

  Vector3 operator*(const Vector3& v) {
    return {(*this)(0, 0) * v.x + (*this)(0, 1) * v.y + (*this)(0, 2) * v.z,
            (*this)(1, 0) * v.x + (*this)(1, 1) * v.y + (*this)(1, 2) * v.z,
            (*this)(2, 0) * v.x + (*this)(2, 1) * v.y + (*this)(2, 2) * v.z};
  }
};

#endif  // ROBO_TRANSFORM4_H
