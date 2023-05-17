#ifndef ROBO_TCPSOCKET_H
#define ROBO_TCPSOCKET_H

#include "SocketAddress.h"
#include "memory"

class TCPSocket {
 public:
  ~TCPSocket();
  int Connect(const SocketAddress& address);
  int Bind(const SocketAddress& bindAddress);
  int Listen(int backlog = 32);
  std::shared_ptr<TCPSocket> Accept(SocketAddress& fromAddress);
  int32_t Send(const void* data, size_t len);
  int32_t Receive(void* data, size_t len);

 private:
  friend class SocketUtil;
  TCPSocket(SOCKET inSocket) : mSocket(inSocket) {}
  SOCKET mSocket;
};
typedef std::shared_ptr<TCPSocket> TCPSocketPtr;

#endif  // ROBO_TCPSOCKET_H
