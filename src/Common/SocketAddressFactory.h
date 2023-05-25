#ifndef ROBO_SOCKETADDRESSFACTORY_H
#define ROBO_SOCKETADDRESSFACTORY_H

#include "SocketAddress.h"

class SocketAddressFactory {
 public:
  static SocketAddressPtr CreateIPv4FromString(const std::string& destination);
};

#endif  // ROBO_SOCKETADDRESSFACTORY_H
