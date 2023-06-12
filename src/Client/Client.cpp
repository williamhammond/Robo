#include "Client.h"

#include <SocketAddressFactory.h>

#include "ClientNetworkManager.h"
#include "RoboMath.h"

bool Client::StaticInit() {
  auto* client = new Client();
  Instance.reset(client);
  return true;
}

Client::Client() {
  std::string dest = "127.0.0.1:2000";
  std::string name = "test";
  sendWin = Clock::Instance.GetTime() + static_cast<double>(RoboMath::GetRandomFloat() * 1000000);
  SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString(dest);
  ClientNetworkManager::StaticInit(*serverAddress, name);
}

void Client::Update() {
  Engine::Update();
  ClientNetworkManager::Instance->ProcessIncomingPackets();
  if (sendWin > Clock::Instance.GetTime()) {
    ClientNetworkManager::Instance->SendWinPacket();
  }
  ClientNetworkManager::Instance->SendOutgoingPackets();
}
