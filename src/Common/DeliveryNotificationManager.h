#ifndef ROBO_DELIVERYNOTIFICATIONMANAGER_H
#define ROBO_DELIVERYNOTIFICATIONMANAGER_H

#include <deque>

#include "AckRange.h"
#include "InFlightPacket.h"
#include "MemoryBitStream.h"

class DeliveryNotificationManager {
 public:
  DeliveryNotificationManager(bool inShouldSendAcks, bool inShouldProcessAcks);
  ~DeliveryNotificationManager();

  inline InFlightPacket* WriteState(OutputMemoryBitStream& inOutputStream);
  inline bool ReadAndProcessState(InputMemoryBitStream& inInputStream);

  void ProcessTimedOutPackets();

  [[nodiscard]] uint32_t GetDroppedPacketCount() const {
    return droppedPacketCount;
  }

  [[nodiscard]] uint32_t GetDeliveredPacketCount() const {
    return deliveredPacketCount;
  }

  [[nodiscard]] uint32_t GetDispatchedPacketCount() const {
    return dispatchedPacketCount;
  }

  [[nodiscard]] const std::deque<InFlightPacket>& GetInFlightPackets() const {
    return inFlightPackets;
  }

 private:
  InFlightPacket* WriteSequenceNumber(OutputMemoryBitStream& inOutputStream);
  void WriteAckData(OutputMemoryBitStream& inOutputStream);

  bool ProcessSequenceNumber(InputMemoryBitStream& inInputStream);
  void ProcessAcks(InputMemoryBitStream& inInputStream);

  void AddPendingAck(PacketSequenceNumber inSequenceNumber);
  void HandlePacketDeliveryFailure(const InFlightPacket& inFlightPacket);
  void HandlePacketDeliverySuccess(const InFlightPacket& inFlightPacket);

  PacketSequenceNumber nextOutgoingSequenceNumber;
  PacketSequenceNumber nextExpectedSequenceNumber;

  std::deque<InFlightPacket> inFlightPackets;
  std::deque<AckRange> pendingAcks;

  bool shouldSendAcks;
  bool shouldProcessAcks;

  uint32_t deliveredPacketCount;
  uint32_t droppedPacketCount;
  uint32_t dispatchedPacketCount;
};

inline InFlightPacket* DeliveryNotificationManager::WriteState(OutputMemoryBitStream& inOutputStream) {
  InFlightPacket* toRet = WriteSequenceNumber(inOutputStream);
  if (shouldSendAcks) {
    WriteAckData(inOutputStream);
  }

  return toRet;
}

inline bool DeliveryNotificationManager::ReadAndProcessState(InputMemoryBitStream& inInputStream) {
  bool result = ProcessSequenceNumber(inInputStream);
  if (shouldProcessAcks) {
    ProcessAcks(inInputStream);
  }

  return result;
}

#endif  // ROBO_DELIVERYNOTIFICATIONMANAGER_H
