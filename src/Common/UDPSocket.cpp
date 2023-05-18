#include "UDPSocket.h"

#include <spdlog/spdlog.h>

#include "SocketUtil.h"

#if !defined(_WIN32)
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

int UDPSocket::Bind(const SocketAddress& toAddress) const {
  int error = bind(mSocket, &toAddress.sockAddr, static_cast<int>(toAddress.GetSize()));
  if (error != 0) {
    SocketUtil::ReportError("UDPSocket::Bind");
    return SocketUtil::GetLastError();
  }

  return NO_ERROR;
}

int UDPSocket::SendTo(const void* toSend, int length, const SocketAddress& toAddress) const {
  int byteSentCount = sendto(mSocket, static_cast<const char*>(toSend), length, 0, &toAddress.sockAddr,
                             static_cast<int>(toAddress.GetSize()));
  if (byteSentCount <= 0) {
    SocketUtil::ReportError("UDPSocket::SendTo");
    return -SocketUtil::GetLastError();
  } else {
    return byteSentCount;
  }
}

int UDPSocket::ReceiveFrom(void* inToReceive, int inMaxLength, SocketAddress& outFromAddress) const {
  auto fromLength = static_cast<socklen_t>(outFromAddress.GetSize());

  int readByteCount =
      recvfrom(mSocket, static_cast<char*>(inToReceive), inMaxLength, 0, &outFromAddress.sockAddr, &fromLength);
  if (readByteCount >= 0) {
    return readByteCount;
  } else {
    int error = SocketUtil::GetLastError();

    if (error == WSAEWOULDBLOCK) {
      return 0;
    } else if (error == WSAECONNRESET) {
      spdlog::info("Connection reset from {}", outFromAddress.ToString().c_str());
      return -WSAECONNRESET;
    } else {
      SocketUtil::ReportError("UDPSocket::ReceiveFrom");
      return -error;
    }
  }
}

UDPSocket::~UDPSocket() {
#if defined(_WIN32)
  closesocket(mSocket);
#else
  close(mSocket);
#endif
}

int UDPSocket::SetNonBlockingMode(bool shouldBeNonBlocking) const {
#if defined(_WIN32)
  u_long arg = shouldBeNonBlocking ? 1 : 0;
  int result = ioctlsocket(mSocket, static_cast<long>(FIONBIO), &arg);
#else
  int flags = fcntl(mSocket, F_GETFL, 0);
  flags = shouldBeNonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
  int result = fcntl(mSocket, F_SETFL, flags);
#endif

  if (result == SOCKET_ERROR) {
    SocketUtil::ReportError("UDPSocket::SetNonBlockingMode");
    return SocketUtil::GetLastError();
  } else {
    return NO_ERROR;
  }
}
