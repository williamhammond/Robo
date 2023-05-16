#ifndef ROBO_GAMEMANAGER_H
#define ROBO_GAMEMANAGER_H

#include <GameObject.h>

class GameManager : public GameObject {
 public:
  GameManager();
  void Update() override;
  void WinButton();
};

typedef shared_ptr<GameManager> GameManagerPtr;
#endif  // ROBO_GAMEMANAGER_H
