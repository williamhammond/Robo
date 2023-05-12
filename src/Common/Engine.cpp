#include "Engine.h"

std::unique_ptr<Engine> Engine::Instance;

int Engine::Run() {
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