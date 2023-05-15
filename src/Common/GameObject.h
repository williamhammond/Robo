#ifndef ROBO_GAMEOBJECT_H
#define ROBO_GAMEOBJECT_H

#include "memory"

using std::shared_ptr;

class GameObject {
 public:
  GameObject();

  virtual ~GameObject() {}
  virtual void Update();

  void SetWorldIndex(int i) {
    worldIndex = i;
  }

  [[nodiscard]] int GetWorldIndex() const {
    return worldIndex;
  }

 private:
  int worldIndex;
};

typedef shared_ptr<GameObject> GameObjectPtr;
#endif  // ROBO_GAMEOBJECT_H
