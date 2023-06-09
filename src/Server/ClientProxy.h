#ifndef ROBO_CLIENTPROXY_H
#define ROBO_CLIENTPROXY_H

#include <SocketAddress.h>

#include <memory>

#include "DeliveryNotificationManager.h"

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

  DeliveryNotificationManager& GetDeliveryNotificationManager() {
    return deliveryNotificationManager;
  }

 private:
  int playerId;
  std::string name;
  SocketAddress socketAddress;
  float lastPacketTime;
  bool isLastMoveTimestampDirty{};
  DeliveryNotificationManager deliveryNotificationManager;
};
typedef std::shared_ptr<ClientProxy> ClientProxyPtr;

#endif  // ROBO_CLIENTPROXY_H
