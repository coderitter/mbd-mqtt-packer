# C MQTT packer by Coderitter

A packer for MQTT written in plain C99. It packs und unpacks MQTT to and from byte arrays. It has no dependencies. It also does not provide the flow of the protocol, neither for the client nor for the server, but it can be used to create client and server implementations.

This library enables you to optimize an MQTT implementation specifically to the plattform that you are using, even to the specific needs of your IoT application. The microcontroller might offer only a limited amount of resources and therefor you only want to connect to an MQTT broker and send PUBLISH packets.

## Quick start

```c
uint8_t receivedMqttBytes[100];
uint32_t size;
size = tcpReceive(receivedMqttBytes);

struct MqttPacket mqttPacket;
unpackMqttPacket(receivedMqttBytes, size, &mqttPacket);

if (mqttPacket.type == MQTT_CONTROL_PACKET_TYPE_PUBLISH)
{
    struct MqttPublishPacket publish;
    unpackMqttPublish(receivedMqttBytes, &packet, &publish);
    
    uint32_t pubAckSize = getMqttPubAckSize();
    uint8_t pubAckBytes[pubAckSize];
    packMqttPubAck(pubAckBytes, publish.packetIdentifier);
    
    tcpSend(pubAckBytes, pubAckSize);
}
```

## Installation



## Decoding MQTT packets

Since this package only provides means to pack and unpack MQTT packets, you need to implement everything around it by yourself. Since most of the time you would exchange MQTT packets through a network connection which most of the time would use TCP, we will stick with this example.

You can unpack a packet chunk by chunk as a TCP socket would deliver them to your application. A chunk might either contain multiple MQTT packets at once or just a part of one.

Since you cannot know, you would collect the bytes chunk by chunk in a larger array. Since in C dynamic arrays are not trivial, you would determine a static array size and by that you would determine the largest MQTT packet that you would be able to receive.

```c
#define MQTT_MAX_PACKET_SIZE 10 * 1024
uint8_t mqttPacketBytes[MQTT_MAX_PACKET_SIZE];
```

In this example, an MQTT packet must not be larger than 10 KiB.

The next step was to create a TCP socket and starting an infinite loop reading bytes from it.

```c
Socket socket = socketConnect
chunkSize = socket_read(socket, &(p->bytes[size]), junkSize < p->maxChunkSize ? junkSize : p->maxChunkSize, 0);
```

## Encoding MQTT packets

