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
            lenCount = 0;
            currentState = PAYLOAD;
        break;
        case PAYLOAD:
            if (lenCount < buffer.len) {
                buffer.packetData[lenCount] = dataIn;
                lenCount++;
                if (lenCount == buffer.len) {
                    currentState = CRC;
                }
            }
            else {
                currentState = CRC;
            }
        break;
        case CRC:
        uint8_t checkSum = 0;
        for (unsigned i(0); i < buffer.len; i++) {
            checkSum += buffer.packetData[i];
        }
        if (checkSum == dataIn) {
            functionCallBack(buffer);
        }
        else {
            // Should return error code maybe?
        }
        currentState = PREAMBLE_A;
        break;
    }
}

packet Capsule::encode(packet packetIn) {
    packet packetOut;
    packetOut.packetId = packetIn.packetId;
    packetOut.len = packetIn.len+5;
    packetOut.packetData[0] = PRA;
    packetOut.packetData[1] = PRB;
    packetOut.packetData[2] = packetIn.packetId;
    packetOut.packetData[3] = packetIn.len;

    uint8_t checkSum = 0;
    for (unsigned i(0); i < packetIn.len; i++) {
        packetOut.packetData[i + 4] = packetIn.packetData[i];
        checkSum += packetIn.packetData[i];
    }
    packetOut.packetData[packetIn.len + 4] = checkSum; 
    return packetOut;
}