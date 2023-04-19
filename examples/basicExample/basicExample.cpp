#include <Arduino.h>
#include <capsule.h>   

#define SENDER false

#define DEVICE1_PORT Serial1
#define DEVICE2_PORT Serial2
#define DEVICE3_PORT Serial3

#define DEVICE1_BAUD 115200
#define DEVICE2_BAUD 6000000
#define DEVICE3_BAUD 6000000

void handleRxPacketDevice1(uint8_t packetId, uint8_t *dataIn, uint32_t len); 
void handleRxPacketDevice2(uint8_t packetId, uint8_t *dataIn, uint32_t len); 
void handleRxPacketDevice3(uint8_t packetId, uint8_t *dataIn, uint32_t len); 

void sendRandomPacket();
void sendRandomNoise();

CapsuleStatic device1(handleRxPacketDevice1);
CapsuleStatic device2(handleRxPacketDevice2);
CapsuleStatic device3(handleRxPacketDevice3);

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
    uint8_t data = DEVICE1_PORT.read();
    device1.decode(data);
  }

  while(DEVICE2_PORT.available()) {
    uint8_t data = DEVICE2_PORT.read();
    device2.decode(data);
  }

  while(DEVICE3_PORT.available()) {
    uint8_t data = DEVICE3_PORT.read();
    //Serial.print(data,HEX); Serial.print(" "); 
    device3.decode(data);
  }
}

void handleRxPacketDevice1(uint8_t packetId, uint8_t *dataIn, uint32_t len) {
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

void handleRxPacketDevice2(uint8_t packetId, uint8_t *dataIn, uint32_t len) {
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

void handleRxPacketDevice3(uint8_t packetId, uint8_t *dataIn, uint32_t len) {
  switch (packetId) {
    case 0x00:
    break;
    case 0x01:
      //Serial.println("Packet with ID 01 received : ");
      //Serial.write(dataIn,len);
      //delay(1000);
      static uint32_t counter = 0;
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
  uint8_t packetData[4];
  uint8_t packetId = 0x01; 
  uint32_t len = 4;
  
  for (int i = 0; i < 4; i++) {
    packetData[i] = i;
  }

  uint8_t* packetToSend = device3.encode(packetId,packetData,len);
  DEVICE3_PORT.write(packetToSend,device3.getCodedLen(len));
  delete[] packetToSend;
}

void sendRandomNoise() {
  uint8_t randomBuffer[25];
  uint32_t randomBufferSize = random(0,25);
  for (uint32_t i = 0; i < randomBufferSize; i++) {
    randomBuffer[i] = random(0,255);
  }
  DEVICE3_PORT.write(randomBuffer,randomBufferSize);
}

