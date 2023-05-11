#ifndef ROBO_ENGINE_H
#define ROBO_ENGINE_H
#include <memory>

#include "Clock.h"
#include "World.h"

class Engine {
 public:
  static std::unique_ptr<Engine> Instance;

  virtual ~Engine();
  virtual int Run();

  void SetQuit(bool shouldQuit) {
    quit = shouldQuit;
  }

 protected:
  Engine();
  virtual void Update();

 private:
  bool quit;
};

#endif  // ROBO_ENGINE_H
