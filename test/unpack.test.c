#include <stdio.h>
#include <stdint.h>
#include "../include/mqtt_packer.h"

uint32_t currentSize = 0;
uint16_t failedAssertions = 0;
uint8_t callbackCounter = 0;

void itShouldUnpackWithA2ByteHeaderAndAZeroLengthRemainingSizeCallback(struct MqttPacket *m, void *extraData)
{
    callbackCounter++;

    if (currentSize != 2)
    {
        printf("Callback: Expected currentSize to be 2 but was %i\n", currentSize);
        failedAssertions++;
    }

    if (m->size != 2)
    {
        printf("Callback: Expected size to be 2 but was %i\n", m->size);
        failedAssertions++;
    }

    if (m->fixedHeaderSize != 2)
    {
        printf("Callback: Expected fixedHeaderSize to be 2 but was %i\n", m->fixedHeaderSize);
        failedAssertions++;
    }
    
    if (m->type != 0x03)
    {
        printf("Callback: Expected type to be 0x03 but was 0x%X\n", m->type);
        failedAssertions++;
    }
        
    if (m->remainingSize != 0)
    {
        printf("Callback: Expected remainingSize to be 0 but was %i\n", m->remainingSize);
        failedAssertions++;
    }
}

static uint16_t itShouldUnpackWithA2ByteHeaderAndAZeroLengthRemainingSize()
{
    printf("It should unpack with a 2 byte header and a zero length remaining size\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[10] = { 0x30, 0x00 };
    struct MqttPacket m = { .bytes = bytes };

    currentSize = 2;
    uint32_t packetSize = unpackMqttChunk(&m, &currentSize, 2, itShouldUnpackWithA2ByteHeaderAndAZeroLengthRemainingSizeCallback, 0);

    if (callbackCounter != 1)
    {
        printf("Expected callbackCounter to be 1 but was %i\n", callbackCounter);
        failedAssertions++;
    }

    if (currentSize != 0)
    {
        printf("Expected currentSize to be 0 but was %i\n", currentSize);
        failedAssertions++;
    }

    if (packetSize != 0)
    {
        printf("Expected packetSize to be 0 but was %i\n", packetSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldUnpackWithA2ByteHeaderCallback(struct MqttPacket *m, void *extraData)
{
    callbackCounter++;

    if (currentSize != 3)
    {
        printf("Callback: Expected currentSize to be 3 but was %i\n", currentSize);
        failedAssertions++;
    }

    if (m->size != 3)
    {
        printf("Callback: Expected size to be 3 but was %i\n", m->size);
        failedAssertions++;
    }

    if (m->fixedHeaderSize != 2)
    {
        printf("Callback: Expected fixedHeaderSize to be 2 but was %i\n", m->fixedHeaderSize);
        failedAssertions++;
    }
    
    if (m->type != 0x03)
    {
        printf("Callback: Expected type to be 0x03 but was 0x%X\n", m->type);
        failedAssertions++;
    }

    if (m->remainingSize != 1)
    {
        printf("Callback: Expected remainingSize to be 1 but was %i\n", m->remainingSize);
        failedAssertions++;
    }
}

static uint16_t itShouldUnpackWithA2ByteHeader()
{
    printf("It should unpack with a 2 byte header\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[10] = { 0x30, 0x01, 0xFF };
    struct MqttPacket m = { .bytes = bytes };

    currentSize = 3;
    uint32_t packetSize = unpackMqttChunk(&m, &currentSize, (int32_t) 3, itShouldUnpackWithA2ByteHeaderCallback, 0);

    if (callbackCounter != 1)
    {
        printf("Expected callbackCounter to be 1 but was %i\n", callbackCounter);
        failedAssertions++;
    }

    if (currentSize != 0)
    {
        printf("Expected currentSize to be 0 but was %i\n", currentSize);
        failedAssertions++;
    }

    if (packetSize != 0)
    {
        printf("Expected packetSize to be 0 but was %i\n", packetSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldUnpackWithA3ByteHeaderCallback(struct MqttPacket *m, void *extraData)
{
    callbackCounter++;
}

static uint16_t itShouldUnpackWithA3ByteHeader()
{
    printf("It should unpack with a 3 byte header\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[10] = { 0x30, 0x81, 0x02, 0xFF };
    struct MqttPacket m = { .bytes = bytes };

    currentSize = 4;
    int32_t packetSize = unpackMqttChunk(&m, &currentSize, (int32_t) 4, itShouldUnpackWithA3ByteHeaderCallback, 0);

    if (callbackCounter != 0)
    {
        printf("Expected callbackCounter to be 0 but was %i\n", callbackCounter);
        failedAssertions++;
    }

    if (currentSize != 4)
    {
        printf("Expected currentSize to be 4 but was %i\n", currentSize);
        failedAssertions++;
    }

    if (packetSize != 260)
    {
        printf("Expected packetSize to be 260 but was %i\n", packetSize);
        failedAssertions++;
    }

    if (m.size != 4)
    {
        printf("Expected size to be 4 but was %i\n", m.size);
        failedAssertions++;
    }

    if (m.fixedHeaderSize != 3)
    {
        printf("Expected fixedHeaderSize to be 3 but was %i\n", m.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (m.type != 0x03)
    {
        printf("Expected type to be 0x03 but was 0x%X\n", m.type);
        failedAssertions++;
    }

    if (m.remainingSize != 257)
    {
        printf("Expected remainingSize to be 257 but was %i\n", m.remainingSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldUnpackWithA4ByteHeaderCallback(struct MqttPacket *m, void *extraData)
{
    callbackCounter++;
}

static uint16_t itShouldUnpackWithA4ByteHeader()
{
    printf("It should unpack with a 4 byte header\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[10] = { 0x30, 0x81, 0x82, 0x03, 0xFF };
    struct MqttPacket m = { .bytes = bytes };

    currentSize = 5;
    int32_t packetSize = unpackMqttChunk(&m, &currentSize, (int32_t) 5, itShouldUnpackWithA4ByteHeaderCallback, 0);

    if (callbackCounter != 0)
    {
        printf("Expected callbackCounter to be 0 but was %i\n", callbackCounter);
        failedAssertions++;
    }

    if (currentSize != 5)
    {
        printf("Expected currentSize to be 5 but was %i\n", currentSize);
        failedAssertions++;
    }

    if (packetSize != 49413)
    {
        printf("Expected packetSize to be 49413 but was %i\n", packetSize);
        failedAssertions++;
    }

    if (m.size != 5)
    {
        printf("Expected size to be 5 but was %i\n", m.size);
        failedAssertions++;
    }

    if (m.fixedHeaderSize != 4)
    {
        printf("Expected fixedHeaderSize to be 4 but was %i\n", m.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (m.type != 0x03)
    {
        printf("Expected type to be 0x03 but was 0x%X\n", m.type);
        failedAssertions++;
    }

    if (m.remainingSize != 49409)
    {
        printf("Expected remainingSize to be 49409 but was %i\n", m.remainingSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldUnpackWithA5ByteHeaderCallback(struct MqttPacket *m, void *extraData)
{
    callbackCounter++;
}

static uint16_t itShouldUnpackWithA5ByteHeader()
{
    printf("It should unpack with a 5 byte header\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[10] = { 0x30, 0x81, 0x82, 0x83, 0x04, 0xFF };
    struct MqttPacket m = { .bytes = bytes };

    currentSize = 6;
    int32_t packetSize = unpackMqttChunk(&m, &currentSize, (int32_t) 6, itShouldUnpackWithA5ByteHeaderCallback, 0);

    if (callbackCounter != 0)
    {
        printf("Expected callbackCounter to be 1 but was %i\n", callbackCounter);
        failedAssertions++;
    }

    if (currentSize != 6)
    {
        printf("Expected currentSize to be 6 but was %i\n", currentSize);
        failedAssertions++;
    }

    if (packetSize != 8438022)
    {
        printf("Expected packetSize to be 8438022 but was %i\n", packetSize);
        failedAssertions++;
    }

    if (m.size != 6)
    {
        printf("Expected size to be 5 but was %i\n", m.size);
        failedAssertions++;
    }

    if (m.fixedHeaderSize != 5)
    {
        printf("Expected fixedHeaderSize to be 5 but was %i\n", m.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (m.type != 0x03)
    {
        printf("Expected type to be 0x03 but was 0x%X\n", m.type);
        failedAssertions++;
    }

    if (m.remainingSize != 8438017)
    {
        printf("Expected remainingSize to be 8438017 but was %i\n", m.remainingSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldUnpackWithA5ByteHeaderGivenThrough1ByteChunksCallback(struct MqttPacket *m, void *extraData)
{
    callbackCounter++;
}

static uint16_t itShouldUnpackWithA5ByteHeaderGivenThrough1ByteChunks()
{
    printf("It should unpack with a 5 byte header given through 1 byte chunks\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[10] = { 0x30, 0x81, 0x82, 0x83, 0x04, 0xFF };
    struct MqttPacket m = { .bytes = bytes };

    currentSize = 1;
    int32_t packetSize = unpackMqttChunk(&m, &currentSize, (int32_t) 1, itShouldUnpackWithA5ByteHeaderGivenThrough1ByteChunksCallback, 0);

    if (callbackCounter != 0)
    {
        printf("Byte 1: Expected callbackCounter to be 0 but was %i\n", callbackCounter);
        failedAssertions++;
    }

    if (currentSize != 1)
    {
        printf("Byte 1: Expected currentSize to be 1 but was %i\n", currentSize);
        failedAssertions++;
    }

    if (packetSize != 2)
    {
        printf("Byte 1: Expected packetSize to be 2 but was %i\n", packetSize);
        failedAssertions++;
    }

    if (m.size != 1)
    {
        printf("Byte 1: Expected size to be 1 but was %i\n", m.size);
        failedAssertions++;
    }

    if (m.fixedHeaderSize != 2)
    {
        printf("Byte 1: Expected fixedHeaderSize to be 2 but was %i\n", m.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (m.type != 0x03)
    {
        printf("Byte 1: Expected type to be 0x03 but was 0x%X\n", m.type);
        failedAssertions++;
    }

    if (m.remainingSize != 0)
    {
        printf("Byte 1: Expected remainingSize to be 0 but was %i\n", m.remainingSize);
        failedAssertions++;
    }

    callbackCounter = 0;
    currentSize = 2;
    packetSize = unpackMqttChunk(&m, &currentSize, (int32_t) 1, itShouldUnpackWithA5ByteHeaderGivenThrough1ByteChunksCallback, 0);

    if (callbackCounter != 0)
    {
        printf("Byte 2: Expected callbackCounter to be 0 but was %i\n", callbackCounter);
        failedAssertions++;
    }

    if (currentSize != 2)
    {
        printf("Byte 2: Expected currentSize to be 2 but was %i\n", currentSize);
        failedAssertions++;
    }

    if (packetSize != 4)
    {
        printf("Byte 2: Expected packetSize to be 4 but was %i\n", packetSize);
        failedAssertions++;
    }

    if (m.size != 2)
    {
        printf("Byte 2: Expected size to be 2 but was %i\n", m.size);
        failedAssertions++;
    }

    if (m.fixedHeaderSize != 3)
    {
        printf("Byte 2: Expected fixedHeaderSize to be 3 but was %i\n", m.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (m.type != 0x03)
    {
        printf("Byte 2: Expected type to be 0x03 but was 0x%X\n", m.type);
        failedAssertions++;
    }

    if (m.remainingSize != 1)
    {
        printf("Byte 2: Expected remainingSize to be 1 but was %i\n", m.remainingSize);
        failedAssertions++;
    }

    callbackCounter = 0;
    currentSize = 3;
    packetSize = unpackMqttChunk(&m, &currentSize, (int32_t) 1, itShouldUnpackWithA5ByteHeaderGivenThrough1ByteChunksCallback, 0);

    if (callbackCounter != 0)
    {
        printf("Byte 3: Expected callbackCounter to be 0 but was %i\n", callbackCounter);
        failedAssertions++;
    }

    if (currentSize != 3)
    {
        printf("Byte 3: Expected currentSize to be 3 but was %i\n", currentSize);
        failedAssertions++;
    }

    if (packetSize != 261)
    {
        printf("Byte 3: Expected packetSize to be 261 but was %i\n", packetSize);
        failedAssertions++;
    }

    if (m.size != 3)
    {
        printf("Byte 3: Expected size to be 3 but was %i\n", m.size);
        failedAssertions++;
    }

    if (m.fixedHeaderSize != 4)
    {
        printf("Byte 3: Expected fixedHeaderSize to be 4 but was %i\n", m.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (m.type != 0x03)
    {
        printf("Byte 3: Expected type to be 0x03 but was 0x%X\n", m.type);
        failedAssertions++;
    }

    if (m.remainingSize != 257)
    {
        printf("Byte 3: Expected remainingSize to be 257 but was %i\n", m.remainingSize);
        failedAssertions++;
    }

    callbackCounter = 0;
    currentSize = 4;
    packetSize = unpackMqttChunk(&m, &currentSize, (int32_t) 1, itShouldUnpackWithA5ByteHeaderGivenThrough1ByteChunksCallback, 0);

    if (callbackCounter != 0)
    {
        printf("Byte 4: Expected callbackCounter to be 0 but was %i\n", callbackCounter);
        failedAssertions++;
    }

    if (currentSize != 4)
    {
        printf("Byte 4: Expected currentSize to be 4 but was %i\n", currentSize);
        failedAssertions++;
    }

    if (packetSize != 49414)
    {
        printf("Byte 4: Expected packetSize to be 49414 but was %i\n", packetSize);
        failedAssertions++;
    }

    if (m.size != 4)
    {
        printf("Byte 4: Expected size to be 4 but was %i\n", m.size);
        failedAssertions++;
    }

    if (m.fixedHeaderSize != 5)
    {
        printf("Byte 4: Expected fixedHeaderSize to be 5 but was %i\n", m.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (m.remainingSize != 49409)
    {
        printf("Byte 4: Expected remainingSize to be 49409 but was %i\n", m.remainingSize);
        failedAssertions++;
    }

    if (m.type != 0x03)
    {
        printf("Byte 4: Expected type to be 0x03 but was 0x%X\n", m.type);
        failedAssertions++;
    }

    callbackCounter = 0;
    currentSize = 5;
    packetSize = unpackMqttChunk(&m, &currentSize, (int32_t) 1, itShouldUnpackWithA5ByteHeaderGivenThrough1ByteChunksCallback, 0);

    if (callbackCounter != 0)
    {
        printf("Byte 5: Expected callbackCounter to be 0 but was %i\n", callbackCounter);
        failedAssertions++;
    }

    if (currentSize != 5)
    {
        printf("Byte 5: Expected currentSize to be 5 but was %i\n", currentSize);
        failedAssertions++;
    }

    if (packetSize != 8438022)
    {
        printf("Byte 5: Expected packetSize to be 8438022 but was %i\n", packetSize);
        failedAssertions++;
    }

    if (m.size != 5)
    {
        printf("Byte 5: Expected size to be 5 but was %i\n", m.size);
        failedAssertions++;
    }

    if (m.fixedHeaderSize != 5)
    {
        printf("Byte 5: Expected fixedHeaderSize to be 5 but was %i\n", m.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (m.type != 0x03)
    {
        printf("Byte 5: Expected type to be 0x03 but was 0x%X\n", m.type);
        failedAssertions++;
    }

    if (m.remainingSize != 8438017)
    {
        printf("Byte 5: Expected remainingSize to be 8438017 but was %i\n", m.remainingSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldUnpackMultiplePacketsCallback(struct MqttPacket *m, void *extraData)
{
    callbackCounter++;

    if (callbackCounter == 1)
    {
        if (m->size != 2)
        {
            printf("Callback 1: Expected size to be 2 but was %i\n", m->size);
            failedAssertions++;
        }

        if (m->fixedHeaderSize != 2)
        {
            printf("Callback 1: Expected fixedHeaderSize to be 2 but was %i\n", m->fixedHeaderSize);
            failedAssertions++;
        }
        
        if (m->type != 0x0C)
        {
            printf("Callback 1: Expected type to be 0x0C but was 0x%X\n", m->type);
            failedAssertions++;
        }
            
        if (m->remainingSize != 0)
        {
            printf("Callback 1: Expected remainingSize to be 0 but was %i\n", m->remainingSize);
            failedAssertions++;
        }
    }

    if (callbackCounter == 2)
    {
        if (m->size != 2)
        {
            printf("Callback 2: Expected size to be 2 but was %i\n", m->size);
            failedAssertions++;
        }

        if (m->fixedHeaderSize != 2)
        {
            printf("Callback 2: Expected fixedHeaderSize to be 2 but was %i\n", m->fixedHeaderSize);
            failedAssertions++;
        }
        
        if (m->type != 0x0D)
        {
            printf("Callback 2: Expected type to be 0x0D but was 0x%X\n", m->type);
            failedAssertions++;
        }
            
        if (m->remainingSize != 0)
        {
            printf("Callback 2: Expected remainingSize to be 0 but was %i\n", m->remainingSize);
            failedAssertions++;
        }
    }
}

static uint16_t itShouldUnpackMultiplePackets()
{
    printf("It should unpack multiple packets\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[] = { 0xC0, 0x00, 0xD0, 0x00, 0x40, 0x02, 0xAA };
    struct MqttPacket m = { .bytes = bytes };

    currentSize = 7;
    int32_t packetSize = unpackMqttChunk(&m, &currentSize, (int32_t) 7, itShouldUnpackMultiplePacketsCallback, 0);

    if (callbackCounter != 2)
    {
        printf("Expected callbackCounter to be 2 but was %i\n", callbackCounter);
        failedAssertions++;
    }
    
    if (currentSize != 3)
    {
        printf("Expected currentSize to be 0 but was %i\n", currentSize);
        failedAssertions++;
    }

    if (packetSize != 4)
    {
        printf("Expected packetSize to be 0 but was %i\n", packetSize);
        failedAssertions++;
    }

    if (m.size != 3)
    {
        printf("Expected size to be 3 but was %i\n", m.size);
        failedAssertions++;
    }

    if (m.fixedHeaderSize != 2)
    {
        printf("Expected fixedHeaderSize to be 2 but was %i\n", m.fixedHeaderSize);
        failedAssertions++;
    }
    
    if (m.type != 0x04)
    {
        printf("Expected type to be 0x04 but was 0x%X\n", m.type);
        failedAssertions++;
    }
        
    if (m.remainingSize != 2)
    {
        printf("Expected remainingSize to be 2 but was %i\n", m.remainingSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldMoveTheBytesOfANewPacketToTheBeginningCallback(struct MqttPacket *m, void *extraData)
{
    callbackCounter++;

    if (callbackCounter == 2)
    {
        if (m->size != 4)
        {
            printf("Callback: Expected size to be 4 but was %i\n", m->size);
            failedAssertions++;
        }

        if (m->fixedHeaderSize != 2)
        {
            printf("Callback: Expected fixedHeaderSize to be 2 but was %i\n", m->fixedHeaderSize);
            failedAssertions++;
        }
        
        if (m->type != 0x03)
        {
            printf("Callback: Expected type to be 0x03 but was 0x%X\n", m->type);
            failedAssertions++;
        }

        if (m->remainingSize != 2)
        {
            printf("Callback: Expected remainingSize to be 2 but was %i\n", m->remainingSize);
            failedAssertions++;
        }
    }
}

static uint16_t itShouldMoveTheBytesOfANewPacketToTheBeginning()
{
    printf("It should move the bytes of a new packet to the beginning\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[] = { 0x30, 0x01, 0xAA, 0x30, 0x02, 0xBB, 0xBB };
    struct MqttPacket m = { .bytes = bytes };

    currentSize = 7;
    int32_t packetSize = unpackMqttChunk(&m, &currentSize, (int32_t) 7, itShouldMoveTheBytesOfANewPacketToTheBeginningCallback, 0);

    if (callbackCounter != 2)
    {
        printf("Expected callbackCounter to be 2 but was %i\n", callbackCounter);
        failedAssertions++;
    }
    
    if (currentSize != 0)
    {
        printf("Expected currentSize to be 0 but was %i\n", currentSize);
        failedAssertions++;
    }

    if (packetSize != 0)
    {
        printf("Expected packetSize to be 0 but was %i\n", packetSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldUnpackThePacketIdentifierWhichIsAfterTheFixedHeaderCallback(struct MqttPacket *m, void *extraData)
{
    callbackCounter++;
    uint16_t packetIdentifier = 0;

    unpackMqttPacketIdentifier(m, &packetIdentifier);

    if (packetIdentifier != 0xAABB)
    {
        printf("Expected packetIdentifier to be 0xAABB but was 0x%X\n", packetIdentifier);
        failedAssertions++;
    }
}

static uint16_t itShouldUnpackThePacketIdentifierWhichIsAfterTheFixedHeader()
{
    printf("It should unpack the packet identifier which is after the fixed header\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[] = { 0x40, 0x02, 0xAA, 0xBB };
    struct MqttPacket m = { .bytes = bytes };

    currentSize = 4;
    int32_t packetSize = unpackMqttChunk(&m, &currentSize, (int32_t) 4, itShouldUnpackThePacketIdentifierWhichIsAfterTheFixedHeaderCallback, 0);

    if (callbackCounter != 1)
    {
        printf("Expected callbackCounter to be 1 but was %i\n", callbackCounter);
        failedAssertions++;
    }
    
    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldUnpackAConnAckPacketCallback(struct MqttPacket *m, void *extraData)
{
    callbackCounter++;
    struct MqttConnAckPacket connAck;

    unpackMqttConnAck(m, &connAck);

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
}

static uint16_t itShouldUnpackAConnAckPacket()
{
    printf("It should unpack a CONNACK packet\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[] = { 0x20, 0x02, 0x01, 0x02 };
    struct MqttPacket m = { .bytes = bytes };

    currentSize = 4;
    int32_t packetSize = unpackMqttChunk(&m, &currentSize, (int32_t) 4, itShouldUnpackAConnAckPacketCallback, 0);

    if (callbackCounter != 1)
    {
        printf("Expected callbackCounter to be 1 but was %i\n", callbackCounter);
        failedAssertions++;
    }
    
    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldUnpackAPublishPacketCallback(struct MqttPacket *m, void *extraData)
{
    callbackCounter++;
    struct MqttPublishPacket publish;

    unpackMqttPublish(m, &publish);

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

    if (publish.topicName != &(m->bytes[4]))
    {
        printf("Expected topicName to be %p but was %p\n", &(m->bytes[4]), publish.topicName);
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

    if (publish.payload != &(m->bytes[11]))
    {
        printf("Expected payload to be %p but was %p\n", &(m->bytes[11]), publish.payload);
        failedAssertions++;
    }

    if (publish.payloadSize != 3)
    {
        printf("Expected payloadSize to be 3 but was %i\n", publish.payloadSize);
        failedAssertions++;
    }
}

static uint16_t itShouldUnpackAPublishPacket()
{
    printf("It should unpack a PUBLISH packet\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[] = { 0x3D, 0x0C, 0x00, 0x05, 0x54, 0x4F, 0x50, 0x49, 0x43, 0xAA, 0xBB, 0x00, 0x01, 0x02 };
    struct MqttPacket m = { .bytes = bytes };

    currentSize = 14;
    int32_t packetSize = unpackMqttChunk(&m, &currentSize, (int32_t) 14, itShouldUnpackAPublishPacketCallback, 0);

    if (callbackCounter != 1)
    {
        printf("Expected callbackCounter to be 1 but was %i\n", callbackCounter);
        failedAssertions++;
    }
    
    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldUnpackASubAckPacketCallback(struct MqttPacket *m, void *extraData)
{
    callbackCounter++;
    struct MqttSubAckPacket subAck;

    unpackMqttSubAck(m, &subAck);

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
}

static uint16_t itShouldUnpackASubAckPacket()
{
    printf("It should unpack a SUBACK packet\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[] = { 0x90, 0x03, 0xAA, 0xBB, 0x02 };
    struct MqttPacket m = { .bytes = bytes };

    currentSize = 5;
    int32_t packetSize = unpackMqttChunk(&m, &currentSize, (int32_t) 5, itShouldUnpackASubAckPacketCallback, 0);

    if (callbackCounter != 1)
    {
        printf("Expected callbackCounter to be 1 but was %i\n", callbackCounter);
        failedAssertions++;
    }
    
    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}
