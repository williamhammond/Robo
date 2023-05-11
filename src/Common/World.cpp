#include "World.h"

std::unique_ptr<World> World::sInstance;

void World::StaticInit() {
  sInstance.reset(new World());
}
void World::Update() {}

World::World() = default;
