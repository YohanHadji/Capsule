#include "string.h"
#include "capsule.h"

Capsule::Capsule(byte PRAIn, byte PRBIn, void (*function)(byte, byte[], unsigned)) 
: PRA(PRAIn), PRB(PRBIn), currentState(PREAMBLE_A), lenCount(0), functionCallBack(function)
{
}

void Capsule::decode(byte dataIn) {
    switch (currentState) {
        case PREAMBLE_A:
            //Serial.println("PREAMBLE_A");
            if (dataIn == PRA) {
                currentState = PREAMBLE_B;
            }
        break;
        case PREAMBLE_B:
            //Serial.println("PREAMBLE_B");
            if (dataIn == PRB) {
                currentState = PACKET_ID;
                packetId = 0x00;
                for (unsigned i(0); i < MAX_BUFFER_SIZE; i++) {
                    buffer[i] = 0;
                }
            } else {
                currentState = PREAMBLE_A;
            }
        break;
        case PACKET_ID:
            //Serial.println("PACKET_ID");
            packetId = dataIn;
            currentState = LENGTH;
        break;
        case LENGTH:
            //Serial.println("LENGTH");
            len = dataIn;
            lenCount = 0;
            currentState = PAYLOAD;
        break;
        case PAYLOAD:
            //Serial.println("PAYLOAD");
            if (lenCount < len) {
                buffer[lenCount] = dataIn;
                lenCount++;
                if (lenCount == len) {
                    currentState = CRC;
                }
            }
            else {
                currentState = CRC;
            }
        break;
        case CRC:
            //Serial.println("CRC");
            uint8_t checkSum = 0;
            for (unsigned i(0); i < len; i++) {
                checkSum += buffer[i];
            }
            if (checkSum == dataIn) {
                //Serial.println("Calling function");
                functionCallBack(packetId, buffer, len);
            }
            else {
                // Should return error code maybe?
            }
            currentState = PREAMBLE_A;
        break;
    }
}

byte* Capsule::encode(byte packetId, byte packetIn[], unsigned lenIn) {
    unsigned lenOut = getCodedLen(lenIn);
    byte* packetOut = new byte[lenOut];

    packetOut[0] = PRA;
    packetOut[1] = PRB;
    packetOut[2] = packetId;
    packetOut[3] = lenIn;

    uint8_t checkSum = 0;
    for (unsigned i(0); i < lenIn; i++) {
        packetOut[i + 4] = packetIn[i];
        checkSum += packetIn[i];
    }
    packetOut[lenIn + 4] = checkSum; 
    return packetOut;
}

unsigned getCodedLen(unsigned lenIn) {
    return lenIn + 5;
}