#ifndef ROBO_CLIENT_H
#define ROBO_CLIENT_H

#include <Engine.h>

class Client : public Engine {
 public:
  static bool StaticInit();

 protected:
  Client();
  double sendWin;

  void Update() override;
};

#endif  // ROBO_CLIENT_H
