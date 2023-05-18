#ifndef ROBO_CLOCK_H
#define ROBO_CLOCK_H

class Clock {
 public:
  static Clock Instance;
  Clock();
  [[nodiscard]] double GetTime() const;

  [[nodiscard]] float GetTimeF() const {
    return static_cast<float>(GetTime());
  }

 private:
  double perfCountDuration;
};

#endif  // ROBO_CLOCK_H
