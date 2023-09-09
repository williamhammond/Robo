#include "Lib/Client.h"

#if defined(_WIN32)
#include "Windows.h"
int WINAPI WinMain([[maybe_unused]] _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                   [[maybe_unused]] _In_ LPSTR lpCmdLine, [[maybe_unused]] _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  if (Client::StaticInit()) {
    return Client::Instance->Run();
  } else {
    SDL_Quit();
    return 1;
  }
}
#else
const char** __argv;
int __argc;
int main(int argc, const char** argv) {
  __argc = argc;
  __argv = argv;

  if (Client::StaticInit()) {
    return Client::Instance->Run();
  } else {
    SDL_Quit();
    // error
    return 1;
  }
}
#endif
