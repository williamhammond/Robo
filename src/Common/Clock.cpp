#include "Clock.h"

#if _WIN32
#include <profileapi.h>
#include <winnt.h>

#include <chrono>
#else
#include <chrono>
using namespace std::chrono;
#endif

Clock Clock::Instance;

namespace {
#if _WIN32
LARGE_INTEGER sStartTime = {0};
#else
high_resolution_clock::time_point sStartTime;
#endif
}  // namespace

Clock::Clock() {
#if _WIN32
  LARGE_INTEGER perfFreq;
  QueryPerformanceFrequency(&perfFreq);
  perfCountDuration = 1.0 / perfFreq.QuadPart;

  QueryPerformanceCounter(&sStartTime);
#else
  sStartTime = high_resolution_clock::now();
#endif
}

double Clock::GetTime() const {
#if _WIN32
  LARGE_INTEGER curTime, timeSinceStart;
  QueryPerformanceCounter(&curTime);

  timeSinceStart.QuadPart = curTime.QuadPart - sStartTime.QuadPart;

  return timeSinceStart.QuadPart * perfCountDuration;
#else
  auto now = high_resolution_clock::now();
  auto ms = duration_cast<milliseconds>(now - sStartTime).count();
  return static_cast<double>(ms) / 1000;
#endif
}
