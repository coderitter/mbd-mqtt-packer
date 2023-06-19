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

If you want to encode an MQTT packet you need to choose a suitable function. These functions are defined in the `mqtt_packer.h` header.   
   
You need to choose between:   
+ `pack_mqtt_connect(uint8_t* bytes, mqtt_connect_packet_t* packet);`
+ `pack_mqtt_publish(uint8_t* bytes, mqtt_publish_packet_t* packet);`
+ `pack_mqtt_subscribe(uint8_t* bytes, mqtt_un_subscribe_packet_t* packet);`
+ `pack_mqtt_unsubscribe(uint8_t* bytes, mqtt_un_subscribe_packet_t* packet);`  
   
The functions pack the content of the respective struct into the *bytes*.   
   
An example for a *publish packet* would look like this:
    
```c
mqtt_publish_packet_t hello_world_packet;

hello_world_packet.dup = 0;
hello_world_packet.qos = 0;
hello_world_packet.retain = 0;
hello_world_packet.topic_name = (const uint8_t *)"h/w";
hello_world_packet.topic_name_size = strlen((const char *)hello_world_packet.topic_name);
hello_world_packet.payload = (const uint8_t *)"abc";
hello_world_packet.payload_size = strlen((const char *)hello_world_packet.payload);
```
   
The byte array could look like this. 
```c
uint8_t publish_hello_world_bytes[50];
```
   
In this example the byte array holds 50 bytes.   
   
Now you can pack the information into the byte array.   
   
```c
uint32_t publish_size = pack_mqtt_publish(publish_hello_world_bytes, &hello_world_packet);
```
The `pack_mqtt_publish()` returns the size of the publish packet you just packed into the byte array.

## Development

### Build test

If you want to test the pack and unpack functionality simply use the make command.
```bash
make 
```


The console output should look like this:
```bash

get_mqtt_remaining_length_size
--------------------------

It should get the correct remaining length size for a 1 byte length
Failed assertions: 0


...

It should unpack a SUBACK packet
Failed assertions: 0


Total failed assertions: 0
```

### Build library

If you want to build the library simply use the make command.
```bash
make build/libmbd-mqtt-packer.a
```

### Build with different compiler

If you want to use a different compiler, use overrdide CC
```
make build/libmbd-mqtt-packer.a CC=riscv32-unknown-elf-gcc
```