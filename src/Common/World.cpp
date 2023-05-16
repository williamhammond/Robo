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

void World::AddGameObject(GameObjectPtr gameObject) {
  gameObjects.push_back(gameObject);
  gameObject->SetWorldIndex(static_cast<int>(gameObjects.size()) - 1);
}
