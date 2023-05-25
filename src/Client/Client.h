#ifndef ROBO_CLIENT_H
#define ROBO_CLIENT_H

#include <Engine.h>
#include <SDL_events.h>
class Client : public Engine {
 public:
  static bool StaticInit();

 protected:
  Client();

  void Update() override;
};

#endif  // ROBO_CLIENT_H
