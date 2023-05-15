#include "World.h"

World::World() = default;

std::unique_ptr<World> World::Instance;

void World::StaticInit() {
  Instance.reset(new World());
}

void World::Update() {
  for (auto& gameObject : gameObjects) {
    gameObject->Update();
  }
}
