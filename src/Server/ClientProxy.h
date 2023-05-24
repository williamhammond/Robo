#ifndef ROBO_CLIENTPROXY_H
#define ROBO_CLIENTPROXY_H

#include <SocketAddress.h>

#include <memory>

class ClientProxy {
 public:
  ClientProxy(const SocketAddress& inSocketAddress, const std::string& inName, int inPlayerId);
  void UpdateLastPacketTime();

  [[nodiscard]] int GetPlayerId() const {
    return playerId;
  }

  [[nodiscard]] std::string GetName() const {
    return name;
  }

  [[nodiscard]] SocketAddress GetSocketAddress() const {
    return socketAddress;
  }

  void SetIsLastMoveTimestampDirty(bool isDirty) {
    isLastMoveTimestampDirty = isDirty;
  }

  [[nodiscard]] bool IsLastMoveTimestampDirty() const {
    return isLastMoveTimestampDirty;
  }

 private:
  int playerId;
  std::string name;
  SocketAddress socketAddress;
  float lastPacketTime;
  bool isLastMoveTimestampDirty;
};
typedef std::shared_ptr<ClientProxy> ClientProxyPtr;

#endif  // ROBO_CLIENTPROXY_H
