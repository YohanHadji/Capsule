#include <Arduino.h>
#include "capsule.h"   

#define SENDER false

#define DEVICE1_PORT Serial1
#define DEVICE2_PORT Serial2
#define DEVICE3_PORT Serial3

#define DEVICE1_BAUD 115200
#define DEVICE2_BAUD 115200
#define DEVICE3_BAUD 6000000

void handlePacketDevice1(byte, byte [], unsigned); 
void handlePacketDevice2(byte, byte [], unsigned); 
void handlePacketDevice3(byte, byte [], unsigned); 

void sendRandomPacket();
void sendRandomNoise();

Capsule device1(0xFF,0xFA,handlePacketDevice1);
Capsule device2(0xFF,0xFB,handlePacketDevice2);
Capsule device3(0xFF,0xFC,handlePacketDevice3);

void setup() {
  DEVICE1_PORT.begin(DEVICE1_BAUD);
  DEVICE2_PORT.begin(DEVICE2_BAUD);
  DEVICE3_PORT.begin(DEVICE3_BAUD);

  if (SENDER) {
    delay(1000);
    sendRandomPacket();
  }
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
    //Serial.print(data,HEX); Serial.print(" "); 
    device3.decode(data);
  }
}

void handlePacketDevice1(byte packetId, byte dataIn[], unsigned len) {
  switch (packetId) {
    case 0x00:
      Serial.println("Packet with ID 00 received : ");
      Serial.write(dataIn,len);
    break;
    case 0x01:
      Serial.println("Packet with ID 01 received : ");
      Serial.write(dataIn,len);
    break;
    default:
    break;
  }
}

void handlePacketDevice2(byte packetId, byte dataIn[], unsigned len) {
  switch (packetId) {
    case 0x00:
      Serial.println("Packet with ID 00 received : ");
      Serial.write(dataIn,len);
    break;
    case 0x01:
      Serial.println("Packet with ID 01 received : ");
      Serial.write(dataIn,len);
    break;
    default:
    break;
  }
}

void handlePacketDevice3(byte packetId, byte dataIn[], unsigned len) {
  switch (packetId) {
    case 0x00:
    break;
    case 0x01:
      //Serial.println("Packet with ID 01 received : ");
      //Serial.write(dataIn,len);
      //delay(1000);
      static unsigned counter = 0;
      counter++;
      Serial.println(counter);
      if (random(0,100) < 10) {
        sendRandomNoise();
      }
      sendRandomPacket();
      delay(100);
    break;
    default:
    break;
  }
}

void sendRandomPacket() {
  byte packetData[4];
  byte packetId = 0x01; 
  unsigned len = 4;
  
  for (int i = 0; i < 4; i++) {
    packetData[i] = i;
  }

  byte* packetToSend = device3.encode(packetId,packetData,len);
  DEVICE3_PORT.write(packetToSend,getCodedLen(len));
  delete[] packetToSend;
}

void sendRandomNoise() {
  byte randomBuffer[25];
  unsigned randomBufferSize = random(0,25);
  for (unsigned i = 0; i < randomBufferSize; i++) {
    randomBuffer[i] = random(0,255);
  }
  DEVICE3_PORT.write(randomBuffer,randomBufferSize);
}

