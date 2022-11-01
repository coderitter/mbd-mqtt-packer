# Embedded MQTT packer by Coderitter

A packer for MQTT written in plain C99. It packs und unpacks MQTT to and from byte arrays. It has no dependencies. It also does not provide the flow of the protocol, neither for the client nor for the server, but it can be used to create client and server implementations.

This library enables you to optimize an MQTT implementation specifically to the plattform that you are using, even to the specific needs of your IoT application. The microcontroller might offer only a limited amount of resources and therefor you only want to connect to an MQTT broker and send PUBLISH packets.

## Quick start

```c
uint8_t received_mqtt_bytes[100];
uint32_t size;
size = tcp_r_eceive(received_mqtt_bytes);

mqtt_packet_t mqtt_packet;
unpack_mqtt_packet(received_mqtt_bytes, size, &mqtt_packet);

if (mqtt_packet.type == MQTT_PACKET_PUBLISH)
{
    mqtt_publish_packet_t publish;
    unpack_mqtt_publish(received_mqtt_bytes, &packet, &publish);
    
    uint32_t puback_size = get_mqtt_puback_size();
    uint8_t puback_bytes[puback_size];
    pack_mqtt_puback(puback_bytes, publish.packet_identifier);
    
    tcp_send(puback_bytes, puback_size);
}
```

## Installation



## Decoding MQTT packets

Since this package only provides means to pack and unpack MQTT packets, you need to implement everything around it yourself. Since you would exchange MQTT packets through a network connection which most of the time would use TCP, we will stick with this example.

You can unpack a packet chunk by chunk as a TCP socket would deliver them to your application. A chunk might either contain multiple MQTT packets at once or just a part of one.

Since you cannot know, you would collect the bytes chunk by chunk in a larger array. Since in C dynamic arrays are not trivial, you would determine a static array size and by that you would determine the largest MQTT packet that you would be able to receive.

```c
#define MQTT_MAX_PACKET_SIZE 10 * 1024
uint8_t tcp_bytes[MQTT_MAX_PACKET_SIZE];
```

In this example, an MQTT packet must not be larger than 10 KiB.

The next step is to create a TCP socket and starting an infinite loop reading bytes from it.

```c
socket_t socket = socket_connect();
chunk_size = socket_read(socket, &(p->bytes[size]), junk_size < p->max_chunk_size ? junk_size : p->max_chunk_size, 0);
```

## Encoding MQTT packets

