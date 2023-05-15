#include "GameObject.h"

GameObject::GameObject(Vector3& in_location) : location(in_location) {
  worldIndex = -1;
}

void GameObject::Update() {}
