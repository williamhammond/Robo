#include "MemoryBitStream.h"

#include <cstring>

void OutputMemoryBitStream::WriteBits(uint8_t inData, uint32_t inBitCount) {
  uint32_t nextBitHead = head + static_cast<uint32_t>(inBitCount);

  if (nextBitHead > capacity) {
    ReallocBuffer(std::max(capacity * 2, nextBitHead));
  }

  uint32_t byteOffset = head >> 3;
  uint32_t bitOffset = head & 0x7;

  auto currentMask = static_cast<uint8_t>(~(0xff << bitOffset));
  buffer[byteOffset] = static_cast<char>((buffer[byteOffset] & currentMask) | (inData << bitOffset));

  uint32_t bitsFreeThisByte = 8 - bitOffset;

  if (bitsFreeThisByte < inBitCount) {
    buffer[byteOffset + 1] = static_cast<char>(inData >> bitsFreeThisByte);
  }

  head = nextBitHead;
}

void OutputMemoryBitStream::WriteBits(const void* inData, uint32_t inBitCount) {
  const char* srcByte = static_cast<const char*>(inData);
  while (inBitCount > 8) {
    WriteBits(static_cast<uint8_t>(*srcByte), 8);
    ++srcByte;
    inBitCount -= 8;
  }
  if (inBitCount > 0) {
    WriteBits(static_cast<uint8_t>(*srcByte), inBitCount);
  }
}

void OutputMemoryBitStream::ReallocBuffer(uint32_t newBitLength) {
  if (buffer == nullptr) {
    buffer = static_cast<char*>(std::malloc(newBitLength >> 3));
    memset(buffer, 0, newBitLength >> 3);
  } else {
    char* tempBuffer = static_cast<char*>(std::malloc(newBitLength >> 3));
    memset(tempBuffer, 0, newBitLength >> 3);
    memcpy(tempBuffer, buffer, capacity >> 3);
    std::free(buffer);
    buffer = tempBuffer;
  }

  capacity = newBitLength;
}

void InputMemoryBitStream::ReadBits(uint8_t& outData, uint32_t inBitCount) {
  uint32_t byteOffset = head >> 3;
  uint32_t bitOffset = head & 0x7;

  outData = static_cast<uint8_t>(buffer[byteOffset]) >> bitOffset;

  uint32_t bitsFreeThisByte = 8 - bitOffset;
  if (bitsFreeThisByte < inBitCount) {
    outData |= static_cast<uint8_t>(buffer[byteOffset + 1]) << bitsFreeThisByte;
  }

  outData &= static_cast<uint8_t>((~(0x00ff << inBitCount)));

  head += inBitCount;
}

void InputMemoryBitStream::ReadBits(void* outData, uint32_t inBitCount) {
  auto* destByte = reinterpret_cast<uint8_t*>(outData);
  while (inBitCount > 8) {
    ReadBits(*destByte, 8);
    ++destByte;
    inBitCount -= 8;
  }
  if (inBitCount > 0) {
    ReadBits(*destByte, inBitCount);
  }
}