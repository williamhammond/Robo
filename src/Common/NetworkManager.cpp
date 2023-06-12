#include "NetworkManager.h"

#include <spdlog/spdlog.h>

#include "RoboMath.h"
#include "SocketUtil.h"

NetworkManager::NetworkManager() : bytesSentThisFrame(0), packetDropChance(0.f), simulatedLatency(0.f) {}

NetworkManager::~NetworkManager() = default;

bool NetworkManager::Init(uint16_t inPort) {
  udpSocket = SocketUtil::CreateUDPSocket(INET);
  SocketAddress ownAddress(INADDR_ANY, inPort);
  if (udpSocket->Bind(ownAddress) != NO_ERROR) {
    return false;
  }

  spdlog::info("Initializing NetworkManager at port {}", inPort);

  bytesReceivedPerSecond = WeightedTimedMovingAverage(1.f);
  bytesSentPerSecond = WeightedTimedMovingAverage(1.f);

  if (udpSocket == nullptr) {
    return false;
  }

  if (udpSocket->SetNonBlockingMode(true) != NO_ERROR) {
    return false;
  }

  return true;
}

void NetworkManager::ProcessIncomingPackets() {
  ReadIncomingPacketsIntoQueue();
  ProcessQueuedPackets();
  UpdateBytesSentLastFrame();
}

void NetworkManager::ReadIncomingPacketsIntoQueue() {
  char packetMem[1500];
  uint32_t packetSize = sizeof(packetMem);
  InputMemoryBitStream inputStream(packetMem, packetSize * 8);
  SocketAddress fromAddress;

  int receivedPackedCount = 0;
  int totalReadBytes = 0;

  while (receivedPackedCount < kMaxPacketsPerFrameCount) {
    int readByteCount = udpSocket->ReceiveFrom(packetMem, static_cast<int>(packetSize), fromAddress);
    if (readByteCount == 0) {
      break;
    } else if (readByteCount == -WSAECONNRESET) {
      HandleConnectionReset(fromAddress);
    } else if (readByteCount > 0) {
      inputStream.ResetToCapacity(static_cast<uint32_t>(readByteCount));
      receivedPackedCount++;
      totalReadBytes += readByteCount;

      if (RoboMath::GetRandomFloat() >= packetDropChance) {
        float simulatedReceivedTime = Clock::Instance.GetTimeF() + simulatedLatency;
        packetQueue.emplace(simulatedReceivedTime, inputStream, fromAddress);
      } else {
        spdlog::warn("Packet dropped");
      }
    } else {
      // TODO handle error
    }
  }

  if (totalReadBytes > 0) {
    bytesReceivedPerSecond.UpdatePerSecond(static_cast<float>(totalReadBytes));
  }
}

void NetworkManager::ProcessQueuedPackets() {
  while (!packetQueue.empty()) {
    ReceivedPacket& nextPacket = packetQueue.front();
    if (Clock::Instance.GetTimeF() > nextPacket.GetReceivedTime()) {
      ProcessPacket(nextPacket.GetPacketBuffer(), nextPacket.GetFromAddress());
      packetQueue.pop();
    } else {
      break;
    }
  }
}

void NetworkManager::SendPacket(const OutputMemoryBitStream& inOutputStream, const SocketAddress& inFromAddress) {
  int sentByteCount =
      udpSocket->SendTo(inOutputStream.GetBufferPtr(), static_cast<int>(inOutputStream.GetByteLength()), inFromAddress);
  if (sentByteCount > 0) {
    bytesSentThisFrame += sentByteCount;
  }
}

void NetworkManager::UpdateBytesSentLastFrame() {
  if (bytesSentThisFrame > 0) {
    bytesSentPerSecond.UpdatePerSecond(static_cast<float>(bytesSentThisFrame));

    bytesSentThisFrame = 0;
  }
}

NetworkManager::ReceivedPacket::ReceivedPacket(float inReceivedTime, InputMemoryBitStream& ioInputMemoryBitStream,
                                               const SocketAddress& inFromAddress)
    : receivedTime(inReceivedTime), packetBuffer(ioInputMemoryBitStream), fromAddress(inFromAddress) {}

GameObjectPtr NetworkManager::GetGameObject(uint32_t networkId) const {
  auto gameObjectIt = networkIdToGameObject.find(networkId);
  if (gameObjectIt != networkIdToGameObject.end()) {
    return gameObjectIt->second;
  }

  return {};
}