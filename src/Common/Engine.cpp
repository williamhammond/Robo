#include "Engine.h"

std::unique_ptr<Engine> Engine::sInstance;

int Engine::Run() {
  double time = 0;
  const double dt = 0.01;
  double currentTime = Clock::Instance.GetTime();
  double accumulator = 0.0;
  while (!quit) {
    double newTime = Clock::Instance.GetTime();
    double frameTime = newTime - currentTime;
    currentTime = newTime;

    accumulator += frameTime;
    while (accumulator >= dt) {
      accumulator -= dt;
      time += dt;
    }
    Update();
  }

  return 0;
}

Engine::Engine() {
  quit = false;
}

Engine::~Engine() = default;

void Engine::Update() {
  World::Instance->Update();
}
