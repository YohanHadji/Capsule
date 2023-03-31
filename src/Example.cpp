#include <Arduino.h>
#include "Capsule.h"         

#define DEVICE1_PORT Serial1
#define DEVICE2_PORT Serial2
#define DEVICE3_PORT Serial3

#define DEVICE1_BAUD 115200
#define DEVICE2_BAUD 115200
#define DEVICE3_BAUD 6000000

void handlePacketDevice1(packet); 
void handlePacketDevice2(packet); 
void handlePacketDevice3(packet); 

void sendRandomPacket();
void sendRandomNoise();

Capsule device1(0xFF,0xFA,handlePacketDevice1);
Capsule device2(0xFF,0xFB,handlePacketDevice2);
Capsule device3(0xFF,0xFC,handlePacketDevice3);

void setup() {
  DEVICE1_PORT.begin(DEVICE1_BAUD);
  DEVICE2_PORT.begin(DEVICE2_BAUD);
  DEVICE3_PORT.begin(DEVICE3_BAUD);
}

void loop() {
  while(DEVICE1_PORT.available()) {
    byte data = DEVICE1_PORT.read();
    device1.decode(data);
  }

  while(DEVICE2_PORT.available()) {
    byte data = DEVICE2_PORT.read();
    device2.decode(data);
  }

  while(DEVICE3_PORT.available()) {
    byte data = DEVICE3_PORT.read();
    device3.decode(data);
  }
}

void handlePacketDevice1(packet dataIn) {
  switch (dataIn.packetId) {
    case 0x00:
      Serial.println("Packet with ID 00 received : ");
      Serial.write(dataIn.packetData,dataIn.len);
    break;
    case 0x01:
      Serial.println("Packet with ID 01 received : ");
      Serial.write(dataIn.packetData,dataIn.len);
    break;
    default:
    break;
  }
}

void handlePacketDevice2(packet dataIn) {
  switch (dataIn.packetId) {
    case 0x00:
      Serial.println("Packet with ID 00 received : ");
      Serial.write(dataIn.packetData,dataIn.len);
    break;
    case 0x01:
      Serial.println("Packet with ID 01 received : ");
      Serial.write(dataIn.packetData,dataIn.len);
    break;
    default:
    break;
  }
}

void handlePacketDevice3(packet dataIn) {
  switch (dataIn.packetId) {
    case 0x00:
    break;
    case 0x01:
      Serial.println("Packet with ID 01 received : ");
      Serial.write(dataIn.packetData,dataIn.len);
    break;
    default:
    break;
  }
}

void sendRandomPacket() {
  packet rawData;
  rawData.packetId = 0x00; 
  rawData.len = 4;
  
  for (int i = 0; i < 4; i++) {
    rawData.packetData[i] = i;
  }
  packet packetToSend = device3.encode(rawData);
  DEVICE3_PORT.write(packetToSend.packetData,packetToSend.len);
}

void sendRandomNoise() {
  byte randomBuffer[100];
  unsigned randomBufferSize = random(0,100);
  for (unsigned i = 0; i < randomBufferSize; i++) {
    randomBuffer[i] = random(0,255);
  }
  DEVICE3_PORT.write(randomBuffer,randomBufferSize);
}

