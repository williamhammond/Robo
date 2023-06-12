#ifndef ROBO_GAMEOBJECT_H
#define ROBO_GAMEOBJECT_H

#include <memory>

#include "MemoryBitStream.h"

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

  virtual uint32_t Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const {
    (void)inOutputStream;
    (void)inDirtyState;
    return 0;
  }

  virtual void Read(InputMemoryBitStream& inInputStream) {
    (void)inInputStream;
  }

  [[nodiscard]] virtual uint32_t GetClassId() const {
    return classId;
  }

  [[nodiscard]] virtual uint32_t GetAllStateMask() const {
    return 0;
  }

 private:
  int worldIndex;
  int classId;
};

typedef shared_ptr<GameObject> GameObjectPtr;
#endif  // ROBO_GAMEOBJECT_H
