#include "Server.h"

#include "ServerNetworkManager.h"

int Server::Run() {
  return Engine::Run();
}

bool Server::StaticInit() {
  Instance.reset(new Server());

  return true;
}

Server::Server() {
  InitNetworkManager();
}

bool Server::InitNetworkManager() {
  return ServerNetworkManager::StaticInit(2000);
}

void Server::Update() {
  ServerNetworkManager::Instance->ProcessIncomingPackets();
  ServerNetworkManager::Instance->CheckForDisconnects();

  Engine::Update();

  ServerNetworkManager::Instance->SendOutgoingPackets();
}

void Server::HandleNewClient([[maybe_unused]] const ClientProxyPtr& inClientProxy) {}
