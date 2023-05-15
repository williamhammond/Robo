#include "FMath.h"

#include <numbers>
#include <stdexcept>

/**
 * Implemented using Newton's Method
 * https://en.wikipedia.org/wiki/Newton's_method
 */
float FMath::sqrt(float x) {
  if (x == 0) {
    return 0;
  }

  if (x < 0) {
    throw std::invalid_argument("sqrt only accepts positive numbers");
  }

  float precision = 0.001f;
  float r = x;
  float root;

  int count = 0;
  while (true) {
    count++;
    root = 0.5f * (r + (x / r));
    if (abs(root - r) < precision) break;
    r = root;
  }

  return root;
}

float FMath::square(float x) {
  return x * x;
}

float FMath::abs(float x) {
  if (x >= 0) {
    return x;
  }
  return -x;
}

int FMath::abs(int x) {
  if (x >= 0) {
    return x;
  }
  return -x;
}

float FMath::pow(float x, int exponent) {
  bool is_negative = exponent < 0;
  exponent = abs(exponent);
  float answer = 1;
  while (exponent > 0) {
    answer *= x;
    exponent--;
  }
  if (is_negative) {
    return 1 / answer;
  }
  return answer;
}
float FMath::min(float x, float y) {
  if (x > y) {
    return y;
  }
  return x;
}

float FMath::max(float x, float y) {
  if (x > y) {
    return x;
  }
  return y;
}

/**
 *  https://www.gamedeveloper.com/programming/in-depth-comparing-floating-point-numbers-2012-edition
 *  https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison
 *  https://randomascii.wordpress.com/2012/01/11/tricks-with-the-floating-point-format/
 *  https://github.com/google/googletest/blob/main/googletest/include/gtest/internal/gtest-internal.h#L339
 */
float FMath::relative_epsilon(float x, float y, float rel_diff) {
  return max(abs(x), abs(y)) * rel_diff;
}

// TODO: look into if this type of cast is correct
float FMath::Degree2Radian(float degree) {
  return ((float)std::numbers::pi / 180) * degree;
}

float FMath::Radian2Degree(float rad) {
  return rad / (float)(std::numbers::pi / 180);
}

bool FMath::Equals(float a, float b, float epsilon) {
  return abs(a - b) <= ((abs(a) < abs(b) ? abs(b) : abs(a)) * epsilon);
}
