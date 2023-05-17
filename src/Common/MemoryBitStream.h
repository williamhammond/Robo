#ifndef ROBO_MEMORYBITSTREAM_H
#define ROBO_MEMORYBITSTREAM_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>

inline uint32_t ConvertToFixed(float inNumber, float inMin, float inPrecision) {
  return static_cast<uint32_t>((inNumber - inMin) / inPrecision);
}

inline float ConvertFromFixed(uint32_t inNumber, float inMin, float inPrecision) {
  return static_cast<float>(inNumber) * inPrecision + inMin;
}

class OutputMemoryBitStream {
 public:
  OutputMemoryBitStream() : buffer(nullptr), head(0) {
    ReallocBuffer(1500 * 8);
  }

  ~OutputMemoryBitStream() {
    std::free(buffer);
  }

  void WriteBits(uint8_t inData, uint32_t inBitCount);
  void WriteBits(const void* inData, uint32_t inBitCount);

  [[nodiscard]] const char* GetBufferPtr() const {
    return buffer;
  }

  [[nodiscard]] uint32_t GetBitLength() const {
    return head;
  }

  [[nodiscard]] uint32_t GetByteLength() const {
    return (head + 7) >> 3;
  }

  void WriteBytes(const void* inData, uint32_t inByteCount) {
    WriteBits(inData, inByteCount << 3);
  }

  template <typename T>
  void Write(T inData, uint32_t inBitCount = sizeof(T) * 8) {
    static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value,
                  "Generic Write only supports primitive data types");
    WriteBits(&inData, inBitCount);
  }

  void Write(bool inData) {
    WriteBits(&inData, 1);
  }

  void Write(const std::string& inString) {
    auto elementCount = static_cast<uint32_t>(inString.size());
    Write(elementCount);
    for (const auto& element : inString) {
      Write(element);
    }
  }

 private:
  void ReallocBuffer(uint32_t newBitLength);

  char* buffer;
  uint32_t head;
  uint32_t capacity;
};

class InputMemoryBitStream {
 public:
  InputMemoryBitStream(char* inBuffer, uint32_t inBitCount)
      : buffer(inBuffer), head(0), capacity(inBitCount), isOwner(false) {}

  InputMemoryBitStream(const InputMemoryBitStream& inOther)
      : head(inOther.head), capacity(inOther.capacity), isOwner(true) {
    uint32_t byteCount = capacity / 8;
    buffer = static_cast<char*>(malloc(byteCount));
    memcpy(buffer, inOther.buffer, byteCount);
  }

  ~InputMemoryBitStream() {
    if (isOwner) {
      free(buffer);
    }
  }

  [[nodiscard]] const char* GetBufferPtr() const {
    return buffer;
  }

  [[nodiscard]] uint32_t GetRemainingBitCount() const {
    return capacity - head;
  }

  void ReadBits(uint8_t& outData, uint32_t inBitCount);
  void ReadBits(void* outData, uint32_t inBitCount);

  void ReadBytes(void* outData, uint32_t byteCount) {
    ReadBits(outData, byteCount << 3);
  }

  template <typename T>
  void Read(T& inData, uint32_t inBitCount = sizeof(T) * 8) {
    static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value,
                  "Generic Read only supports primitive data types");
    ReadBits(&inData, inBitCount);
  }

  void Read(uint32_t& outData, uint32_t inBitCount = 32) {
    ReadBits(&outData, inBitCount);
  }
  void Read(int& outData, uint32_t inBitCount = 32) {
    ReadBits(&outData, inBitCount);
  }
  void Read(float& outData) {
    ReadBits(&outData, 32);
  }

  void Read(uint16_t& outData, uint32_t inBitCount = 16) {
    ReadBits(&outData, inBitCount);
  }
  void Read(int16_t& outData, uint32_t inBitCount = 16) {
    ReadBits(&outData, inBitCount);
  }

  void Read(uint8_t& outData, uint32_t inBitCount = 8) {
    ReadBits(&outData, inBitCount);
  }
  void Read(bool& outData) {
    ReadBits(&outData, 1);
  }

  void ResetToCapacity(uint32_t inByteCapacity) {
    capacity = inByteCapacity << 3;
    head = 0;
  }

  void Read(std::string& inString) {
    uint32_t elementCount;
    Read(elementCount);
    inString.resize(elementCount);
    for (auto& element : inString) {
      Read(element);
    }
  }

 private:
  char* buffer;
  uint32_t head;
  uint32_t capacity;
  bool isOwner;
};

#endif  // ROBO_MEMORYBITSTREAM_H
