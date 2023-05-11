#ifndef ROBO_ENGINE_H
#define ROBO_ENGINE_H
#include <memory>

#include "Timing.h"
#include "World.h"

class Engine {
 public:
  virtual ~Engine();
  static std::unique_ptr<Engine> sInstance;

  virtual int Run();
  void SetShouldKeepRunning(bool inShouldKeepRunning) {
    quit = inShouldKeepRunning;
  }

 protected:
  Engine();

  virtual void DoFrame();

 private:
  int DoRunLoop();

  bool quit;
};

#endif  // ROBO_ENGINE_H
