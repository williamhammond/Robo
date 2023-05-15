#ifndef ROBO_GAMEOBJECT_H
#define ROBO_GAMEOBJECT_H

#include "Vector3.h"
#include "memory"

using std::shared_ptr;

class GameObject {
 public:
  GameObject(Vector3& in_location);

  virtual ~GameObject() {}
  virtual void Update();

  [[nodiscard]] const Vector3& GetLocation() const {
    return location;
  }
  void SetLocation(const Vector3& inLocation) {
    location = inLocation;
  }

  void SetWorldIndex(int i) {
    worldIndex = i;
  }

  [[nodiscard]] int GetWorldIndex() const {
    return worldIndex;
  }

 private:
  int worldIndex;
  Vector3& location;
};

typedef shared_ptr<GameObject> GameObjectPtr;
#endif  // ROBO_GAMEOBJECT_H
