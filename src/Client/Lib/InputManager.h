#ifndef ROBO_INPUTMANAGER_H
#define ROBO_INPUTMANAGER_H

#include <memory>

#include "InputAction.h"

class InputManager {
 public:
  static std::unique_ptr<InputManager> Instance;

  static void StaticInit();
  void Update();
  void HandleInput(EInputAction action, int key);
  bool IsTimeToSampleInput();

 private:
  InputManager();
  float nextInputSampleTime;
  float nextInputDelta;
};

#endif  // ROBO_INPUTMANAGER_H
