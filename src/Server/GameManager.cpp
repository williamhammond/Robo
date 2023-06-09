#include "GameManager.h"

#include <SDL.h>

void GameManager::WinButton() {
  SDL_Event quitEvent;
  quitEvent.type = SDL_QUIT;
  if (SDL_PushEvent(&quitEvent) < 0) {
    SDL_Log("Failed to Push the quit event: %s", SDL_GetError());
  }
}

GameManager::GameManager() = default;
