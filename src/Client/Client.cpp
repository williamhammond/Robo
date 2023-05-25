#include "Client.h"

#include <SocketAddressFactory.h>

#include "ClientNetworkManager.h"

bool Client::StaticInit() {
  auto* client = new Client();
  Instance.reset(client);
  return true;
}

Client::Client() {
  std::string dest = "127.0.0.1:2000";
  std::string name = "test";

  SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString(dest);
  ClientNetworkManager::StaticInit(*serverAddress, name);
}

void Client::Update() {
  Engine::Update();
  ClientNetworkManager::Instance->ProcessIncomingPackets();
  ClientNetworkManager::Instance->SendOutgoingPackets();
}
