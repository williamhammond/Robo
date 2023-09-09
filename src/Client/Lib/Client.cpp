#include "Client.h"

#include "ClientNetworkManager.h"
#include "InputManager.h"
#include "SocketAddressFactory.h"
#include "WindowManager.h"

bool Client::StaticInit() {
  auto* client = new Client();
  if (!WindowManager::StaticInit()) {
    return false;
  }
  Instance.reset(client);
  InputManager::StaticInit();
  return true;
}

Client::Client() {
  std::string dest = "127.0.0.1:2000";
  std::string name = "test";
  SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString(dest);
  ClientNetworkManager::StaticInit(*serverAddress, name);
}

void Client::Update() {
  InputManager::Instance->Update();

  Engine::Update();
  ClientNetworkManager::Instance->ProcessIncomingPackets();
  ClientNetworkManager::Instance->SendOutgoingPackets();
}

void Client::HandleEvent(SDL_Event* event) {
  switch (event->type) {
    case SDL_KEYDOWN:
      InputManager::Instance->HandleInput(EIA_Pressed, event->key.keysym.sym);
      break;
    case SDL_KEYUP:
      InputManager::Instance->HandleInput(EIA_Released, event->key.keysym.sym);
      break;
    default:
      break;
  }
}
