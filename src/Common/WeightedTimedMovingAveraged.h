#ifndef ROBO_WEIGHTEDTIMEDMOVINGAVERAGED_H
#define ROBO_WEIGHTEDTIMEDMOVINGAVERAGED_H

#include "Clock.h"
class WeightedTimedMovingAverage {
 public:
  explicit WeightedTimedMovingAverage(float inDuration = 5.f) : value(0.f), duration(inDuration) {
    lastEntryMade = Clock::Instance.GetTimeF();
  }

  void UpdatePerSecond(float inValue) {
    float time = Clock::Instance.GetTimeF();
    float timeSinceLastEntry = time - lastEntryMade;

    float valueOverTime = inValue / timeSinceLastEntry;

    float fractionOfDuration = (timeSinceLastEntry / duration);
    if (fractionOfDuration > 1.f) {
      fractionOfDuration = 1.f;
    }

    value = value * (1.f - fractionOfDuration) + valueOverTime * fractionOfDuration;

    lastEntryMade = time;
  }

  void Update(float inValue) {
    float time = Clock::Instance.GetTimeF();
    float timeSinceLastEntry = time - lastEntryMade;

    float fractionOfDuration = (timeSinceLastEntry / duration);
    if (fractionOfDuration > 1.f) {
      fractionOfDuration = 1.f;
    }

    value = value * (1.f - fractionOfDuration) + inValue * fractionOfDuration;

    lastEntryMade = time;
  }

  [[nodiscard]] float GetValue() const {
    return value;
  }

 private:
  float lastEntryMade;
  float value;
  float duration;
};
#endif  // ROBO_WEIGHTEDTIMEDMOVINGAVERAGED_H
