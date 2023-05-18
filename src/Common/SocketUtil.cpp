#include "SocketUtil.h"

#include <spdlog/spdlog.h>

#include <algorithm>

#include "TCPSocket.h"

#if !defined(_WIN32)
#include <sys/select.h>
#endif

bool SocketUtil::StaticInit() {
#if defined(_WIN32)
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != NO_ERROR) {
    ReportError("Starting Up");
    return false;
  }
#endif
  return true;
}

void SocketUtil::CleanUp() {
#if defined(_WIN32)
  WSACleanup();
#endif
}

void SocketUtil::ReportError([[maybe_unused]] const char* inOperationDesc) {
#if defined(_WIN32)
  LPVOID lpMsgBuf;
  auto errorNum = static_cast<DWORD>(GetLastError());

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
                errorNum, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

  spdlog::error("Error {}: {}-{}", inOperationDesc, errorNum, lpMsgBuf);
#else
  spdlog::error("Error: {}", inOperationDesc);
#endif
}

int SocketUtil::GetLastError() {
#if defined(_WIN32)
  return WSAGetLastError();
#else
  return errno;
#endif
}

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily family) {
  SOCKET s = socket(family, SOCK_DGRAM, IPPROTO_UDP);

  if (s != INVALID_SOCKET) {
    return UDPSocketPtr(new UDPSocket(s));
  } else {
    ReportError("SocketUtil::CreateUDPSocket");
    return nullptr;
  }
}

TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily family) {
  SOCKET s = socket(family, SOCK_STREAM, IPPROTO_TCP);

  if (s != INVALID_SOCKET) {
    return TCPSocketPtr(new TCPSocket(s));
  } else {
    ReportError("SocketUtil::CreateTCPSocket");
    return nullptr;
  }
}

fd_set* SocketUtil::FillSetFromVector(fd_set& outSet, const std::vector<TCPSocketPtr>* sockets,
                                      [[maybe_unused]] int& maxNfds) {
  if (sockets) {
    FD_ZERO(&outSet);

    for (const TCPSocketPtr& socket : *sockets) {
      FD_SET(socket->mSocket, &outSet);
    }
    return &outSet;
  } else {
    return nullptr;
  }
}

void SocketUtil::FillVectorFromSet(std::vector<TCPSocketPtr>* outSockets, const std::vector<TCPSocketPtr>* inSockets,
                                   const fd_set& inSet) {
  if (inSockets && outSockets) {
    outSockets->clear();
    for (const TCPSocketPtr& socket : *inSockets) {
      if (FD_ISSET(socket->mSocket, &inSet)) {
        outSockets->push_back(socket);
      }
    }
  }
}

int SocketUtil::Select(const std::vector<TCPSocketPtr>* inReadSet, std::vector<TCPSocketPtr>* outReadSet,
                       const std::vector<TCPSocketPtr>* inWriteSet, std::vector<TCPSocketPtr>* outWriteSet,
                       const std::vector<TCPSocketPtr>* inExceptSet, std::vector<TCPSocketPtr>* outExceptSet) {
  fd_set read, write, except;

  int nfds = 0;

  fd_set* readPtr = FillSetFromVector(read, inReadSet, nfds);
  fd_set* writePtr = FillSetFromVector(read, inWriteSet, nfds);
  fd_set* exceptPtr = FillSetFromVector(read, inExceptSet, nfds);

  int toRet = select(nfds + 1, readPtr, writePtr, exceptPtr, nullptr);

  if (toRet > 0) {
    FillVectorFromSet(outReadSet, inReadSet, read);
    FillVectorFromSet(outWriteSet, inWriteSet, write);
    FillVectorFromSet(outExceptSet, inExceptSet, except);
  }
  return toRet;
}
