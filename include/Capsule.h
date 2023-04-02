#ifndef CAPSULE_H
#define CAPSULE_H

#include <Arduino.h>

#define MAX_BUFFER_SIZE 1024

enum parserState {
    PREAMBLE_A = 0,
    PREAMBLE_B,
    PACKET_ID,
    LENGTH,
    PAYLOAD,
    CRC
};

unsigned getCodedLen(unsigned);

class Capsule {
  public:
    Capsule(byte, byte, void (*function)(byte, byte [], unsigned));
    void decode(byte);
    byte* encode(byte, byte [], unsigned);
  private:
    byte buffer[MAX_BUFFER_SIZE];
    byte PRA;
    byte PRB;
    byte packetId;
    byte len;
    parserState currentState;
    unsigned lenCount;
    void (*functionCallBack)(byte, byte [], unsigned);
};


#endif    // end CAPSULE_H