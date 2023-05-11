#include "World.h"

std::unique_ptr<World> World::Instance;

void World::StaticInit() {
  Instance.reset(new World());
}
void World::Update() {}

World::World() = default;
