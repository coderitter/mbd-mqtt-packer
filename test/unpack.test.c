#include <stdio.h>
#include <stdint.h>
#include "../include/mqtt_packer.h"

uint16_t failedAssertions = 0;

static uint16_t itShouldUnpackWithA2ByteHeaderAndAZeroLengthRemainingSize()
{
    printf("It should unpack with a 2 byte header and a zero length remaining size\n");
    failedAssertions = 0;

    uint8_t bytes[10] = { 0x30, 0x00 };
    struct MqttPacket packet;

    unpackMqttPacket(bytes, 2, &packet);

    if (packet.size != 2)
    {
        printf("Callback: Expected size to be 2 but was %i\n", packet.size);
        failedAssertions++;
    }

    if (packet.fixedHeaderSize != 2)
    {
        printf("Callback: Expected fixedHeaderSize to be 2 but was %i\n", packet.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Callback: Expected type to be 0x03 but was 0x%X\n", packet.type);
        failedAssertions++;
    }
        
    if (packet.remainingSize != 0)
    {
        printf("Callback: Expected remainingSize to be 0 but was %i\n", packet.remainingSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldUnpackWithA2ByteHeader()
{
    printf("It should unpack with a 2 byte header\n");
    failedAssertions = 0;

    uint8_t bytes[10] = { 0x30, 0x01, 0xFF };
    struct MqttPacket packet;

    unpackMqttPacket(bytes, 3, &packet);

    if (packet.size != 3)
    {
        printf("Callback: Expected size to be 3 but was %i\n", packet.size);
        failedAssertions++;
    }

    if (packet.fixedHeaderSize != 2)
    {
        printf("Callback: Expected fixedHeaderSize to be 2 but was %i\n", packet.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Callback: Expected type to be 0x03 but was 0x%X\n", packet.type);
        failedAssertions++;
    }

    if (packet.remainingSize != 1)
    {
        printf("Callback: Expected remainingSize to be 1 but was %i\n", packet.remainingSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldUnpackWithA3ByteHeader()
{
    printf("It should unpack with a 3 byte header\n");
    failedAssertions = 0;

    uint8_t bytes[10] = { 0x30, 0x81, 0x02, 0xFF };
    struct MqttPacket packet;

    unpackMqttPacket(bytes, 4, &packet);

    if (packet.size != 260)
    {
        printf("Expected size to be 260 but was %i\n", packet.size);
        failedAssertions++;
    }

    if (packet.fixedHeaderSize != 3)
    {
        printf("Expected fixedHeaderSize to be 3 but was %i\n", packet.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Expected type to be 0x03 but was 0x%X\n", packet.type);
        failedAssertions++;
    }

    if (packet.remainingSize != 257)
    {
        printf("Expected remainingSize to be 257 but was %i\n", packet.remainingSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldUnpackWithA4ByteHeader()
{
    printf("It should unpack with a 4 byte header\n");
    failedAssertions = 0;

    uint8_t bytes[10] = { 0x30, 0x81, 0x82, 0x03, 0xFF };
    struct MqttPacket packet;

    unpackMqttPacket(bytes, 5, &packet);

    if (packet.size != 49413)
    {
        printf("Expected size to be 49413 but was %i\n", packet.size);
        failedAssertions++;
    }

    if (packet.fixedHeaderSize != 4)
    {
        printf("Expected fixedHeaderSize to be 4 but was %i\n", packet.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Expected type to be 0x03 but was 0x%X\n", packet.type);
        failedAssertions++;
    }

    if (packet.remainingSize != 49409)
    {
        printf("Expected remainingSize to be 49409 but was %i\n", packet.remainingSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldUnpackWithA5ByteHeader()
{
    printf("It should unpack with a 5 byte header\n");
    failedAssertions = 0;

    uint8_t bytes[10] = { 0x30, 0x81, 0x82, 0x83, 0x04, 0xFF };
    struct MqttPacket packet;

    unpackMqttPacket(bytes, 6, &packet);

    if (packet.size != 8438022)
    {
        printf("Expected size to be 8438022 but was %i\n", packet.size);
        failedAssertions++;
    }

    if (packet.fixedHeaderSize != 5)
    {
        printf("Expected fixedHeaderSize to be 5 but was %i\n", packet.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Expected type to be 0x03 but was 0x%X\n", packet.type);
        failedAssertions++;
    }

    if (packet.remainingSize != 8438017)
    {
        printf("Expected remainingSize to be 8438017 but was %i\n", packet.remainingSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldUnpackWithA5ByteHeaderGivenThrough1ByteChunks()
{
    printf("It should unpack with a 5 byte header given through 1 byte chunks\n");
    failedAssertions = 0;

    uint8_t bytes[10] = { 0x30, 0x81, 0x82, 0x83, 0x04, 0xFF };
    struct MqttPacket packet;

    unpackMqttPacket(bytes, 1, &packet);

    if (packet.size != 2)
    {
        printf("Byte 1: Expected size to be 2 but was %i\n", packet.size);
        failedAssertions++;
    }

    if (packet.fixedHeaderSize != 2)
    {
        printf("Byte 1: Expected fixedHeaderSize to be 2 but was %i\n", packet.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Byte 1: Expected type to be 0x03 but was 0x%X\n", packet.type);
        failedAssertions++;
    }

    if (packet.remainingSize != 0)
    {
        printf("Byte 1: Expected remainingSize to be 0 but was %i\n", packet.remainingSize);
        failedAssertions++;
    }

    unpackMqttPacket(bytes, 2, &packet);

    if (packet.size != 4)
    {
        printf("Byte 2: Expected size to be 4 but was %i\n", packet.size);
        failedAssertions++;
    }

    if (packet.fixedHeaderSize != 3)
    {
        printf("Byte 2: Expected fixedHeaderSize to be 3 but was %i\n", packet.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Byte 2: Expected type to be 0x03 but was 0x%X\n", packet.type);
        failedAssertions++;
    }

    if (packet.remainingSize != 1)
    {
        printf("Byte 2: Expected remainingSize to be 1 but was %i\n", packet.remainingSize);
        failedAssertions++;
    }

    unpackMqttPacket(bytes, 3, &packet);

    if (packet.size != 261)
    {
        printf("Byte 3: Expected size to be 261 but was %i\n", packet.size);
        failedAssertions++;
    }

    if (packet.fixedHeaderSize != 4)
    {
        printf("Byte 3: Expected fixedHeaderSize to be 4 but was %i\n", packet.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Byte 3: Expected type to be 0x03 but was 0x%X\n", packet.type);
        failedAssertions++;
    }

    if (packet.remainingSize != 257)
    {
        printf("Byte 3: Expected remainingSize to be 257 but was %i\n", packet.remainingSize);
        failedAssertions++;
    }

    unpackMqttPacket(bytes, 4, &packet);

    if (packet.size != 49414)
    {
        printf("Byte 4: Expected size to be 49414 but was %i\n", packet.size);
        failedAssertions++;
    }

    if (packet.fixedHeaderSize != 5)
    {
        printf("Byte 4: Expected fixedHeaderSize to be 5 but was %i\n", packet.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (packet.remainingSize != 49409)
    {
        printf("Byte 4: Expected remainingSize to be 49409 but was %i\n", packet.remainingSize);
        failedAssertions++;
    }

    if (packet.type != 0x03)
    {
        printf("Byte 4: Expected type to be 0x03 but was 0x%X\n", packet.type);
        failedAssertions++;
    }

    unpackMqttPacket(bytes, 5, &packet);

    if (packet.size != 8438022)
    {
        printf("Byte 5: Expected size to be 8438022 but was %i\n", packet.size);
        failedAssertions++;
    }

    if (packet.fixedHeaderSize != 5)
    {
        printf("Byte 5: Expected fixedHeaderSize to be 5 but was %i\n", packet.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Byte 5: Expected type to be 0x03 but was 0x%X\n", packet.type);
        failedAssertions++;
    }

    if (packet.remainingSize != 8438017)
    {
        printf("Byte 5: Expected remainingSize to be 8438017 but was %i\n", packet.remainingSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldUnpackOnlyOnePacket()
{
    printf("It should unpack only one packet\n");
    failedAssertions = 0;

    uint8_t bytes[] = { 0xC0, 0x00, 0xD0, 0x00, 0x40, 0x02, 0xAA };
    struct MqttPacket packet;

    unpackMqttPacket(bytes, 7, &packet);

    if (packet.size != 2)
    {
        printf("Callback 1: Expected size to be 2 but was %i\n", packet.size);
        failedAssertions++;
    }

    if (packet.fixedHeaderSize != 2)
    {
        printf("Callback 1: Expected fixedHeaderSize to be 2 but was %i\n", packet.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (packet.type != 0x0C)
    {
        printf("Callback 1: Expected type to be 0x0C but was 0x%X\n", packet.type);
        failedAssertions++;
    }
        
    if (packet.remainingSize != 0)
    {
        printf("Callback 1: Expected remainingSize to be 0 but was %i\n", packet.remainingSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldUnpackThePacketIdentifierWhichIsAfterTheFixedHeader()
{
    printf("It should unpack the packet identifier which is after the fixed header\n");
    failedAssertions = 0;

    uint8_t bytes[] = { 0x40, 0x02, 0xAA, 0xBB };
    struct MqttPacket packet;
    unpackMqttPacket(bytes, 4, &packet);

    uint16_t packetIdentifier = 0;
    unpackMqttPacketIdentifier(bytes, &packet, &packetIdentifier);

    if (packetIdentifier != 0xAABB)
    {
        printf("Expected packetIdentifier to be 0xAABB but was 0x%X\n", packetIdentifier);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldUnpackAConnAckPacket()
{
    printf("It should unpack a CONNACK packet\n");
    failedAssertions = 0;

    uint8_t bytes[] = { 0x20, 0x02, 0x01, 0x02 };
    struct MqttPacket packet;
    unpackMqttPacket(bytes, 4, &packet);

    struct MqttConnAckPacket connAck;
    unpackMqttConnAck(bytes, &packet, &connAck);

    if (connAck.sessionPresent != 1)
    {
        printf("Expected sessionPresent to be 1 but was %i\n", connAck.sessionPresent);
        failedAssertions++;
    }

    if (connAck.returnCode != 0x02)
    {
        printf("Expected returnCode to be 0x02 but was 0x%X\n", connAck.returnCode);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldUnpackAPublishPacket()
{
    printf("It should unpack a PUBLISH packet\n");
    failedAssertions = 0;

    uint8_t bytes[] = { 0x3D, 0x0C, 0x00, 0x05, 0x54, 0x4F, 0x50, 0x49, 0x43, 0xAA, 0xBB, 0x00, 0x01, 0x02 };
    struct MqttPacket packet;

    unpackMqttPacket(bytes, 14, &packet);

    struct MqttPublishPacket publish;
    unpackMqttPublish(bytes, &packet, &publish);

    if (publish.dup != 1)
    {
        printf("Expected dup to be 1 but was %i\n", publish.dup);
        failedAssertions++;
    }

    if (publish.qos != MQTT_QOS_EXACTLY_ONCE)
    {
        printf("Expected qos to be 0x02 but was 0x%X\n", publish.qos);
        failedAssertions++;
    }

    if (publish.retain != 1)
    {
        printf("Expected retain to be 1 but was %i\n", publish.dup);
        failedAssertions++;
    }

    if (publish.topicName != &(bytes[4]))
    {
        printf("Expected topicName to be %p but was %p\n", &(bytes[4]), publish.topicName);
        failedAssertions++;
    }

    if (publish.topicNameSize != 5)
    {
        printf("Expected topicNameSize to be 5 but was %i\n", publish.topicNameSize);
        failedAssertions++;
    }

    if (publish.packetIdentifier != 0xAABB)
    {
        printf("Expected packetIdentifier to be 0xAABB but was 0x%X\n", publish.packetIdentifier);
        failedAssertions++;
    }

    if (publish.payload != &(bytes[11]))
    {
        printf("Expected payload to be %p but was %p\n", &(bytes[11]), publish.payload);
        failedAssertions++;
    }

    if (publish.payloadSize != 3)
    {
        printf("Expected payloadSize to be 3 but was %i\n", publish.payloadSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldUnpackASubAckPacket()
{
    printf("It should unpack a SUBACK packet\n");
    failedAssertions = 0;

    uint8_t bytes[] = { 0x90, 0x03, 0xAA, 0xBB, 0x02 };
    struct MqttPacket packet;

    unpackMqttPacket(bytes, 5, &packet);

    struct MqttSubAckPacket subAck;
    unpackMqttSubAck(bytes, &packet, &subAck);

    if (subAck.packetIdentifier != 0xAABB)
    {
        printf("Expected packetIdentifier to be 0xAABB but was 0x%X\n", subAck.packetIdentifier);
        failedAssertions++;
    }

    if (subAck.returnCode != 0x02)
    {
        printf("Expected returnCode to be 0x02 but was 0x%X\n", subAck.returnCode);
        failedAssertions++;
    }
    
    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}
