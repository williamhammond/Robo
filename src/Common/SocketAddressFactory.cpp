#include "SocketAddressFactory.h"

#include "SocketUtil.h"

#if !defined(_WIN32)
#include <netdb.h>
#endif

SocketAddressPtr SocketAddressFactory::CreateIPv4FromString(const std::string& destination) {
  auto pos = destination.find_last_of(':');
  std::string host, port;
  if (pos != std::string::npos) {
    host = destination.substr(0, pos);
    port = destination.substr(pos + 1);
  } else {
    host = destination;
    port = "0";
  }

  addrinfo hint{};
  memset(&hint, 0, sizeof(hint));
  hint.ai_family = AF_INET;

  addrinfo* address;
  int error = getaddrinfo(host.c_str(), port.c_str(), &hint, &address);
  if (error != 0 && address != nullptr) {
    SocketUtil::ReportError("SocketAddressFactory::CreateIPv4FromString");
    return nullptr;
  }

  while (!address->ai_addr && address->ai_next) {
    address = address->ai_next;
  }

  if (!address->ai_addr) {
    return nullptr;
  }

  auto result = std::make_shared<SocketAddress>(*address->ai_addr);
  freeaddrinfo(address);
  return result;
}
