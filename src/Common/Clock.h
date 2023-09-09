#ifndef ROBO_CLOCK_H
#define ROBO_CLOCK_H

class Clock {
 public:
  static Clock Instance;
  Clock();

  void Update();

  [[nodiscard]] double GetTime() const;

  [[nodiscard]] float GetTimeF() const {
    return static_cast<float>(GetTime());
  }

  [[nodiscard]] float GetFrameStartTime() const {
    return frameStartTime;
  }

 private:
  float deltaTime;

  double perfCountDuration;
  double lastFrameStartTime;

  float frameStartTime;
};

#endif  // ROBO_CLOCK_H
