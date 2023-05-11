#include <memory>

#ifndef ROBO_WORLD_H
class World {
 public:
  static void StaticInit();
  static std::unique_ptr<World> Instance;
  void Update();

 private:
  World();
};
#define ROBO_WORLD_H

#endif  // ROBO_WORLD_H
