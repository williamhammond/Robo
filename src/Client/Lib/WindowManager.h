#ifndef ROBO_WINDOWMANAGER_H
#define ROBO_WINDOWMANAGER_H

#include <memory>

#include "SDL.h"

class WindowManager {
 public:
  static bool StaticInit();
  static std::unique_ptr<WindowManager> Instance;

  SDL_Window* GetMainWindow() const {
    return mMainWindow;
  }

  ~WindowManager();

 private:
  WindowManager(SDL_Window* mainWindow);

  SDL_Window* mMainWindow;
};

#endif  // ROBO_WINDOWMANAGER_H
