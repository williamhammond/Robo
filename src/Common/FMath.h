#ifndef ROBO_FMATH_H
#define ROBO_FMATH_H

class FMath {
 public:
  static float sqrt(float x);
  static float square(float x);
  static float abs(float x);
  static int abs(int x);
  static float pow(float x, int exponent);
  static float min(float x, float y);
  static float max(float x, float y);
  static float relative_epsilon(float x, float y, float rel_diff = 1e-9f);
  static float Degree2Radian(float degree);
  static float Radian2Degree(float rad);
  static bool Equals(float a, float b, float epsilon);
};

#endif  // ROBO_FMATH_H
