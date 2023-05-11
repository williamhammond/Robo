#include "Server.h"

int Server::Run() {
  return Engine::Run();
}

bool Server::StaticInit() {
  sInstance.reset(new Server());

  return true;
}

Server::Server() = default;
