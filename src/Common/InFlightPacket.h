#ifndef ROBO_INFLIGHTPACKET_H
#define ROBO_INFLIGHTPACKET_H

#include <cstdint>
#include <unordered_map>
#include <utility>

#include "TransmissionData.h"

class DeliveryNotificationManager;
typedef uint16_t PacketSequenceNumber;

class InFlightPacket {
 public:
  InFlightPacket(PacketSequenceNumber inSequenceNumber);

  [[nodiscard]] PacketSequenceNumber GetSequenceNumber() const {
    return sequenceNumber;
  }

  [[nodiscard]] float GetTimeDispatched() const {
    return timeDispatched;
  }

  void SetTransmissionData(int key, TransmissionDataPtr transmissionData) {
    transmissionDataMap[key] = std::move(transmissionData);
  }

  [[nodiscard]] TransmissionDataPtr GetTransmissionData(int key) const {
    auto it = transmissionDataMap.find(key);
    return (it != transmissionDataMap.end()) ? it->second : nullptr;
  }

  void HandleDeliveryFailure(DeliveryNotificationManager* deliveryNotificationManager) const;
  void HandleDeliverySuccess(DeliveryNotificationManager* deliveryNotificationManager) const;

 private:
  PacketSequenceNumber sequenceNumber;
  float timeDispatched;

  std::unordered_map<int, TransmissionDataPtr> transmissionDataMap;
};

#endif  // ROBO_INFLIGHTPACKET_H
