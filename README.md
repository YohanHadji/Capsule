# Capsule

This is supposed to be a small protocol on the layer above the UART protocol and `Serial.read()`, `Serial.write()` function from the Arduino framework to transmit packets with a two bytes preamble, a one byte packet ID, CRC, and callback functions. 

For example: ```Capsule device1(0xFF,0xFA,handlePacketDevice1);```

The protocol is implemented in the `Capsule` class. The class is initialised with 3 parameters, two preamble bytes, and the callback function to call when a packet is received to handle the decoded data. 

The goal is to be able to reliably manage multiple bidirectional UART communications on a single board. The `main.cpp` attached file is an example of the class used to manage 3 different UART ports. We start by creating the 3 objects as well as redefining the 3 UART port fo easier use later. We also decided to create a different handle fonction for each device. In some cases, using a single handle function for all devices might be possible. 

<img src="https://user-images.githubusercontent.com/58160726/228162170-5696e2e3-6f03-4f44-9bfa-80ff29bf40a6.png" width=45% height=45%>

You can note that here, a different second preamble byte has been given to each device, this can be used to avoid a device being connected on the wrong port. Giving the same preamble byte to all devices, and using a single handling function would allow you to connect any device on any physical serial port on your board given that all serial ports are configured correctly in your `setup()`. This is probably the most powerful use of this code. 

In the `loop()`, every byte received from the UART port is feed in the object using the `decode` function. As soon as a full packet is received, the handling callback function will be called to manage the decapsulated data. 

<img src="https://user-images.githubusercontent.com/58160726/228163890-8532bdc5-9697-44cc-8f86-fcb6d98fe9cc.png" width=40% height=40%>

Here is one example of data hanlding function. Note that at no point in the structure of the raw data defined, this is something that will be managed by another module. Here we just manage the encapsulation and decapsulation of the data on both ends. In this example, we respond to a given packet with another packet. For this the `encode(packet)` function is used. This function will place the packet given in input between the preamble, id, len, and crc bytes and will return the packet ready to be sent. Again, the structure of your data is not considered here. You can input whatever you want in the packet transmited to the `encode(packet)` function and it'll encode it without having to understand what it is. 

<img src="https://user-images.githubusercontent.com/58160726/228165960-abe60c9f-bbd3-4421-ad46-aef9ce75fa17.png" width=85% height=85%>
