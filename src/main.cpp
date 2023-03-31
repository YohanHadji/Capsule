#include <Arduino.h>
#include "Capsule/Capsule.h"    
#include "packetDef.h"      

#define DEVICE1_PORT Serial1
#define DEVICE2_PORT Serial2
#define DEVICE3_PORT Serial3

#define DEVICE1_BAUD 115200
#define DEVICE2_BAUD 115200
#define DEVICE3_BAUD 115200

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
  pinMode(LED_BUILTIN, OUTPUT);
  sendRandomPacket();
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

  /* static unsigned lastPacketSent = 0;
  if (millis() - lastPacketSent > 10000) {
    sendRandomPacket();
    lastPacketSent = millis();
  } */
}

void handlePacketDevice1(packet dataIn) {
  switch (dataIn.packetId) {
    // Let'pretend the ID 0x01 if for a packet sent by AV 
    case 0x01:
      // Example of checking the data with Lionel's packet definition 
      PacketAV_downlink packetAVDown; 
      if (dataIn.len == packetAV_downlink_size) { 
        memcpy(&packetAVDown, *dataIn.packetData, packetAV_downlink_size);
        if (packetAVDown.baro_temp == 0) {
            Serial.println("Baro temp is 0");
        }
      }

      // Based on the packet we juste received, we might want to send something else to someone else
      packet rawData;
      rawData.packetId = 0x02; // Let's pretend 0x02 is the good ID
      rawData.len = packetAV_uplink_size;
      PacketAV_uplink packetAVUp;
      memcpy(rawData.packetData, &packetAVUp, packetAV_uplink_size); 
      // Then the packet is ready to be encapsulated between preamble and CRC and be sent to the port.
      packet packetToSend = device3.encode(rawData);
      DEVICE3_PORT.write(packetToSend.packetData,packetToSend.len);
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
      digitalWrite(LED_BUILTIN, HIGH);
      /* Serial.println("Packet with ID 00 received : ");
      //Serial.write(dataIn.packetData,dataIn.len);
      for (int i = 0; i < dataIn.len; i++) {
        Serial.print(dataIn.packetData[i],HEX);
        Serial.print(" ");
      }
      Serial.println(); */
      delay(20);
      digitalWrite(LED_BUILTIN, LOW);
      delay(20);
      unsigned test = random(0,10);
      if (test >= 9) {
        sendRandomNoise();
      }
      sendRandomPacket();
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
  //Serial.println("Sending some packet");
  // Based on the packet we juste received, we might want to send something else to someone else
  packet rawData;
  rawData.packetId = 0x00; // Let's pretend 0x02 is the good ID
  rawData.len = 4;
  
  for (int i = 0; i < 4; i++) {
    rawData.packetData[i] = i;
  }
  packet packetToSend = device3.encode(rawData);
  DEVICE3_PORT.write(packetToSend.packetData,packetToSend.len);
}

void sendRandomNoise() {
  byte randomBuffer[255];
  unsigned randomBufferSize = random(0,255);
  for (int i = 0; i < randomBufferSize; i++) {
    randomBuffer[i] = random(0,255);
  }
  DEVICE3_PORT.write(randomBuffer,randomBufferSize);
}

