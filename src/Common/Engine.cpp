#include "Engine.h"

std::unique_ptr<Engine> Engine::sInstance;

int Engine::Run() {
  return DoRunLoop();
}

Engine::Engine() {
  quit = false;
}

Engine::~Engine() = default;

int Engine::DoRunLoop() {
  double time = 0;
  const double dt = 0.01;
  double currentTime = Timing::sInstance.GetTime();
  double accumulator = 0.0;
  while (!quit) {
    double newTime = Timing::sInstance.GetTime();
    double frameTime = newTime - currentTime;
    currentTime = newTime;

    accumulator += frameTime;
    while (accumulator >= dt) {
      accumulator -= dt;
      time += dt;
    }
    DoFrame();
  }

  return 0;
}

void Engine::DoFrame() {
  World::sInstance->Update();
}
