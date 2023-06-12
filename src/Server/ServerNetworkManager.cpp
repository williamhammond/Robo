#include "ServerNetworkManager.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <memory>

#include "NetworkIds.h"
#include "NetworkManager.h"
#include "ReplicationManagerTransmissionData.h"
#include "SDL.h"
#include "Server.h"

ServerNetworkManager* ServerNetworkManager::Instance;

bool ServerNetworkManager::StaticInit(uint16_t port) {
  Instance = new ServerNetworkManager();
  return Instance->Init(port);
}

ServerNetworkManager::ServerNetworkManager() {
  NewPlayerId = 0;
}

void ServerNetworkManager::ProcessPacket(InputMemoryBitStream& inputStream, const SocketAddress& fromAddress) {
  auto it = addressToClient.find(fromAddress);
  if (it == addressToClient.end()) {
    HandlePacketFromNewClient(inputStream, fromAddress);
  } else {
    ProcessPacket((*it).second, inputStream);
  }
}

void ServerNetworkManager::ProcessPacket(const ClientProxyPtr& clientProxy, InputMemoryBitStream& inputStream) {
  clientProxy->UpdateLastPacketTime();

  PacketType packetType;
  inputStream.Read(packetType);
  switch (packetType) {
    case PacketType::HelloPacketId:
      SendWelcomePacket(clientProxy);
      break;
    case PacketType::WinPacketId:
      HandleWinPacket(clientProxy);
      break;
    default:
      spdlog::error("Unknown packet type received from {}", clientProxy->GetSocketAddress().ToString().c_str());
      break;
  }
}

void ServerNetworkManager::HandlePacketFromNewClient(InputMemoryBitStream& inputStream,
                                                     const SocketAddress& fromAddress) {
  PacketType packetType;
  inputStream.Read(packetType);
  if (packetType == PacketType::HelloPacketId) {
    std::string name;
    inputStream.Read(name);
    ClientProxyPtr newClientProxy = std::make_shared<ClientProxy>(fromAddress, name, NewPlayerId++);
    addressToClient[fromAddress] = newClientProxy;
    playerIdToClient[newClientProxy->GetPlayerId()] = newClientProxy;

    // TODO fix casting nonsense here
    dynamic_cast<Server*>(Engine::Instance.get())->HandleNewClient(newClientProxy);

    SendWelcomePacket(newClientProxy);
  } else {
    spdlog::error("Bad incoming packet from unknown client at socket {}", fromAddress.ToString().c_str());
  }
}

void ServerNetworkManager::SendWelcomePacket(const ClientProxyPtr& clientProxy) {
  OutputMemoryBitStream welcomePacket;

  welcomePacket.Write(PacketType::WelcomePacketId);
  welcomePacket.Write(clientProxy->GetPlayerId());

  spdlog::info("Client '{}' connected as player {}", clientProxy->GetName().c_str(), clientProxy->GetPlayerId());

  SendPacket(welcomePacket, clientProxy->GetSocketAddress());
}

void ServerNetworkManager::HandleWinPacket([[maybe_unused]] const ClientProxyPtr& clientProxy) {
  // TODO put this in it's own class
  SDL_Event quitEvent;
  quitEvent.type = SDL_QUIT;
  if (SDL_PushEvent(&quitEvent) < 0) {
    SDL_Log("Failed to Push the quit event: %s", SDL_GetError());
  }
}

void ServerNetworkManager::SendOutgoingPackets() {
  for (auto& it : addressToClient) {
    ClientProxyPtr clientProxy = it.second;
    clientProxy->GetDeliveryNotificationManager().ProcessTimedOutPackets();
    if (clientProxy->IsLastMoveTimestampDirty()) {
      SendStatePacketToClient(clientProxy);
    }
  }
}

void ServerNetworkManager::SendStatePacketToClient([[maybe_unused]] const ClientProxyPtr& clientProxy) {
  OutputMemoryBitStream statePacket;
  statePacket.Write(PacketType::StatePacketId);

  InFlightPacket* inFlightPacket = clientProxy->GetDeliveryNotificationManager().WriteState(statePacket);

  auto* transmissionData = new ReplicationManagerTransmissionData(&clientProxy->GetReplicationManagerServer());
  clientProxy->GetReplicationManagerServer().Write(statePacket, transmissionData);
  // TODO fix key
  inFlightPacket->SetTransmissionData(0, TransmissionDataPtr(transmissionData));

  SendPacket(statePacket, clientProxy->GetSocketAddress());
}

void ServerNetworkManager::CheckForDisconnects() {}
