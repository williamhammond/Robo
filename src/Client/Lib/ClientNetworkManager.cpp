#include "ClientNetworkManager.h"

#include "Client.h"
#include "NetworkIds.h"
#include "spdlog/spdlog.h"

ClientNetworkManager* ClientNetworkManager::Instance;

void ClientNetworkManager::StaticInit(const SocketAddress& serverAddress, const std::string& name) {
  Instance = new ClientNetworkManager();
  return Instance->Init(serverAddress, name);
}

void ClientNetworkManager::Init(const SocketAddress& inServerAddress, const std::string& inName) {
  NetworkManager::Init(0);
  serverAddress = inServerAddress;
  name = inName;
  state = NCS_SayingHello;
}

ClientNetworkManager::ClientNetworkManager() : state(NCS_Uninitialized), playerId(-1) {}

void ClientNetworkManager::SendOutgoingPackets() {
  switch (state) {
    case NCS_SayingHello:
      SendHelloPacket();
      break;
    case NCS_Welcomed:
    case NCS_Uninitialized:
      break;
  }
}

void ClientNetworkManager::ProcessPacket(InputMemoryBitStream& inputStream,
                                         [[maybe_unused]] const SocketAddress& fromAddress) {
  PacketType packetType;
  inputStream.Read(packetType);
  switch (packetType) {
    case PacketType::WelcomePacketId:
      HandleWelcomePacket(inputStream);
      break;
    case PacketType::WinPacketId:
      HandleGameOverPacket(inputStream);
      Client::Instance->SetQuit(true);
      break;
    case PacketType::HelloPacketId:
    case PacketType::StatePacketId:
      break;
  }
}

void ClientNetworkManager::SendHelloPacket() {
  OutputMemoryBitStream helloPacket;

  helloPacket.Write(PacketType::HelloPacketId);
  helloPacket.Write(name);

  SendPacket(helloPacket, serverAddress);
}

void ClientNetworkManager::SendWinPacket() {
  OutputMemoryBitStream winPacket;

  winPacket.Write(PacketType::WinPacketId);
  winPacket.Write(playerId);

  spdlog::info("Sending win packet");
  SendPacket(winPacket, serverAddress);
}

void ClientNetworkManager::HandleWelcomePacket(InputMemoryBitStream& inputStream) {
  if (state == NCS_SayingHello) {
    inputStream.Read(playerId);
    state = NCS_Welcomed;
    spdlog::info("'{}' was welcomed on client as player {}", name.c_str(), playerId);
  }
}

void ClientNetworkManager::HandleGameOverPacket(InputMemoryBitStream& inputStream) const {
  int winnerId;
  inputStream.Read(winnerId);
  if (winnerId == playerId) {
    spdlog::info("You won!");
  } else {
    spdlog::info("You lost! '{}' won!", winnerId);
  }
}
