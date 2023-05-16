#ifndef ROBO_SERVER_H
#define ROBO_SERVER_H

#include "../Common/Engine.h"
#include "GameManager.h"

class Server : public Engine {
 public:
  static bool StaticInit();

  int Run() override;

 private:
  Server();
  GameManager gameManager;
};

#endif  // ROBO_SERVER_H
