#ifndef ROBO_CLIENT_H
#define ROBO_CLIENT_H

#include <SDL.h>

#include "Engine.h"

class Client : public Engine {
 public:
  static bool StaticInit();

 protected:
  Client();

  void Update() override;
  void HandleEvent(SDL_Event* event) override;
};

#endif  // ROBO_CLIENT_H
