#include "TCPSocket.h"

#include "SocketUtil.h"

#if !defined(_WIN32)
#include <sys/socket.h>
#include <unistd.h>
#endif

int TCPSocket::Connect(const SocketAddress& address) {
  int err = connect(mSocket, &address.sockAddr, static_cast<socklen_t>(address.GetSize()));
  if (err < 0) {
    SocketUtil::ReportError("TCPSocket::Connect");
    return -SocketUtil::GetLastError();
  }
  return NO_ERROR;
}

int TCPSocket::Listen(int backlog) {
  int err = listen(mSocket, backlog);
  if (err < 0) {
    SocketUtil::ReportError("TCPSocket::Listen");
    return -SocketUtil::GetLastError();
  }
  return NO_ERROR;
}

TCPSocketPtr TCPSocket::Accept(SocketAddress& fromAddress) {
  auto length = static_cast<socklen_t>(fromAddress.GetSize());
  SOCKET newSocket = accept(mSocket, &fromAddress.sockAddr, &length);

  if (newSocket != INVALID_SOCKET) {
    return TCPSocketPtr(new TCPSocket(newSocket));
  } else {
    SocketUtil::ReportError("TCPSocket::Accept");
    return nullptr;
  }
}

int32_t TCPSocket::Send(const void* data, size_t len) {
  int bytesSentCount = send(mSocket, static_cast<const char*>(data), static_cast<socklen_t>(len), 0);
  if (bytesSentCount < 0) {
    SocketUtil::ReportError("TCPSocket::Send");
    return -SocketUtil::GetLastError();
  }
  return bytesSentCount;
}

int32_t TCPSocket::Receive(void* data, size_t len) {
  int bytesReceivedCount = recv(mSocket, static_cast<char*>(data), static_cast<socklen_t>(len), 0);
  if (bytesReceivedCount < 0) {
    SocketUtil::ReportError("TCPSocket::Receive");
    return -SocketUtil::GetLastError();
  }
  return bytesReceivedCount;
}

int TCPSocket::Bind(const SocketAddress& bindAddress) {
  int error = bind(mSocket, &bindAddress.sockAddr, static_cast<int>(bindAddress.GetSize()));
  if (error != 0) {
    SocketUtil::ReportError("TCPSocket::Bind");
    return SocketUtil::GetLastError();
  }

  return NO_ERROR;
}

TCPSocket::~TCPSocket() {
#if defined(_WIN32)
  closesocket(mSocket);
#else
  close(mSocket);
#endif
}
