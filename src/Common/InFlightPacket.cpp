#include "InFlightPacket.h"

#include "Clock.h"

InFlightPacket::InFlightPacket(PacketSequenceNumber inSequenceNumber)
    : sequenceNumber(inSequenceNumber), timeDispatched(Clock::Instance.GetTimeF()) {}

void InFlightPacket::HandleDeliveryFailure(DeliveryNotificationManager* deliveryNotificationManager) const {
  for (const auto& pair : transmissionDataMap) {
    pair.second->HandleDeliveryFailure(deliveryNotificationManager);
  }
}

void InFlightPacket::HandleDeliverySuccess(DeliveryNotificationManager* deliveryNotificationManager) const {
  for (const auto& pair : transmissionDataMap) {
    pair.second->HandleDeliverySuccess(deliveryNotificationManager);
  }
}
