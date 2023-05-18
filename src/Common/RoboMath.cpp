#include "RoboMath.h"

#include <random>

// Mersenne Twister pseudo-random 0..1f
float RoboMath::GetRandomFloat() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<float> dis(0.f, 1.f);
  return dis(gen);
}
