#ifndef ROBO_ENGINE_H
#define ROBO_ENGINE_H
#include <memory>

#include "Clock.h"
#include "World.h"
#include "spdlog/spdlog.h"

class Engine {
 public:
  static std::unique_ptr<Engine> Instance;

  virtual ~Engine();
  virtual int Run();

  void SetQuit(bool shouldQuit) {
    SPDLOG_INFO("Engine shutting down");
    quit = shouldQuit;
  }

  int GetFramesPerSecond() {
    return framesPerSecond;
  }

 protected:
  Engine();
  virtual void Update();

 private:
  int framesPerSecond;
  bool quit;
};

#endif  // ROBO_ENGINE_H
