#ifndef ROBO_SERVERNETWORKMANAGER_H
#define ROBO_SERVERNETWORKMANAGER_H

#include <unordered_map>

#include "ClientProxy.h"
#include "NetworkManager.h"

class ServerNetworkManager : public NetworkManager {
 public:
  static ServerNetworkManager* Instance;

  static bool StaticInit(uint16_t port);
  void ProcessPacket(InputMemoryBitStream& inputStream, const SocketAddress& fromAddress) override;
  void SendOutgoingPackets();
  void CheckForDisconnects();

 private:
  ServerNetworkManager();
  void HandlePacketFromNewClient(InputMemoryBitStream& inputStream, const SocketAddress& fromAddress);
  void ProcessPacket(const ClientProxyPtr& clientProxy, InputMemoryBitStream& inputStream);
  void SendWelcomePacket(const ClientProxyPtr& clientProxy);
  void SendStatePacketToClient(const ClientProxyPtr& clientProxy);
  static void HandleWinPacket(const ClientProxyPtr& clientProxy);

  std::unordered_map<int, ClientProxyPtr> playerIdToClient;
  std::unordered_map<SocketAddress, ClientProxyPtr> addressToClient;
  int NewPlayerId;
};

#endif  // ROBO_SERVERNETWORKMANAGER_H
