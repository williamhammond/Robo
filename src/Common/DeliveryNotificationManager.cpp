#include "DeliveryNotificationManager.h"

#include "Clock.h"
#include "spdlog/spdlog.h"

namespace {
const float kDelayBeforeAckTimeout = 0.5f;
}

DeliveryNotificationManager::DeliveryNotificationManager(bool inShouldSendAcks, bool inShouldProcessAcks)
    : nextOutgoingSequenceNumber(0),
      nextExpectedSequenceNumber(0),
      shouldSendAcks(inShouldSendAcks),
      shouldProcessAcks(inShouldProcessAcks),
      deliveredPacketCount(0),
      droppedPacketCount(0),
      dispatchedPacketCount(0) {}

DeliveryNotificationManager::~DeliveryNotificationManager() {
  spdlog::info("DeliveryNotificationManager destructor. Delivery rate %d%%, Drop rate %d%%",
               (100 * deliveredPacketCount) / dispatchedPacketCount,
               (100 * droppedPacketCount) / dispatchedPacketCount);
}

InFlightPacket* DeliveryNotificationManager::WriteSequenceNumber(OutputMemoryBitStream& inOutputStream) {
  PacketSequenceNumber sequenceNumber = nextOutgoingSequenceNumber++;
  inOutputStream.Write(sequenceNumber);

  ++dispatchedPacketCount;

  if (shouldProcessAcks) {
    inFlightPackets.emplace_back(sequenceNumber);

    return &inFlightPackets.back();
  } else {
    return nullptr;
  }
}

void DeliveryNotificationManager::WriteAckData(OutputMemoryBitStream& inOutputStream) {
  bool hasAcks = (pendingAcks.size() > 0);

  inOutputStream.Write(hasAcks);
  if (hasAcks) {
    pendingAcks.front().Write(inOutputStream);
    pendingAcks.pop_front();
  }
}

bool DeliveryNotificationManager::ProcessSequenceNumber(InputMemoryBitStream& inInputStream) {
  PacketSequenceNumber sequenceNumber;

  inInputStream.Read(sequenceNumber);
  if (sequenceNumber == nextExpectedSequenceNumber) {
    nextExpectedSequenceNumber = sequenceNumber + 1;
    if (shouldSendAcks) {
      AddPendingAck(sequenceNumber);
    }
    return true;
  } else if (sequenceNumber < nextExpectedSequenceNumber) {
    return false;
  } else if (sequenceNumber > nextExpectedSequenceNumber) {
    nextExpectedSequenceNumber = sequenceNumber + 1;
    if (shouldSendAcks) {
      AddPendingAck(sequenceNumber);
    }
    return true;
  }

  return false;
}

void DeliveryNotificationManager::ProcessAcks(InputMemoryBitStream& inInputStream) {
  bool hasAcks;
  inInputStream.Read(hasAcks);
  if (hasAcks) {
    AckRange ackRange;
    ackRange.Read(inInputStream);

    PacketSequenceNumber nextAckdSequenceNumber = ackRange.GetStart();
    uint32_t onePastAckdSequenceNumber = nextAckdSequenceNumber + ackRange.GetCount();
    while (nextAckdSequenceNumber < onePastAckdSequenceNumber && !inFlightPackets.empty()) {
      const auto& nextInFlightPacket = inFlightPackets.front();
      PacketSequenceNumber nextInFlightPacketSequenceNumber = nextInFlightPacket.GetSequenceNumber();
      if (nextInFlightPacketSequenceNumber < nextAckdSequenceNumber) {
        auto copyOfInFlightPacket = nextInFlightPacket;
        inFlightPackets.pop_front();
        HandlePacketDeliveryFailure(copyOfInFlightPacket);
      } else if (nextInFlightPacketSequenceNumber == nextAckdSequenceNumber) {
        HandlePacketDeliverySuccess(nextInFlightPacket);
        inFlightPackets.pop_front();
        ++nextAckdSequenceNumber;
      } else if (nextInFlightPacketSequenceNumber > nextAckdSequenceNumber) {
        ++nextAckdSequenceNumber;
      }
    }
  }
}

void DeliveryNotificationManager::ProcessTimedOutPackets() {
  float timeoutTime = Clock::Instance.GetTimeF() - kDelayBeforeAckTimeout;

  while (!inFlightPackets.empty()) {
    const auto& nextInFlightPacket = inFlightPackets.front();

    if (nextInFlightPacket.GetTimeDispatched() < timeoutTime) {
      HandlePacketDeliveryFailure(nextInFlightPacket);
      inFlightPackets.pop_front();
    } else {
      break;
    }
  }
}

void DeliveryNotificationManager::AddPendingAck(PacketSequenceNumber inSequenceNumber) {
  if (pendingAcks.size() == 0 || !pendingAcks.back().ExtendIfShould(inSequenceNumber)) {
    pendingAcks.emplace_back(inSequenceNumber);
  }
}

void DeliveryNotificationManager::HandlePacketDeliveryFailure(const InFlightPacket& inFlightPacket) {
  ++droppedPacketCount;
  inFlightPacket.HandleDeliveryFailure(this);
}

void DeliveryNotificationManager::HandlePacketDeliverySuccess(const InFlightPacket& inFlightPacket) {
  ++deliveredPacketCount;
  inFlightPacket.HandleDeliverySuccess(this);
}