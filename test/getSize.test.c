#include <stdio.h>
#include <stdint.h>
#include "../src/mqttCodec.h"

static uint16_t itShouldGetTheCorrectConnectSize()
{
    printf("It should get the correct CONNECT size\n");
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

    uint32_t size = getMqttConnectSize(&parameter);

    if (size != 37)
    {
        printf("Expected size to be 37 but was %i\n", size);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldGetTheCorrectConnectSizeWithClientIdentifierSizeOf0()
{
    printf("It should encode CONNECT with a client identifier size of 0\n");
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

    uint32_t size = getMqttConnectSize(&parameter);

    if (size != 14)
    {
        printf("Expected size to be 14 but was %i\n", size);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldGetTheCorrectPublishSize()
{
    printf("It should get the correct PUBLISH size\n");
    uint16_t failedAssertions = 0;

    uint8_t topicName[] = "TOPIC";
    uint8_t payload[] = { 0x00, 0x01, 0x02 };

    struct MqttPublishParameter parameter = {
        .packetIdentifier = 0xAABB,
        .dup = 1,
        .qos = MQTT_PUBLISH_FIXED_HEADER_FLAG_QOS_EXACTLY_ONCE,
        .retain = 1,
        .topicName = topicName,
        .topicNameSize = 5,
        .payload = payload,
        .payloadSize = 3,
    };

    uint32_t size = getMqttPublishSize(&parameter);

    if (size != 14)
    {
        printf("Expected size to be 14 but was %i\n", size);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldGetTheCorrectPubAckSize()
{
    printf("It should get the correct PUBACK size\n");
    uint16_t failedAssertions = 0;

    uint32_t size = getMqttPubAckSize(0xAABB);

    if (size != 4)
    {
        printf("Expected size to be 4 but was %i\n", size);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldGetTheCorrectPubRecSize()
{
    printf("It should get the correct PUBREC size\n");
    uint16_t failedAssertions = 0;

    uint32_t size = getMqttPubRecSize(0xAABB);

    if (size != 4)
    {
        printf("Expected size to be 4 but was %i\n", size);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldGetTheCorrectPubRelSize()
{
    printf("It should get the correct PUBREL size\n");
    uint16_t failedAssertions = 0;

    uint32_t size = getMqttPubRelSize(0xAABB);

    if (size != 4)
    {
        printf("Expected size to be 4 but was %i\n", size);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldGetTheCorrectPubCompSize()
{
    printf("It should get the correct PUBCOMP size\n");
    uint16_t failedAssertions = 0;

    uint32_t size = getMqttPubCompSize(0xAABB);

    if (size != 4)
    {
        printf("Expected size to be 4 but was %i\n", size);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldGetTheCorrectSubscribeSize()
{
    printf("It should get the correct SUBSCRIBE size\n");
    uint16_t failedAssertions = 0;

    uint8_t topicFilter[] = "TOPIC";

    struct MqttUnSubscribeParameter parameter = {
        .packetIdentifier = 0xAABB,
        .topicFilter = topicFilter,
        .topicFilterSize = 5,
        .qos = MQTT_SUBSCRIBE_PAYLOAD_QOS_EXACTLY_ONCE
    };

    uint32_t size = getMqttSubscribeSize(&parameter);

    if (size != 12)
    {
        printf("Expected size to be 12 but was %i\n", size);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldGetTheCorrectUnsubscribeSize()
{
    printf("It should get the correct UBSUBSCRIBE size\n");
    uint16_t failedAssertions = 0;

    uint8_t topicFilter[] = "TOPIC";

    struct MqttUnSubscribeParameter parameter = {
        .packetIdentifier = 0xAABB,
        .topicFilter = topicFilter,
        .topicFilterSize = 5,
        .qos = MQTT_SUBSCRIBE_PAYLOAD_QOS_EXACTLY_ONCE
    };

    uint32_t size = getMqttUnsubscribeSize(&parameter);

    if (size != 11)
    {
        printf("Expected size to be 11 but was %i\n", size);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldGetTheCorrectPingReqSize()
{
    printf("It should get the correct PINGREQ size\n");
    uint16_t failedAssertions = 0;

    uint32_t size = getMqttPingReqSize();

    if (size != 2)
    {
        printf("Expected size to be 2 but was %i\n", size);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}

static uint16_t itShouldGetTheCorrectDisconnectSize()
{
    printf("It should get the correct DISCONNECT size\n");
    uint16_t failedAssertions = 0;

    uint8_t bytes[2];
    uint32_t size = getMqttDisconnectSize();

    if (size != 2)
    {
        printf("Expected size to be 2 but was %i\n", size);
        failedAssertions++;
    }

    printf("Failed assertions: %i\n\n", failedAssertions);
    return failedAssertions;
}
