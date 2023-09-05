#include "Lib/Client.h"

int main() {
  if (Client::StaticInit()) {
    return Client::Instance->Run();
  } else {
    return 1;
  }
}
