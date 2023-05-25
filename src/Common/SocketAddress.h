#ifndef ROBO_SOCKETADDRESS_H
#define ROBO_SOCKETADDRESS_H

#if defined(_WIN32)
// Include order matters because windows
// clang-format off
typedef int socklen_t;
#include <winsock2.h>
#include <ws2tcpip.h>
// clang-format on
#else
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
typedef int SOCKET;
const int NO_ERROR = 0;
const int INVALID_SOCKET = -1;
const int WSAECONNRESET = ECONNRESET;
const int WSAEWOULDBLOCK = EAGAIN;
const int SOCKET_ERROR = -1;
#endif

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>

#include "memory"

class SocketAddress {
 public:
  SocketAddress(uint32_t inAddress, uint16_t inPort) {
    GetAsSockAddrIn()->sin_family = AF_INET;
    GetIP4Ref() = htonl(inAddress);
    GetAsSockAddrIn()->sin_port = htons(inPort);
  }

  SocketAddress(const sockaddr& inSockAddr) {
    memcpy(&sockAddr, &inSockAddr, sizeof(sockaddr));
  }

  SocketAddress() {
    GetAsSockAddrIn()->sin_family = AF_INET;
    GetIP4Ref() = INADDR_ANY;
    GetAsSockAddrIn()->sin_port = 0;
  }

  bool operator==(const SocketAddress& inOther) const {
    return (sockAddr.sa_family == AF_INET && GetAsSockAddrIn()->sin_port == inOther.GetAsSockAddrIn()->sin_port) &&
           (GetIP4Ref() == inOther.GetIP4Ref());
  }

  [[nodiscard]] size_t GetHash() const {
    return (GetIP4Ref()) | ((static_cast<uint32_t>(GetAsSockAddrIn()->sin_port)) << 13) | sockAddr.sa_family;
  }

  [[nodiscard]] uint32_t GetSize() const {
    return sizeof(sockaddr);
  }

  [[nodiscard]] std::string ToString() const;

 private:
  friend class UDPSocket;
  friend class TCPSocket;

  sockaddr sockAddr;
#if defined(_WIN32)
  uint32_t& GetIP4Ref() {
    return *reinterpret_cast<uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr);
  }
  [[nodiscard]] const uint32_t& GetIP4Ref() const {
    return *reinterpret_cast<const uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr);
  }
#else
  uint32_t& GetIP4Ref() {
    return GetAsSockAddrIn()->sin_addr.s_addr;
  }
  const uint32_t& GetIP4Ref() const {
    return GetAsSockAddrIn()->sin_addr.s_addr;
  }
#endif

  sockaddr_in* GetAsSockAddrIn() {
    return reinterpret_cast<sockaddr_in*>(&sockAddr);
  }

  [[nodiscard]] const sockaddr_in* GetAsSockAddrIn() const {
    return reinterpret_cast<const sockaddr_in*>(&sockAddr);
  }
};

typedef std::shared_ptr<SocketAddress> SocketAddressPtr;

namespace std {
template <>
struct hash<SocketAddress> {
  size_t operator()(const SocketAddress& socketAddress) const {
    return socketAddress.GetHash();
  }
};
}  // namespace std
#endif  // ROBO_SOCKETADDRESS_H
