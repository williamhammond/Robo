#include "Clock.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <chrono>
using namespace std::chrono;
#endif

Clock Clock::Instance;

namespace {
#if defined(_WIN32)
LARGE_INTEGER sStartTime = {};
#else
high_resolution_clock::time_point sStartTime;
#endif
}  // namespace

Clock::Clock() {
#if defined(_WIN32)
  LARGE_INTEGER perfFreq;
  QueryPerformanceFrequency(&perfFreq);
  perfCountDuration = 1.0 / static_cast<double>(perfFreq.QuadPart);

  QueryPerformanceCounter(&sStartTime);
#else
  sStartTime = high_resolution_clock::now();
#endif
}

double Clock::GetTime() const {
#if defined(_WIN32)
  LARGE_INTEGER curTime, timeSinceStart;
  QueryPerformanceCounter(&curTime);

  timeSinceStart.QuadPart = curTime.QuadPart - sStartTime.QuadPart;

  return static_cast<double>(timeSinceStart.QuadPart) * perfCountDuration;
#else
  auto now = high_resolution_clock::now();
  auto ms = duration_cast<milliseconds>(now - sStartTime).count();
  return static_cast<double>(ms) / 1000;
#endif
}
