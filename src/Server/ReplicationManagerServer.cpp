#include "ReplicationManagerServer.h"

#include "GameObject.h"
#include "ReplicationCommand.h"
#include "ServerNetworkManager.h"

void ReplicationManagerServer::ReplicateCreate(int networkId, uint32_t initialDirtyState) {
  networkIdToReplicationCommand[networkId] = ReplicationCommand(initialDirtyState);
}

void ReplicationManagerServer::ReplicateDestroy(int networkId) {
  networkIdToReplicationCommand[networkId].SetDestroy();
}

void ReplicationManagerServer::RemoveFromReplication(int networkId) {
  networkIdToReplicationCommand.erase(networkId);
}

void ReplicationManagerServer::SetStateDirty(int networkId, uint32_t dirtyState) {
  networkIdToReplicationCommand[networkId].AddDirtyState(dirtyState);
}

void ReplicationManagerServer::HandleCreateAckd(int networkId) {
  networkIdToReplicationCommand[networkId].HandleCreateAckd();
}

void ReplicationManagerServer::Write(OutputMemoryBitStream& outputStream,
                                     ReplicationManagerTransmissionData* transmissionData) {
  for (auto& pair : networkIdToReplicationCommand) {
    ReplicationCommand& replicationCommand = pair.second;
    if (replicationCommand.HasDirtyState()) {
      int networkId = pair.first;

      outputStream.Write(networkId);

      ReplicationAction action = replicationCommand.GetAction();
      outputStream.Write(action, 2);

      uint32_t writtenState = 0;
      uint32_t dirtyState = replicationCommand.GetDirtyState();

      switch (action) {
        case RA_Create:
          writtenState = WriteCreateAction(outputStream, networkId, dirtyState);
          break;
        case RA_Update:
          writtenState = WriteUpdateAction(outputStream, networkId, dirtyState);
          break;
        case RA_Destroy:
          writtenState = WriteDestroyAction(outputStream, networkId, dirtyState);
          break;
        case RA_RPC:
        case RA_MAX:
          break;
      }

      transmissionData->AddTransmission(networkId, action, writtenState);
      replicationCommand.ClearDirtyState(writtenState);
    }
  }
}

uint32_t ReplicationManagerServer::WriteCreateAction(OutputMemoryBitStream& outputStream, int networkId,
                                                     uint32_t dirtyState) {
  GameObjectPtr gameObject = ServerNetworkManager::Instance->GetGameObject(networkId);
  outputStream.Write(gameObject->GetClassId());
  return gameObject->Write(outputStream, dirtyState);
}

uint32_t ReplicationManagerServer::WriteUpdateAction(OutputMemoryBitStream& outputStream, int networkId,
                                                     uint32_t dirtyState) {
  GameObjectPtr gameObject = ServerNetworkManager::Instance->GetGameObject(networkId);
  uint32_t writtenState = gameObject->Write(outputStream, dirtyState);
  return writtenState;
}

uint32_t ReplicationManagerServer::WriteDestroyAction(OutputMemoryBitStream& outputStream, int networkId,
                                                      uint32_t dirtyState) {
  (void)outputStream;
  (void)networkId;
  (void)dirtyState;

  return dirtyState;
}