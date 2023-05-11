#ifndef ROBO_CLOCK_H
#define ROBO_CLOCK_H

#include <cstdint>
class Clock {
 public:
  static Clock Instance;
  Clock();
  [[nodiscard]] double GetTime() const;

 private:
  double perfCountDuration;
};

#endif  // ROBO_CLOCK_H
