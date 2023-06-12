#include "AckRange.h"

void AckRange::Write(OutputMemoryBitStream& inOutputStream) const {
  inOutputStream.Write(start);
  bool hasCount = count > 1;
  inOutputStream.Write(hasCount);
  if (hasCount) {
    uint32_t countMinusOne = count - 1;
    uint8_t countToAck = countMinusOne > 255 ? 255 : static_cast<uint8_t>(countMinusOne);
    inOutputStream.Write(countToAck);
  }
}

void AckRange::Read(InputMemoryBitStream& inInputStream) {
  inInputStream.Read(start);
  bool hasCount;
  inInputStream.Read(hasCount);
  if (hasCount) {
    uint8_t countMinusOne;
    inInputStream.Read(countMinusOne);
    count = countMinusOne + 1;
  } else {
    count = 1;
  }
}