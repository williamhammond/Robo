#ifndef ROBO_NETWORKIDS_H
#define ROBO_NETWORKIDS_H

#include <cstdint>

// TODO Find a better way to do this. Maybe a https://en.wikipedia.org/wiki/FourCC?
enum class PacketType : uint32_t { HelloPacketId = '1', WelcomePacketId = '2', WinPacketId = '3' };

#endif  // ROBO_NETWORKIDS_H
