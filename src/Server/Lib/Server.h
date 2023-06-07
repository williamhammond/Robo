#ifndef ROBO_SERVER_H
#define ROBO_SERVER_H

#include "ClientProxy.h"
#include "Engine.h"
#include "GameManager.h"

class Server : public Engine {
 public:
  static bool StaticInit();

  int Run() override;
  void Update() override;

  static void HandleNewClient([[maybe_unused]] const ClientProxyPtr& inClientProxy);

 private:
  static bool InitNetworkManager();
  Server();
  GameManager gameManager;
};

#endif  // ROBO_SERVER_H
