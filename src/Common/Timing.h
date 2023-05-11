#ifndef ROBO_TIMING_H
#define ROBO_TIMING_H

#include <cstdint>
class Timing {
 public:
  Timing();
  void Update();

  [[nodiscard]] float GetDeltaTime() const {
    return mDeltaTime;
  }
  [[nodiscard]] double GetTime() const;

  [[nodiscard]] float GetTimeF() const {
    return static_cast<float>(GetTime());
  }

  [[nodiscard]] float GetFrameStartTime() const {
    return mFrameStartTimef;
  }

  static Timing sInstance;

 private:
  float mDeltaTime{};
  uint64_t mDeltaTick{};
  float mFrameStartTimef{};

  double mLastFrameStartTime;
  double mPerfCountDuration;
};

#endif  // ROBO_TIMING_H
