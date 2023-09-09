#ifndef ROBO_CLIENTNETWORKMANAGER_H
#define ROBO_CLIENTNETWORKMANAGER_H

#include "NetworkManager.h"
#include "SocketAddress.h"

class ClientNetworkManager : public NetworkManager {
  enum NetworkClientState { NCS_Uninitialized, NCS_SayingHello, NCS_Welcomed };

 public:
  static ClientNetworkManager* Instance;

  static void StaticInit(const SocketAddress& serverAddress, const std::string& name);

  void SendOutgoingPackets();
  void ProcessPacket(InputMemoryBitStream& inputStream, const SocketAddress& fromAddress) override;
  void SendWinPacket();

  [[nodiscard]] int GetPlayerId() const {
    return playerId;
  }

 private:
  ClientNetworkManager();
  void Init(const SocketAddress& serverAddress, const std::string& name);

  void SendHelloPacket();

  void HandleGameOverPacket(InputMemoryBitStream& inputStream) const;
  void HandleWelcomePacket(InputMemoryBitStream& inputStream);

  NetworkClientState state;
  SocketAddress serverAddress;
  int playerId;
  std::string name;
};

#endif  // ROBO_CLIENTNETWORKMANAGER_H
