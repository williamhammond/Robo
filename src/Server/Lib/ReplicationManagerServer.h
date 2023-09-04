#ifndef ROBO_REPLICATIONMANAGERSERVER_H
#define ROBO_REPLICATIONMANAGERSERVER_H

#include <unordered_map>

#include "MemoryBitStream.h"
#include "ReplicationCommand.h"
#include "ReplicationManagerTransmissionData.h"

class ReplicationManagerServer {
 public:
  void ReplicateCreate(int networkId, uint32_t initialDirtyState);
  void ReplicateDestroy(int networkId);
  void SetStateDirty(int networkId, uint32_t dirtyState);
  void HandleCreateAckd(int networkId);
  void RemoveFromReplication(int networkId);

  void Write(OutputMemoryBitStream& outputStream, ReplicationManagerTransmissionData* transmissionData);

 private:
  uint32_t WriteCreateAction(OutputMemoryBitStream& outputStream, int networkId, uint32_t dirtyState);
  uint32_t WriteUpdateAction(OutputMemoryBitStream& outputStream, int networkId, uint32_t dirtyState);
  static uint32_t WriteDestroyAction(OutputMemoryBitStream& outputStream, int networkId, uint32_t dirtyState);

  std::unordered_map<int, ReplicationCommand> networkIdToReplicationCommand;
};

#endif  // ROBO_REPLICATIONMANAGERSERVER_H
