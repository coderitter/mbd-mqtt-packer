# Coderitter C MQTT codec

A codec for MQTT written in plain C99. It encodes MQTT decodes by given parameters. It decodes MQTT out of a chunk-wise growing stream of bytes. Its only dependencies are `memcpy` and `memmove` from `string.h`. It does not provide the flow of the protocol, neither for the client nor for the server, but it can be used to create client and server implementations.

This library enables you to optimize an MQTT implementation specifically to the plattform that you are using, even to the specific needs of your IoT application. The microcontroller might offer only a limited amount of resources and therefor you only want to connect to an MQTT broker and send PUBLISH messages.

## Quick start

Let your byte stream function fill the `mqttMessageBytes` array with a chunk, keep track of the total size and note the size of the just received chunk.

```c
#define MQTT_MAX_MESSAGE_SIZE 10 * 1024
uint8_t mqttMessageBytes[MQTT_MAX_MESSAGE_SIZE];

uint32_t currentSize = 0;
uint32_t chunkSize = tcpReceiveChunk(mqttMessageBytes);
currentSize += chunkSize;
```

Create an MQTT message struct `mqttMessage` and define a callback function `onMqttMessageDecoded` which will get called by `decodeMqttChunk` as soon as an MQTT message was fully decoded.

```c
struct MqttMessage mqttMessage = 
{
    .bytes = mqttMessageBytes
};

void onMqttMessageDecoded(struct MqttMessage *message)
{
    if (message->controlPacketType == MQTT_CONTROL_PACKET_TYPE_PUBLISH)
    {
        uint8_t pubAckBytes[getMqttPubAckSize()];
        encodeMqttPubAck(message->packetIdentifier, pubAckBytes);
        tcpSend(pubAckBytes);
    }
};

decodeMqttChunk(&mqttMessage, currentSize, chunkSize, onMqttMessageDecoded);
```

## Installation



## Decoding MQTT messages

Since this package only provides means to encode and decode MQTT message, you need to implement everything around it by yourself. Since most of the time you would exchange MQTT message through a network connection which most of the time would use TCP, we will stick with this example.

You can decode a message chunk by chunk as a TCP socket would deliver them to your application. A chunk might either contain a multiple MQTT messages at once or just a part of one.

Since you cannot know, you would collect the bytes chunk by chunk in a larger array. Since in C dynamic arrays are not trivial, you would determine a static array size and by that you would determine the largest MQTT message that you would be able to receive.

```c
#define MQTT_MAX_MESSAGE_SIZE 10 * 1024
uint8_t mqttMessageBytes[MQTT_MAX_MESSAGE_SIZE];
```

In this example, an MQTT message must not be larger than 10 KiB.

The next step was to create a TCP socket and starting an infinite loop reading bytes from it.

```c
Socket socket = socketConnect
chunkSize = usoc_socket_read(socket, &(p->bytes[size]), junkSize < p->maxChunkSize ? junkSize : p->maxChunkSize, 0);

## Encoding MQTT messages

