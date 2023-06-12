#ifndef ROBO_REPLICATIONMANAGERTRANSMISSIONDATA_H
#define ROBO_REPLICATIONMANAGERTRANSMISSIONDATA_H

#include <memory>
#include <vector>

#include "DeliveryNotificationManager.h"
#include "ReplicationManagerServer.h"

class ReplicationManagerServer;

class ReplicationManagerTransmissionData : public TransmissionData {
 public:
  explicit ReplicationManagerTransmissionData(ReplicationManagerServer* inReplicationManagerServer)
      : replicationManagerServer(inReplicationManagerServer) {}

  class ReplicationTransmission {
   public:
    ReplicationTransmission(int inNetworkId, ReplicationAction inAction, uint32_t inState)
        : networkId(inNetworkId), action(inAction), state(inState) {}

    [[nodiscard]] int GetNetworkId() const {
      return networkId;
    }

    [[nodiscard]] ReplicationAction GetAction() const {
      return action;
    }

    [[nodiscard]] uint32_t GetState() const {
      return state;
    }

   private:
    int networkId;
    ReplicationAction action;
    uint32_t state;
  };

  void AddTransmission(int inNetworkId, ReplicationAction inAction, uint32_t inState);

  virtual void HandleDeliveryFailure(DeliveryNotificationManager* deliveryNotificationManager) const override;
  virtual void HandleDeliverySuccess(DeliveryNotificationManager* deliveryNotificationManager) const override;

 private:
  void HandleCreateDeliveryFailure(int networkId) const;
  void HandleUpdateStateDeliveryFailure(int networkId, uint32_t state,
                                        DeliveryNotificationManager* deliveryNotificationManager) const;
  void HandleDestroyDeliveryFailure(int networkId) const;
  void HandleCreateDeliverySuccess(int network) const;
  void HandleDestroyDeliverySuccess(int networkId) const;

  ReplicationManagerServer* replicationManagerServer;

  std::vector<ReplicationTransmission> transmissions;
};

typedef std::shared_ptr<ReplicationManagerTransmissionData> ReplicationManagerTransmissionDataPtr;
#endif  // ROBO_REPLICATIONMANAGERTRANSMISSIONDATA_H
