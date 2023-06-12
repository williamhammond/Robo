#include "ClientNetworkManager.h"

#include <spdlog/spdlog.h>

#include "NetworkIds.h"

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
      SendWinPacket();
      break;
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
    case PacketType::HelloPacketId:
    case PacketType::WinPacketId:
    case PacketType::StatePacketId:
      break;
  }
}

void ClientNetworkManager::HandleWelcomePacket(InputMemoryBitStream& inputStream) {
  if (state == NCS_SayingHello) {
    inputStream.Read(playerId);
    state = NCS_Welcomed;
    spdlog::info("'{}' was welcomed on client as player {}", name.c_str(), playerId);
  }
}

void ClientNetworkManager::UpdateSayingHello() {}

void ClientNetworkManager::SendHelloPacket() {
  OutputMemoryBitStream helloPacket;

  helloPacket.Write(PacketType::HelloPacketId);
  helloPacket.Write(name);

  SendPacket(helloPacket, serverAddress);
}

void ClientNetworkManager::SendWinPacket() {
  OutputMemoryBitStream winPacket;

  winPacket.Write(PacketType::WinPacketId);
  winPacket.Write(name);

  SendPacket(winPacket, serverAddress);
}
