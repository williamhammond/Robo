#include "WindowManager.h"

std::unique_ptr<WindowManager> WindowManager::Instance;

bool WindowManager::StaticInit() {
  SDL_Window* wnd = SDL_CreateWindow("Game", 100, 100, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (wnd == nullptr) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create window.");
    return false;
  }

  Instance.reset(new WindowManager(wnd));

  return true;
}

WindowManager::WindowManager(SDL_Window* inMainWindow) {
  mMainWindow = inMainWindow;
}

WindowManager::~WindowManager() {
  SDL_DestroyWindow(mMainWindow);
}