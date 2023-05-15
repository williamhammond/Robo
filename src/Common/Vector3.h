#ifndef ROBO_VECTOR3_H
#define ROBO_VECTOR3_H

#include <sstream>

#include "FMath.h"

class Vector3 {
 public:
  float x, y, z;

  Vector3() = default;
  Vector3(float x_in, float y_in, float z_in);

  float& operator[](int i);
  const float& operator[](int i) const;

  inline Vector3 operator+(const Vector3& v) const {
    return {x + v.x, y + v.y, z + v.z};
  }
  inline Vector3& operator+=(const Vector3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return (*this);
  }
  inline Vector3 operator-(const Vector3& v) const {
    return {x - v.x, y - v.y, z - v.z};
  }
  inline Vector3& operator-=(const Vector3& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return (*this);
  }

  inline Vector3& operator*=(float a) {
    x *= a;
    y *= a;
    z *= a;
    return (*this);
  }
  inline Vector3& operator/=(float a) {
    x /= a;
    y /= a;
    z /= a;
    return (*this);
  }
  inline Vector3 operator*(float a) const {
    return {x * a, y * a, z * a};
  }
  inline Vector3 operator/(float a) const {
    return {x / a, y / a, z / a};
  }
  inline Vector3 operator-() const {
    return {-x, -y, -z};
  }

  [[nodiscard]] inline bool Equals(const Vector3& b, float epsilon = 0.0f) const {
    return FMath::abs(b.x - x) <= epsilon && FMath::abs(b.y - y) <= epsilon && FMath::abs(b.z - z) <= epsilon;
  }

  [[nodiscard]] inline float Magnitude() const {
    return FMath::sqrt(FMath::square(x) + FMath::square(y) + FMath::square(z));
  }

  [[nodiscard]] inline Vector3 Normalize() const {
    if (Magnitude() == 0) {
      return (*this);
    }
    return ((*this) / Magnitude());
  }

  [[nodiscard]] inline float Dot(Vector3 b) const {
    return x * b.x + y * b.y + z * b.z;
  }

  [[nodiscard]] inline Vector3 Cross(Vector3 b) const {
    return {y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x};
  }

  // TODO: figure out 0 projection
  [[nodiscard]] inline Vector3 Project(Vector3 b) const {
    return b * (this->Dot(b) / b.Dot(b));
  }

  [[nodiscard]] inline Vector3 Reject(const Vector3& b) const {
    return *this - Project(b);
  }

  [[nodiscard]] inline bool IsOrthogonal(const Vector3& b) const {
    return FMath::Equals((*this).Dot(b), 0, 1e-9f);
  }

  [[nodiscard]] std::string ToString() const;
};

#endif  // ROBO_VECTOR3_H
