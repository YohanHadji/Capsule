#ifndef UARTPRO_H
#define UARTPRO_H

#include <Arduino.h>

enum parserState {
    PREAMBLE_A = 0,
    PREAMBLE_B,
    PACKET_ID,
    LENGTH,
    PAYLOAD,
    CRC
};

struct packet {
    byte packetData[256];
    unsigned len;
    byte packetId;
};

class Capsule {
  public:
    Capsule(byte, byte, void (*function)(packet));
    void decode(byte);
    packet encode(packet);
  private:
    packet buffer;
    byte PRA;
    byte PRB;
    parserState currentState;
    unsigned lenCount;
    void (*functionCallBack)(packet);
};

#endif    //end UARTPRO_H