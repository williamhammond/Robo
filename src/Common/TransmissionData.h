#ifndef ROBO_TRANSMISSIONDATA_H
#define ROBO_TRANSMISSIONDATA_H

#include <memory>

class DeliveryNotificationManager;

class TransmissionData {
 public:
  virtual void HandleDeliveryFailure(DeliveryNotificationManager* inDeliveryNotificationManager) const = 0;
  virtual void HandleDeliverySuccess(DeliveryNotificationManager* inDeliveryNotificationManager) const = 0;
};
typedef std::shared_ptr<TransmissionData> TransmissionDataPtr;

#endif  // ROBO_TRANSMISSIONDATA_H
