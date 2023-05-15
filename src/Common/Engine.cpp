#include "Engine.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"

std::unique_ptr<Engine> Engine::Instance;
Engine::Engine() {
  World::StaticInit();
  quit = false;
}

Engine::~Engine() = default;

int Engine::Run() {
  spdlog::set_level(spdlog::level::trace);
  SPDLOG_TRACE("Engine::Run");
  const double dt = 0.01;
  double currentTime = Clock::Instance.GetTime();
  double accumulator = 0.0;

  double secondTimer = 0;
  int frameCount = 0;
  while (!quit) {
    double newTime = Clock::Instance.GetTime();
    double frameTime = newTime - currentTime;

    secondTimer += frameTime;
    frameCount++;
    if (secondTimer > 1) {
      SPDLOG_TRACE("FPS: {}", frameCount);
      framesPerSecond = frameCount;
      secondTimer = 0;
      frameCount = 0;
    }

    currentTime = newTime;
    accumulator += frameTime;
    while (accumulator >= dt) {
      accumulator -= dt;
    }
    Update();
  }

  return 0;
}

void Engine::Update() {
  World::Instance->Update();
}
