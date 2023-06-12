#include "SocketAddress.h"

#include <fmt/core.h>

#include <string>

std::string SocketAddress::ToString() const {
#if defined(_WIN32)
  const sockaddr_in* s = GetAsSockAddrIn();
  char destinationBuffer[128];
  InetNtop(s->sin_family, const_cast<in_addr*>(&s->sin_addr), destinationBuffer, sizeof(destinationBuffer));
  return fmt::format("{}:{}", destinationBuffer, ntohs(s->sin_port));
#else
  return {"not implemented on unix for now"};
#endif
}
