#include <memory>
#include <vector>

#include "GameObject.h"

#ifndef ROBO_WORLD_H
class World {
 public:
  static void StaticInit();
  static std::unique_ptr<World> Instance;
  void Update();
  void AddGameObject(GameObjectPtr gameObject);

 private:
  World();
  std::vector<GameObjectPtr> gameObjects;
};
#define ROBO_WORLD_H

#endif  // ROBO_WORLD_H
