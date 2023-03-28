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

