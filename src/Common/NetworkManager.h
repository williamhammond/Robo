#ifndef ROBO_NETWORKMANAGER_H
#define ROBO_NETWORKMANAGER_H

#include <list>
#include <map>
#include <queue>
#include <unordered_map>

#include "GameObject.h"
#include "MemoryBitStream.h"
#include "SocketAddress.h"
#include "UDPSocket.h"
#include "WeightedTimedMovingAveraged.h"

typedef std::unordered_map<int, GameObjectPtr> IntToGameObjectMap;

class NetworkManager {
 public:
  static const int kMaxPacketsPerFrameCount = 10;

  NetworkManager();
  virtual ~NetworkManager();

  bool Init(uint16_t inPort);
  void ProcessIncomingPackets();

  virtual void ProcessPacket(InputMemoryBitStream& inInputStream, const SocketAddress& inFromAddress) = 0;
  virtual void HandleConnectionReset(const SocketAddress& inFromAddress) {
    (void)inFromAddress;
  }

  void SendPacket(const OutputMemoryBitStream& inOutputStream, const SocketAddress& inFromAddress);

  [[nodiscard]] const WeightedTimedMovingAverage& GetBytesReceivedPerSecond() const {
    return bytesReceivedPerSecond;
  }
  [[nodiscard]] const WeightedTimedMovingAverage& GetBytesSentPerSecond() const {
    return bytesSentPerSecond;
  }

  void SetDropPacketChance(float inChance) {
    packetDropChance = inChance;
  }
  void SetSimulatedLatency(float inLatency) {
    simulatedLatency = inLatency;
  }

  GameObjectPtr GetGameObject(uint32_t networkId) const;

 protected:
  IntToGameObjectMap NetworkIdToGameObject;

 private:
  class ReceivedPacket {
   public:
    ReceivedPacket(float inReceivedTime, InputMemoryBitStream& inInputMemoryBitStream, const SocketAddress& inAddress);

    [[nodiscard]] const SocketAddress& GetFromAddress() const {
      return fromAddress;
    }

    [[nodiscard]] float GetReceivedTime() const {
      return receivedTime;
    }

    InputMemoryBitStream& GetPacketBuffer() {
      return packetBuffer;
    }

   private:
    float receivedTime;
    InputMemoryBitStream packetBuffer;
    SocketAddress fromAddress;
  };

  void UpdateBytesSentLastFrame();
  void ReadIncomingPacketsIntoQueue();
  void ProcessQueuedPackets();

  std::queue<ReceivedPacket, std::list<ReceivedPacket>> packetQueue;

  std::map<uint32_t, GameObjectPtr> networkIdToGameObject;

  UDPSocketPtr udpSocket;

  WeightedTimedMovingAverage bytesReceivedPerSecond;
  WeightedTimedMovingAverage bytesSentPerSecond;

  int bytesSentThisFrame;

  float packetDropChance;
  float simulatedLatency;
};

#endif  // ROBO_NETWORKMANAGER_H
