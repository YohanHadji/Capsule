# Capsule

This is supposed to be a small protocol on the layer above the UART protocol and `Serial.read()`, `Serial.write()` function from the Arduino framework to transmit packets with a two bytes preamble, a one byte packet ID, CRC, and callback functions. 

The protocol is implemented in the `Capsule` class. The class is initialised with 3 (The last 2 are optional) parameters. The first parameter is the callback function to call when a packet is received to handle the decoded data. The two next parameters are the preamble bytes to use to transmit the packet. This is some sort of a code between devices. By default, the preamble bytes are 0xFF and 0xFA. You can choose any other combination as long as you set the same combination on both ends of the communication (so on the transmitting AND the receiving device).

The goal is to be able to reliably manage multiple bidirectional UART communications on a single board. The `basicExample.cpp` attached file is an example of the class used to manage 3 different UART ports. We start by creating the 3 objects as well as redefining the 3 UART port fo easier use later. The main class is named `Capsule` however, this class has been written to be potentially used inside another class where the callback function would also be. Because of that, when you want to use the `Capsule`class outside of any class, you have to use the `CapsuleStatic` name instead of `Capsule` alone. We also decided to create a different handle fonction for each device. In some cases, using a single handle function for all devices might be possible. 

Here is how to create a Capsule object outside of any class without specificing any special preamble: 

You first have to write the prototype of the callback function that willl be called when a full packed has been parsed. This callback function has 3 paremeters, the packetId, used to give the ability to the user to differentiate multiple different potential packet types, then the data as a byte array and the lenght of the data.  

```void handleRxPacketDevice1(uint8_t packetId, uint8_t *dataIn, uint32_t len);```

The depending on whether we use the Capsule object within another class or at main level there are two different constructions of the object. 

If the object is used at main level. You should use this one:

```CapsuleStatic device1(handlePacketDevice1);```

If you want to use specific preambles bytes you can use this 

```CapsuleStatic device1(handlePacketDevice1,PRA,PRB);```

If the object is used withing another class and the callback function is a method of this given class then the Capsule object should be created like that:

```Capsule<ReplaceWithYourClass> device1(handlePacketDeviceDefinedInYourClass, this);```

You can again specify specific preambles bytes to use with : 

```Capsule<ReplaceWithYourClass> device1(handlePacketDeviceDefinedInYourClass, this, PRA, PRB);```

Once the object is created, you have to "feed it" with incoming data. Here is an example of how it's done when the data is coming from a serial port. The implementation would be almost the same for data coming from a LoRa or from WiFi, the Capsule class doesn't care where or how the data is arriving, as long as you feed the data byte by byte inside its encode function. 

```
while(DEVICE1_PORT.available()) {
    uint8_t data = DEVICE1_PORT.read();
    device1.decode(data);
  }
```

The Capsule class will eat the bytes one by one and will let you know when a full packet has been parsed by calling the callback function that you provided. Here is an example of how.. handle the data. 

```
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
```

This is pretty cool right?! Now, can you encode and send packets too?? Yes - here is how. 

This function is an example of how to send a packet with 4 data bytes. The first thing you can note is that to send 4 data bytes, we actually have to send 4+5 = 9 bytes. So sending very small packets is not super usefull. This is the case because we have 2 preambles bytes used to detect the beginning of a packet. Then we have the packetId, the lenght byte, and at the end of the packet the checksum byte. 

```
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
```

The main thing to note is that the .encode(packetId, packetData, len) method from the Capsule class is returning a pointer on a byte, so the byte array is existing somewhere in memory, we can read it to send the packet, but we will have to delete it before we leave our sending function. This is really important! Otherwise you'll have memory leak problems. The reason for this choice is to avoid having to have the output array as a parameter of the encode function. It's nicer to just be able to write output = encode(input) than encode(input, output). But the drawback is that you have to make sure you delete output at the end of your sending function. 