#include <Arduino.h>
#include <Capsule.h>   

#define SENDER false

#define SERIAL_TO_PC Serial

#define DEVICE1_PORT Serial1
#define DEVICE2_PORT Serial2
#define DEVICE3_PORT Serial3

#define DEVICE1_BAUD 115200
#define DEVICE2_BAUD 6000000
#define DEVICE3_BAUD 6000000

void handleRxPacketDevice1(uint8_t packetId, uint8_t *dataIn, uint32_t len); 
void handleRxPacketDevice2(uint8_t packetId, uint8_t *dataIn, uint32_t len); 
void handleRxPacketDevice3(uint8_t packetId, uint8_t *dataIn, uint32_t len); 

void sendPacketWithDevice3();
void sendNoiseWithDevice3();

CapsuleStatic device1(handleRxPacketDevice1);
CapsuleStatic device2(handleRxPacketDevice2);
CapsuleStatic device3(handleRxPacketDevice3);

enum CAPSULE_ID {
  TELEMETRY = 0,
  COMMAND,
  EMERGENCY,
  ACK
};

long counter = 0;

void setup() {
  //SERIAL_TO_PC.begin(115200);
  DEVICE1_PORT.begin(DEVICE1_BAUD);
  DEVICE2_PORT.begin(DEVICE2_BAUD);
  DEVICE3_PORT.begin(DEVICE3_BAUD);

  if (SENDER) {
    delay(1000);
    sendPacketWithDevice3();
  }

  pinMode(LED_BUILTIN, OUTPUT);
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
    device3.decode(data);
  }
}

void handleRxPacketDevice1(uint8_t packetId, uint8_t *dataIn, uint32_t len) {
  switch (packetId) {
    case CAPSULE_ID::TELEMETRY:
      SERIAL_TO_PC.println("Telemetry packet received from device 1");
      SERIAL_TO_PC.write(dataIn,len);
    break;
    case CAPSULE_ID::COMMAND:
      SERIAL_TO_PC.println("Command packet received from device 1");
      SERIAL_TO_PC.write(dataIn,len);
    break;
    default:
    break;
  }
}

void handleRxPacketDevice2(uint8_t packetId, uint8_t *dataIn, uint32_t len) {
  switch (packetId) {
    case CAPSULE_ID::EMERGENCY:
      SERIAL_TO_PC.println("Emergency packet received from device 2");
      SERIAL_TO_PC.write(dataIn,len);
    break;
    case CAPSULE_ID::ACK:
      SERIAL_TO_PC.println("Ack packet received from device 2");
      SERIAL_TO_PC.write(dataIn,len);
    break;
    default:
    break;
  }
}

void handleRxPacketDevice3(uint8_t packetId, uint8_t *dataIn, uint32_t len) {
  switch (packetId) {
    case CAPSULE_ID::TELEMETRY:
    break;
    case CAPSULE_ID::COMMAND:
    break;
    case CAPSULE_ID::EMERGENCY:
    break;
    case CAPSULE_ID::ACK:
    {
      digitalWrite(LED_BUILTIN, HIGH);
      uint8_t* packetToSend = new uint8_t[device3.getCodedLen(len)];
      packetToSend = device3.encode(packetId, dataIn, len);
      DEVICE3_PORT.write(packetToSend,device3.getCodedLen(len));
      delete[] packetToSend;
      digitalWrite(LED_BUILTIN, LOW);
      //counter++;
      //Serial.println(counter);
    }
    break;
    default:
    break;
  }
}

void sendPacketWithDevice3() {
  uint8_t packetData[200];
  uint8_t packetId = CAPSULE_ID::ACK; 
  uint32_t len = 200;
  
  for (int i = 0; i < 200; i++) {
    packetData[i] = i;
  }
  uint8_t* packetToSend = device3.encode(packetId,packetData,len);
  DEVICE3_PORT.write(packetToSend,device3.getCodedLen(len));
  delete[] packetToSend;
}

void sendNoiseWithDevice3() {
  uint8_t randomBuffer[25];
  uint32_t randomBufferSize = random(0,25);
  for (uint32_t i = 0; i < randomBufferSize; i++) {
    randomBuffer[i] = random(0,255);
  }
  DEVICE3_PORT.write(randomBuffer,randomBufferSize);
}
