#ifndef ROBO_ACKRANGE_H
#define ROBO_ACKRANGE_H

#include "InFlightPacket.h"
#include "MemoryBitStream.h"
class AckRange {
 public:
  AckRange() : start(0), count(0) {}
  explicit AckRange(PacketSequenceNumber inStart) : start(inStart), count(1) {}

  inline bool ExtendIfShould(PacketSequenceNumber inSequenceNumber);

  [[nodiscard]] PacketSequenceNumber GetStart() const {
    return start;
  }

  [[nodiscard]] uint32_t GetCount() const {
    return count;
  }

  void Write(OutputMemoryBitStream& inOutputStream) const;
  void Read(InputMemoryBitStream& inInputStream);

 private:
  PacketSequenceNumber start;
  uint32_t count;
};

inline bool AckRange::ExtendIfShould(PacketSequenceNumber inSequenceNumber) {
  if (inSequenceNumber == start + count) {
    count++;
    return true;
  } else {
    return false;
  }
}
#endif  // ROBO_ACKRANGE_H
