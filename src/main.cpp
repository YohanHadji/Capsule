#include <Arduino.h>
#include "Capsule/Capsule.h"          // main library

#define DEVICE1_PORT Serial1
#define DEVICE2_PORT Serial2
#define DEVICE3_PORT Serial3

#define DEVICE1_BAUD 115200
#define DEVICE2_BAUD 115200
#define DEVICE3_BAUD 115200

void handlePacketDevice1(); 
void handlePacketDevice2(); 
void handlePacketDevice3(); 

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
    device1.decode(DEVICE1_PORT.read());
  }

  while(DEVICE2_PORT.available()) {
    device2.decode(DEVICE2_PORT.read());
  }

  while(DEVICE3_PORT.available()) {
    device3.decode(DEVICE3_PORT.read());
  }
}

void handlePacketDevice1(packet dataIn) {
  switch (dataIn.packetId) {
    case 0x00:
      // Print some stuff and the data recived
      Serial.println("Packet with ID 00 received : ");
      Serial.write(dataIn.packetData,dataIn.len);
      // Based on the data we receive, we want to send a packet to another device 
      packet rawData;
      // Name the packet you want to send accordingly, 0x32 is random here
      rawData.packetId = 0x32; 
      rawData.len = 16;
      // Fill the packet how you want without having to care about preamble, len and crc
      rawData.packetData[1] = 0x12;
      // Then the packet is ready to be encapsulated in preamble and CRC and be sent to the port.
      packet packetToSend = device3.encode(rawData);
      DEVICE3_PORT.write(packetToSend.packetData,packetToSend.len);
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

