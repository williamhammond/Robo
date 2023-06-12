#include "ReplicationManagerTransmissionData.h"

#include "GameObject.h"
#include "ServerNetworkManager.h"

void ReplicationManagerTransmissionData::AddTransmission(int inNetworkId, ReplicationAction inAction,
                                                         uint32_t inState) {
  /*
  //it would be silly if we already had a transmission for this network id in here...
  for( const auto& transmission: transmissions )
  {
          assert( inNetworkId != transmission.GetNetworkId() );
  }
  */
  transmissions.emplace_back(inNetworkId, inAction, inState);
}

void ReplicationManagerTransmissionData::HandleDeliveryFailure(
    DeliveryNotificationManager* deliveryNotificationManager) const {
  for (const ReplicationTransmission& replicationTransmission : transmissions) {
    int networkId = replicationTransmission.GetNetworkId();

    switch (replicationTransmission.GetAction()) {
      case RA_Create:
        HandleCreateDeliveryFailure(networkId);
        break;
      case RA_Update:
        HandleUpdateStateDeliveryFailure(networkId, replicationTransmission.GetState(), deliveryNotificationManager);
        break;
      case RA_Destroy:
        HandleDestroyDeliveryFailure(networkId);
        break;
      case RA_RPC:
      case RA_MAX:
        break;
    }
  }
}

void ReplicationManagerTransmissionData::HandleDeliverySuccess(
    [[maybe_unused]] DeliveryNotificationManager* deliveryNotificationManager) const {
  for (const ReplicationTransmission& replicationTransmission : transmissions) {
    switch (replicationTransmission.GetAction()) {
      case RA_Create:
        HandleCreateDeliverySuccess(replicationTransmission.GetNetworkId());
        break;
      case RA_Destroy:
        HandleDestroyDeliverySuccess(replicationTransmission.GetNetworkId());
        break;
      case RA_Update:
      case RA_RPC:
      case RA_MAX:
        break;
    }
  }
}

void ReplicationManagerTransmissionData::HandleCreateDeliveryFailure(int networkId) const {
  GameObjectPtr gameObject = ServerNetworkManager::Instance->GetGameObject(networkId);
  if (gameObject) {
    replicationManagerServer->ReplicateCreate(networkId, gameObject->GetAllStateMask());
  }
}

void ReplicationManagerTransmissionData::HandleDestroyDeliveryFailure(int networkId) const {
  replicationManagerServer->ReplicateDestroy(networkId);
}

void ReplicationManagerTransmissionData::HandleUpdateStateDeliveryFailure(
    int networkId, uint32_t state, DeliveryNotificationManager* deliveryNotificationManager) const {
  if (!ServerNetworkManager::Instance->GetGameObject(networkId)) return;

  for (const auto& inFlightPacket : deliveryNotificationManager->GetInFlightPackets()) {
    // TODO fix the key here
    ReplicationManagerTransmissionDataPtr replicationManagerTransmissionData =
        std::static_pointer_cast<ReplicationManagerTransmissionData>(inFlightPacket.GetTransmissionData(0));

    for (const ReplicationTransmission& other : replicationManagerTransmissionData->transmissions) {
      state &= ~other.GetState();
    }
  }

  if (state) {
    replicationManagerServer->SetStateDirty(networkId, state);
  }
}

void ReplicationManagerTransmissionData::HandleCreateDeliverySuccess(int network) const {
  replicationManagerServer->HandleCreateAckd(network);
}

void ReplicationManagerTransmissionData::HandleDestroyDeliverySuccess(int networkId) const {
  replicationManagerServer->RemoveFromReplication(networkId);
}