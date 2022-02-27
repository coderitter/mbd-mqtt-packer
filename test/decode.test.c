#include <stdio.h>
#include <stdint.h>
#include "../src/mqttCodec.h"

int32_t currentSize = 0;
uint16_t failedAssertions = 0;
uint8_t callbackCounter = 0;

void itShouldDecodeAMessageWithA2ByteHeaderAndAZeroLengthRemainingSizeCallback(struct MqttMessage *m)
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
    
    if (m->controlPacketType != 0x30)
    {
        printf("Callback: Expected controlPacketType to be 0x30 but was 0x%X\n", m->controlPacketType);
        failedAssertions++;
    }
        
    if (m->flags != 0x00)
    {
        printf("Callback: Expected flags to be 0x00 but was 0x%X\n", m->flags);
        failedAssertions++;
    }

    if (m->remainingSize != 0)
    {
        printf("Callback: Expected remainingSize to be 0 but was %i\n", m->remainingSize);
        failedAssertions++;
    }
}

static uint16_t itShouldDecodeAMessageWithA2ByteHeaderAndAZeroLengthRemainingSize()
{
    printf("It should decode a message with a 2 byte header and a zero length remaining size\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[10] = { 0x30, 0x00 };
    struct MqttMessage m = { .bytes = bytes };

    currentSize = 2;
    uint32_t messageSize = decodeMqttChunk(&m, &currentSize, (int32_t) 2, itShouldDecodeAMessageWithA2ByteHeaderAndAZeroLengthRemainingSizeCallback);

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

    if (messageSize != 0)
    {
        printf("Expected messageSize to be 0 but was %i\n", messageSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldDecodeAMessageWithA2ByteHeaderCallback(struct MqttMessage *m)
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
    
    if (m->controlPacketType != 0x30)
    {
        printf("Callback: Expected controlPacketType to be 0x30 but was 0x%X\n", m->controlPacketType);
        failedAssertions++;
    }

    if (m->flags != 0x00)
    {
        printf("Callback: Expected flags to be 0x00 but was 0x%X\n", m->flags);
        failedAssertions++;
    }

    if (m->remainingSize != 1)
    {
        printf("Callback: Expected remainingSize to be 1 but was %i\n", m->remainingSize);
        failedAssertions++;
    }
}

static uint16_t itShouldDecodeAMessageWithA2ByteHeader()
{
    printf("It should decode a message with a 2 byte header\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[10] = { 0x30, 0x01, 0xFF };
    struct MqttMessage m = { .bytes = bytes };

    currentSize = 3;
    uint32_t messageSize = decodeMqttChunk(&m, &currentSize, (int32_t) 3, itShouldDecodeAMessageWithA2ByteHeaderCallback);

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

    if (messageSize != 0)
    {
        printf("Expected messageSize to be 0 but was %i\n", messageSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldDecodeAMessageWithA3ByteHeaderCallback(struct MqttMessage *m)
{
    callbackCounter++;
}

static uint16_t itShouldDecodeAMessageWithA3ByteHeader()
{
    printf("It should decode a message with a 3 byte header\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[10] = { 0x30, 0x81, 0x02, 0xFF };
    struct MqttMessage m = { .bytes = bytes };

    currentSize = 4;
    int32_t messageSize = decodeMqttChunk(&m, &currentSize, (int32_t) 4, itShouldDecodeAMessageWithA3ByteHeaderCallback);

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

    if (messageSize != 260)
    {
        printf("Expected messageSize to be 260 but was %i\n", messageSize);
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
    
    if (m.controlPacketType != 0x30)
    {
        printf("Expected controlPacketType to be 0x30 but was 0x%X\n", m.controlPacketType);
        failedAssertions++;
    }

    if (m.flags != 0x00)
    {
        printf("Expected flags to be 0x00 but was 0x%X\n", m.flags);
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

void itShouldDecodeAMessageWithA4ByteHeaderCallback(struct MqttMessage *m)
{
    callbackCounter++;
}

static uint16_t itShouldDecodeAMessageWithA4ByteHeader()
{
    printf("It should decode a message with a 4 byte header\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[10] = { 0x30, 0x81, 0x82, 0x03, 0xFF };
    struct MqttMessage m = { .bytes = bytes };

    currentSize = 5;
    int32_t messageSize = decodeMqttChunk(&m, &currentSize, (int32_t) 5, itShouldDecodeAMessageWithA4ByteHeaderCallback);

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

    if (messageSize != 49413)
    {
        printf("Expected messageSize to be 49413 but was %i\n", messageSize);
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
    
    if (m.controlPacketType != 0x30)
    {
        printf("Expected controlPacketType to be 0x30 but was 0x%X\n", m.controlPacketType);
        failedAssertions++;
    }

    if (m.flags != 0x00)
    {
        printf("Expected flags to be 0x00 but was 0x%X\n", m.flags);
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

void itShouldDecodeAMessageWithA5ByteHeaderCallback(struct MqttMessage *m)
{
    callbackCounter++;
}

static uint16_t itShouldDecodeAMessageWithA5ByteHeader()
{
    printf("It should decode a message with a 5 byte header\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[10] = { 0x30, 0x81, 0x82, 0x83, 0x04, 0xFF };
    struct MqttMessage m = { .bytes = bytes };

    currentSize = 6;
    int32_t messageSize = decodeMqttChunk(&m, &currentSize, (int32_t) 6, itShouldDecodeAMessageWithA5ByteHeaderCallback);

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

    if (messageSize != 8438022)
    {
        printf("Expected messageSize to be 8438022 but was %i\n", messageSize);
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
    
    if (m.controlPacketType != 0x30)
    {
        printf("Expected controlPacketType to be 0x30 but was 0x%X\n", m.controlPacketType);
        failedAssertions++;
    }

    if (m.flags != 0x00)
    {
        printf("Expected flags to be 0x00 but was 0x%X\n", m.flags);
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

void itShouldDecodeAMessageWithA5ByteHeaderGivenThrough1ByteChunksCallback(struct MqttMessage *m)
{
    callbackCounter++;
}

static uint16_t itShouldDecodeAMessageWithA5ByteHeaderGivenThrough1ByteChunks()
{
    printf("It should decode a message with a 5 byte header given through 1 byte chunks\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[10] = { 0x30, 0x81, 0x82, 0x83, 0x04, 0xFF };
    struct MqttMessage m = { .bytes = bytes };

    currentSize = 1;
    int32_t messageSize = decodeMqttChunk(&m, &currentSize, (int32_t) 1, itShouldDecodeAMessageWithA5ByteHeaderGivenThrough1ByteChunksCallback);

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

    if (messageSize != 2)
    {
        printf("Byte 1: Expected messageSize to be 2 but was %i\n", messageSize);
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
    
    if (m.controlPacketType != 0x30)
    {
        printf("Byte 1: Expected controlPacketType to be 0x30 but was 0x%X\n", m.controlPacketType);
        failedAssertions++;
    }

    if (m.flags != 0x00)
    {
        printf("Byte 1: Expected flags to be 0x00 but was 0x%X\n", m.flags);
        failedAssertions++;
    }

    if (m.remainingSize != 0)
    {
        printf("Byte 1: Expected remainingSize to be 0 but was %i\n", m.remainingSize);
        failedAssertions++;
    }

    callbackCounter = 0;
    currentSize = 2;
    messageSize = decodeMqttChunk(&m, &currentSize, (int32_t) 1, itShouldDecodeAMessageWithA5ByteHeaderGivenThrough1ByteChunksCallback);

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

    if (messageSize != 4)
    {
        printf("Byte 2: Expected messageSize to be 4 but was %i\n", messageSize);
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
    
    if (m.controlPacketType != 0x30)
    {
        printf("Byte 2: Expected controlPacketType to be 0x30 but was 0x%X\n", m.controlPacketType);
        failedAssertions++;
    }

    if (m.flags != 0x00)
    {
        printf("Byte 2: Expected flags to be 0x00 but was 0x%X\n", m.flags);
        failedAssertions++;
    }

    if (m.remainingSize != 1)
    {
        printf("Byte 2: Expected remainingSize to be 1 but was %i\n", m.remainingSize);
        failedAssertions++;
    }

    callbackCounter = 0;
    currentSize = 3;
    messageSize = decodeMqttChunk(&m, &currentSize, (int32_t) 1, itShouldDecodeAMessageWithA5ByteHeaderGivenThrough1ByteChunksCallback);

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

    if (messageSize != 261)
    {
        printf("Byte 3: Expected messageSize to be 261 but was %i\n", messageSize);
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
    
    if (m.controlPacketType != 0x30)
    {
        printf("Byte 3: Expected controlPacketType to be 0x30 but was 0x%X\n", m.controlPacketType);
        failedAssertions++;
    }

    if (m.flags != 0x00)
    {
        printf("Byte 3: Expected flags to be 0x00 but was 0x%X\n", m.flags);
        failedAssertions++;
    }

    if (m.remainingSize != 257)
    {
        printf("Byte 3: Expected remainingSize to be 257 but was %i\n", m.remainingSize);
        failedAssertions++;
    }

    callbackCounter = 0;
    currentSize = 4;
    messageSize = decodeMqttChunk(&m, &currentSize, (int32_t) 1, itShouldDecodeAMessageWithA5ByteHeaderGivenThrough1ByteChunksCallback);

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

    if (messageSize != 49414)
    {
        printf("Byte 4: Expected messageSize to be 49414 but was %i\n", messageSize);
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

    if (m.controlPacketType != 0x30)
    {
        printf("Byte 4: Expected controlPacketType to be 0x30 but was 0x%X\n", m.controlPacketType);
        failedAssertions++;
    }

    if (m.flags != 0x00)
    {
        printf("Byte 4: Expected flags to be 0x00 but was 0x%X\n", m.flags);
        failedAssertions++;
    }

    callbackCounter = 0;
    currentSize = 5;
    messageSize = decodeMqttChunk(&m, &currentSize, (int32_t) 1, itShouldDecodeAMessageWithA5ByteHeaderGivenThrough1ByteChunksCallback);

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

    if (messageSize != 8438022)
    {
        printf("Byte 5: Expected messageSize to be 8438022 but was %i\n", messageSize);
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
    
    if (m.controlPacketType != 0x30)
    {
        printf("Byte 5: Expected controlPacketType to be 0x30 but was 0x%X\n", m.controlPacketType);
        failedAssertions++;
    }

    if (m.flags != 0x00)
    {
        printf("Byte 5: Expected flags to be 0x00 but was 0x%X\n", m.flags);
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

void itShouldMoveTheBytesOfANewMessageToTheBeginningCallback(struct MqttMessage *m)
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
        
        if (m->controlPacketType != 0x30)
        {
            printf("Callback: Expected controlPacketType to be 0x30 but was 0x%X\n", m->controlPacketType);
            failedAssertions++;
        }

        if (m->flags != 0x00)
        {
            printf("Callback: Expected flags to be 0x00 but was 0x%X\n", m->flags);
            failedAssertions++;
        }

        if (m->remainingSize != 2)
        {
            printf("Callback: Expected remainingSize to be 2 but was %i\n", m->remainingSize);
            failedAssertions++;
        }
    }
}

static uint16_t itShouldMoveTheBytesOfANewMessageToTheBeginning()
{
    printf("It should move the bytes of a new message to the beginning\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[] = { 0x30, 0x01, 0xAA, 0x30, 0x02, 0xBB, 0xBB };
    struct MqttMessage m = { .bytes = bytes };

    currentSize = 7;
    int32_t messageSize = decodeMqttChunk(&m, &currentSize, (int32_t) 7, itShouldMoveTheBytesOfANewMessageToTheBeginningCallback);

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

    if (messageSize != 0)
    {
        printf("Expected messageSize to be 0 but was %i\n", messageSize);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldDecodeThePacketIdentifierWhichIsAfterTheFixedHeaderCallback(struct MqttMessage *m)
{
    callbackCounter++;
    uint16_t packetIdentifier = 0;

    decodeMqttPacketIdentifier(m, &packetIdentifier);

    if (packetIdentifier != 0xAABB)
    {
        printf("Expected packetIdentifier to be 0xAABB but was 0x%X\n", packetIdentifier);
        failedAssertions++;
    }
}

static uint16_t itShouldDecodeThePacketIdentifierWhichIsAfterTheFixedHeader()
{
    printf("It should decode the packet identifier which is after the fixed header\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[] = { 0x40, 0x02, 0xAA, 0xBB };
    struct MqttMessage m = { .bytes = bytes };

    currentSize = 4;
    int32_t messageSize = decodeMqttChunk(&m, &currentSize, (int32_t) 4, itShouldDecodeThePacketIdentifierWhichIsAfterTheFixedHeaderCallback);

    if (callbackCounter != 1)
    {
        printf("Expected callbackCounter to be 1 but was %i\n", callbackCounter);
        failedAssertions++;
    }
    
    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldDecodeThePacketIdentifierOfPublishCallback(struct MqttMessage *m)
{
    callbackCounter++;
    uint16_t packetIdentifier = 0;

    decodeMqttPacketIdentifier(m, &packetIdentifier);

    if (packetIdentifier != 0xAABB)
    {
        printf("Expected packetIdentifier to be 0xAABB but was 0x%X\n", packetIdentifier);
        failedAssertions++;
    }
}

static uint16_t itShouldDecodeThePacketIdentifierOfPublish()
{
    printf("It should decode the packet identifier of PUBLISH\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[] = { 0x30, 0x05, 0x00, 0x01, 0x64, 0xAA, 0xBB };
    struct MqttMessage m = { .bytes = bytes };

    currentSize = 7;
    int32_t messageSize = decodeMqttChunk(&m, &currentSize, (int32_t) 7, itShouldDecodeThePacketIdentifierOfPublishCallback);

    if (callbackCounter != 1)
    {
        printf("Expected callbackCounter to be 1 but was %i\n", callbackCounter);
        failedAssertions++;
    }
    
    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldDecodeThePublishTopicNameCallback(struct MqttMessage *m)
{
    callbackCounter++;
    uint8_t *topicName = 0;
    uint16_t topicNameSize = 0;

    decodeMqttPublishTopicName(m, &topicName, &topicNameSize);

    if (topicName != &(m->bytes[4]))
    {
        printf("Expected topicName to be %p but was %p\n", &(m->bytes[4]), topicName);
        failedAssertions++;
    }

    if (topicNameSize != 5)
    {
        printf("Expected topicNameSize to be 5 but was %i\n", topicNameSize);
        failedAssertions++;
    }
}

static uint16_t itShouldDecodeThePublishTopicName()
{
    printf("It should decode the packet identifier of PUBLISH\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[] = { 0x30, 0x09, 0x00, 0x05, 0x54, 0x4F, 0x50, 0x49, 0x43, 0xAA, 0xBB };
    struct MqttMessage m = { .bytes = bytes };

    currentSize = 11;
    int32_t messageSize = decodeMqttChunk(&m, &currentSize, (int32_t) 11, itShouldDecodeThePublishTopicNameCallback);

    if (callbackCounter != 1)
    {
        printf("Expected callbackCounter to be 1 but was %i\n", callbackCounter);
        failedAssertions++;
    }
    
    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

void itShouldDecodeThePublishPayloadCallback(struct MqttMessage *m)
{
    callbackCounter++;
    uint8_t *payload = 0;
    uint16_t payloadSize = 0;

    decodeMqttPublishPayload(m, &payload, &payloadSize);

    if (payload != &(m->bytes[11]))
    {
        printf("Expected payload to be %p but was %p\n", &(m->bytes[11]), payload);
        failedAssertions++;
    }

    if (payloadSize != 3)
    {
        printf("Expected payloadSize to be 3 but was %i\n", payloadSize);
        failedAssertions++;
    }
}

static uint16_t itShouldDecodeThePublishPayload()
{
    printf("It should decode the packet identifier of PUBLISH\n");
    failedAssertions = 0;
    callbackCounter = 0;

    uint8_t bytes[] = { 0x30, 0x0C, 0x00, 0x05, 0x54, 0x4F, 0x50, 0x49, 0x43, 0xAA, 0xBB, 0x00, 0x01, 0x02 };
    struct MqttMessage m = { .bytes = bytes };

    currentSize = 14;
    int32_t messageSize = decodeMqttChunk(&m, &currentSize, (int32_t) 14, itShouldDecodeThePublishPayloadCallback);

    if (callbackCounter != 1)
    {
        printf("Expected callbackCounter to be 1 but was %i\n", callbackCounter);
        failedAssertions++;
    }
    
    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

