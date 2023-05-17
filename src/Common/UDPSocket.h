#ifndef ROBO_UDPSOCKET_H
#define ROBO_UDPSOCKET_H

#include "SocketAddress.h"

class UDPSocket {
 public:
  ~UDPSocket();

  [[nodiscard]] int Bind(const SocketAddress& toAddress) const;
  int SendTo(const void* toSend, int length, const SocketAddress& toAddress) const;
  int ReceiveFrom(void* inToReceive, int inMaxLength, SocketAddress& outFromAddress) const;

  [[nodiscard]] int SetNonBlockingMode(bool shouldBeNonBlocking) const;

 private:
  friend class SocketUtil;
  explicit UDPSocket(SOCKET inSocket) : mSocket(inSocket) {}
  SOCKET mSocket;
};

typedef std::shared_ptr<UDPSocket> UDPSocketPtr;

#endif  // ROBO_UDPSOCKET_H
