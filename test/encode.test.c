#include <stdio.h>
#include <stdint.h>
#include "../src/mqttCodec.h"

static uint16_t itShouldEncodeConnect()
{
    printf("It should encode connect\n");
    uint16_t failedAssertions = 0;

    uint8_t clientIdentifier[] = "C";
    uint8_t willTopic[] = "TO";
    uint8_t willMessage[] = "MES";
    uint8_t userName[] = "USER";
    uint8_t password[] = "PASSW";

    struct MqttConnectParameter parameter = {
        .clientIdentifier = clientIdentifier,
        .clientIdentifierSize = 1,
        .cleanSession = 1,
        .willTopic = willTopic,
        .willTopicSize = 2,
        .willMessage = willMessage,
        .willMessageSize = 3,
        .willQos = MQTT_CONNECT_VARIABLE_HEADER_CONNECT_FLAG_WILL_QOS_EXACTLY_ONCE,
        .willRetain = 1,
        .userName = userName,
        .userNameSize = 4,
        .password = password,
        .passwordSize = 5,
        .keepAlive = 0xAABB
    };

    uint8_t bytes[37];
    uint32_t size = encodeMqttConnect(&parameter, bytes);

    if (size != 37)
    {
        printf("Expected size to be 37 but was %i\n", size);
        failedAssertions++;
    }

    // Fixed header - Packet type
    if (bytes[0] != 0x10)
    {
        printf("Expected byte[0] to be 0x10 but was 0x%X\n", bytes[0]);
        failedAssertions++;
    }

    // Fixed header - Remaining length
    if (bytes[1] != 35)
    {
        printf("Expected byte[1] to be 35 but was %i\n", bytes[1]);
        failedAssertions++;
    }

    // Variable header - Protocol name - Length MSB
    if (bytes[2] != 0x00)
    {
        printf("Expected byte[2] to be 0x00 but was 0x%X\n", bytes[2]);
        failedAssertions++;
    }

    // Variable header - Protocol name - Length LSB
    if (bytes[3] != 0x04)
    {
        printf("Expected byte[3] to be 0x04 but was 0x%X\n", bytes[3]);
        failedAssertions++;
    }

    // Variable header - Protocol name - M
    if (bytes[4] != 0x4D)
    {
        printf("Expected byte[4] to be 0x4D but was 0x%X\n", bytes[4]);
        failedAssertions++;
    }

    // Variable header - Protocol name - Q
    if (bytes[5] != 0x51)
    {
        printf("Expected byte[5] to be 0x51 but was 0x%X\n", bytes[5]);
        failedAssertions++;
    }

    // Variable header - Protocol name - Q
    if (bytes[6] != 0x54)
    {
        printf("Expected byte[6] to be 0x54 but was 0x%X\n", bytes[6]);
        failedAssertions++;
    }

    // Variable header - Protocol name - Q
    if (bytes[7] != 0x54)
    {
        printf("Expected byte[7] to be 0x54 but was 0x%X\n", bytes[7]);
        failedAssertions++;
    }

    // Variable header - Protocol version
    if (bytes[8] != 0x04)
    {
        printf("Expected byte[8] to be 0x04 but was 0x%X\n", bytes[8]);
        failedAssertions++;
    }

    // Variable header - Connect flags
    if (bytes[9] != 0xF6)
    {
        printf("Expected byte[9] to be 0xF6 but was 0x%X\n", bytes[9]);
        failedAssertions++;
    }

    // Variable header - Keep alive MSB
    if (bytes[10] != 0xAA)
    {
        printf("Expected byte[10] to be 0xAA but was 0x%X\n", bytes[10]);
        failedAssertions++;
    }

    // Variable header - Keep alive LSB
    if (bytes[11] != 0xBB)
    {
        printf("Expected byte[11] to be 0xBB but was 0x%X\n", bytes[11]);
        failedAssertions++;
    }

    // Payload - Client identifier - Length MSB
    if (bytes[12] != 0x00)
    {
        printf("Expected byte[12] to be 0x00 but was 0x%X\n", bytes[12]);
        failedAssertions++;
    }

    // Payload - Client identifier - Length LSB
    if (bytes[13] != 0x01)
    {
        printf("Expected byte[13] to be 0x01 but was 0x%X\n", bytes[13]);
        failedAssertions++;
    }

    // Payload - Client identifier - C
    if (bytes[14] != 0x43)
    {
        printf("Expected byte[14] to be 0x43 but was 0x%X\n", bytes[14]);
        failedAssertions++;
    }

    // Payload - Will topic - Length MSB
    if (bytes[15] != 0x00)
    {
        printf("Expected byte[15] to be 0x00 but was 0x%X\n", bytes[15]);
        failedAssertions++;
    }

    // Payload - Will topic - Length LSB
    if (bytes[16] != 0x02)
    {
        printf("Expected byte[16] to be 0x02 but was 0x%X\n", bytes[16]);
        failedAssertions++;
    }

    // Payload - Will topic - T
    if (bytes[17] != 0x54)
    {
        printf("Expected byte[17] to be 0x54 but was 0x%X\n", bytes[17]);
        failedAssertions++;
    }

    // Payload - Will topic - O
    if (bytes[18] != 0x4F)
    {
        printf("Expected byte[18] to be 0x4F but was 0x%X\n", bytes[18]);
        failedAssertions++;
    }

    // Payload - Will message - Length MSB
    if (bytes[19] != 0x00)
    {
        printf("Expected byte[19] to be 0x00 but was 0x%X\n", bytes[19]);
        failedAssertions++;
    }

    // Payload - Will message - Length LSB
    if (bytes[20] != 0x03)
    {
        printf("Expected byte[20] to be 0x03 but was 0x%X\n", bytes[20]);
        failedAssertions++;
    }

    // Payload - Will message - M
    if (bytes[21] != 0x4D)
    {
        printf("Expected byte[21] to be 0x4D but was 0x%X\n", bytes[21]);
        failedAssertions++;
    }

    // Payload - Will message - E
    if (bytes[22] != 0x45)
    {
        printf("Expected byte[22] to be 0x45 but was 0x%X\n", bytes[22]);
        failedAssertions++;
    }

    // Payload - Will message - S
    if (bytes[23] != 0x53)
    {
        printf("Expected byte[23] to be 0x53 but was 0x%X\n", bytes[23]);
        failedAssertions++;
    }

    // Payload - User name - Length MSB
    if (bytes[24] != 0x00)
    {
        printf("Expected byte[24] to be 0x00 but was 0x%X\n", bytes[24]);
        failedAssertions++;
    }

    // Payload - User name - Length LSB
    if (bytes[25] != 0x04)
    {
        printf("Expected byte[25] to be 0x04 but was 0x%X\n", bytes[25]);
        failedAssertions++;
    }

    // Payload - User name - U
    if (bytes[26] != 0x55)
    {
        printf("Expected byte[26] to be 0x55 but was 0x%X\n", bytes[26]);
        failedAssertions++;
    }

    // Payload - User name - S
    if (bytes[27] != 0x53)
    {
        printf("Expected byte[27] to be 0x53 but was 0x%X\n", bytes[27]);
        failedAssertions++;
    }

    // Payload - User name - E
    if (bytes[28] != 0x45)
    {
        printf("Expected byte[28] to be 0x45 but was 0x%X\n", bytes[28]);
        failedAssertions++;
    }

    // Payload - User name - R
    if (bytes[29] != 0x52)
    {
        printf("Expected byte[29] to be 0x52 but was 0x%X\n", bytes[29]);
        failedAssertions++;
    }

    // Payload - Password - Length MSB
    if (bytes[30] != 0x00)
    {
        printf("Expected byte[30] to be 0x00 but was 0x%X\n", bytes[30]);
        failedAssertions++;
    }

    // Payload - Password - Length LSB
    if (bytes[31] != 0x05)
    {
        printf("Expected byte[31] to be 0x05 but was 0x%X\n", bytes[31]);
        failedAssertions++;
    }

    // Payload - Password - P
    if (bytes[32] != 0x50)
    {
        printf("Expected byte[32] to be 0x50 but was 0x%X\n", bytes[32]);
        failedAssertions++;
    }

    // Payload - Password - A
    if (bytes[33] != 0x41)
    {
        printf("Expected byte[33] to be 0x41 but was 0x%X\n", bytes[33]);
        failedAssertions++;
    }

    // Payload - Password - S
    if (bytes[34] != 0x53)
    {
        printf("Expected byte[34] to be 0x53 but was 0x%X\n", bytes[34]);
        failedAssertions++;
    }

    // Payload - Password - S
    if (bytes[35] != 0x53)
    {
        printf("Expected byte[35] to be 0x53 but was 0x%X\n", bytes[35]);
        failedAssertions++;
    }

    // Payload - Password - W
    if (bytes[36] != 0x57)
    {
        printf("Expected byte[36] to be 0x57 but was 0x%X\n", bytes[36]);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldEncodeConnectWithClientIdentifierSizeOf0()
{
    printf("It should encode connect with a client identifier size of 0\n");
    uint16_t failedAssertions = 0;

    struct MqttConnectParameter parameter = {
        .clientIdentifier = 0,
        .clientIdentifierSize = 0,
        .cleanSession = 0,
        .willTopic = 0,
        .willTopicSize = 0,
        .willMessage = 0,
        .willMessageSize = 0,
        .willQos = 0,
        .willRetain = 0,
        .userName = 0,
        .userNameSize = 0,
        .password = 0,
        .passwordSize = 0,
        .keepAlive = 0xAABB
    };

    uint8_t bytes[14];
    uint32_t size = encodeMqttConnect(&parameter, bytes);

    if (size != 14)
    {
        printf("Expected size to be 14 but was %i\n", size);
        failedAssertions++;
    }

    // Fixed header - Packet type
    if (bytes[0] != 0x10)
    {
        printf("Expected byte[0] to be 0x10 but was 0x%X\n", bytes[0]);
        failedAssertions++;
    }

    // Fixed header - Remaining length
    if (bytes[1] != 12)
    {
        printf("Expected byte[1] to be 12 but was %i\n", bytes[1]);
        failedAssertions++;
    }

    // Variable header - Protocol name - Length MSB
    if (bytes[2] != 0x00)
    {
        printf("Expected byte[2] to be 0x00 but was 0x%X\n", bytes[2]);
        failedAssertions++;
    }

    // Variable header - Protocol name - Length LSB
    if (bytes[3] != 0x04)
    {
        printf("Expected byte[3] to be 0x04 but was 0x%X\n", bytes[3]);
        failedAssertions++;
    }

    // Variable header - Protocol name - M
    if (bytes[4] != 0x4D)
    {
        printf("Expected byte[4] to be 0x4D but was 0x%X\n", bytes[4]);
        failedAssertions++;
    }

    // Variable header - Protocol name - Q
    if (bytes[5] != 0x51)
    {
        printf("Expected byte[5] to be 0x51 but was 0x%X\n", bytes[5]);
        failedAssertions++;
    }

    // Variable header - Protocol name - Q
    if (bytes[6] != 0x54)
    {
        printf("Expected byte[6] to be 0x54 but was 0x%X\n", bytes[6]);
        failedAssertions++;
    }

    // Variable header - Protocol name - Q
    if (bytes[7] != 0x54)
    {
        printf("Expected byte[7] to be 0x54 but was 0x%X\n", bytes[7]);
        failedAssertions++;
    }

    // Variable header - Protocol version
    if (bytes[8] != 0x04)
    {
        printf("Expected byte[8] to be 0x04 but was 0x%X\n", bytes[8]);
        failedAssertions++;
    }

    // Variable header - Connect flags
    if (bytes[9] != 0x02)
    {
        printf("Expected byte[9] to be 0x02 but was 0x%X\n", bytes[9]);
        failedAssertions++;
    }

    // Variable header - Keep alive MSB
    if (bytes[10] != 0xAA)
    {
        printf("Expected byte[10] to be 0xAA but was 0x%X\n", bytes[10]);
        failedAssertions++;
    }

    // Variable header - Keep alive LSB
    if (bytes[11] != 0xBB)
    {
        printf("Expected byte[11] to be 0xBB but was 0x%X\n", bytes[11]);
        failedAssertions++;
    }

    // Payload - Client identifier - Length MSB
    if (bytes[12] != 0x00)
    {
        printf("Expected byte[12] to be 0x00 but was 0x%X\n", bytes[12]);
        failedAssertions++;
    }

    // Payload - Client identifier - Length LSB
    if (bytes[13] != 0x00)
    {
        printf("Expected byte[13] to be 0x00 but was 0x%X\n", bytes[13]);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}
