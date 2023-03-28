#include "string.h"
#include "Capsule.h"

Capsule::Capsule(byte PRAIn, byte PRBIn, void (*function)(packet)) 
: PRA(PRAIn), PRB(PRBIn), currentState(PREAMBLE_A), lenCount(0), functionCallBack(function)
{
}

void Capsule::decode(byte dataIn) {
    switch (currentState) {
        case PREAMBLE_A:
            if (dataIn == PRA) {
                currentState = PREAMBLE_B;
            }
        break;
        case PREAMBLE_B:
            if (dataIn == PRB) {
                currentState = PACKET_ID;
                buffer.packetId = 0x00;
                for (unsigned i(0); i < 256; i++) {
                    buffer.packetData[i] = 0;
                }
            } else {
                currentState = PREAMBLE_A;
            }
        break;
        case PACKET_ID:
            buffer.packetId = dataIn;
            currentState = LENGTH;
        break;
        case LENGTH:
            buffer.len = dataIn;
            lenCount = buffer.len;
            currentState = PAYLOAD;
        break;
        case PAYLOAD:
            if (lenCount == 0) {
                currentState = CRC;
                break;
            }
            buffer.packetData[buffer.len - lenCount] = dataIn;
            lenCount--;
        break;
        case CRC:
        // Implement CRC check here
        // If CRC is correct, call the callback function
        functionCallBack(buffer);
        // If CRC is incorrect, reset the state machine
        currentState = PREAMBLE_A;
        break;
    }
}

packet Capsule::encode(packet) {

}