#include "InputManager.h"

#include "ClientNetworkManager.h"
#include "Clock.h"
#include "SDL_keyboard.h"

std::unique_ptr<InputManager> InputManager::Instance;

void InputManager::StaticInit() {
  Instance.reset(new InputManager());
}

InputManager::InputManager() {}

void InputManager::Update() {}

void InputManager::HandleInput([[maybe_unused]] EInputAction action, int key) {
  switch (key) {
    case SDLK_RETURN:
      ClientNetworkManager::Instance->SendWinPacket();
  }
}

bool InputManager::IsTimeToSampleInput() {
  float time = Clock::Instance.GetFrameStartTime();

  if (time > nextInputSampleTime) {
    nextInputSampleTime = nextInputSampleTime + nextInputDelta;
    return true;
  }

  return false;
}
