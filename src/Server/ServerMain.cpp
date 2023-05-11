#include "Server.h"

int main() {
  if (Server::StaticInit()) {
    return Server::Instance->Run();
  } else {
    return 1;
  }
}