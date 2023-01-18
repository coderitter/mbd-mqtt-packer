#include <stdio.h>
#include <stdint.h>
#include <mqtt_packer.h>

static uint16_t it_should_pack_the_remaining_length_with_1_byte()
{
    printf("It should pack the remaining length with 1 byte\n");

    uint8_t bytes[2];
    uint32_t size = pack_mqtt_remaining_length(bytes, 127);
    uint16_t failed_assertions = 0;

    if (size != 1)
    {
        printf("Expected size to be 1 but was %i\n", size);
        failed_assertions++;
    }

    if (bytes[0] != 0x7F)
    {
        printf("Expected byte[0] to be 0x7F but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_pack_the_remaining_length_with_2_bytes()
{
    printf("It should pack the remaining length with 2 bytes\n");
    uint16_t failed_assertions = 0;

    uint8_t bytes[3];
    uint32_t size = pack_mqtt_remaining_length(bytes, 128);

    if (size != 2)
    {
        printf("Remaining length 128: Expected size to be 2 but was %i\n", size);
        failed_assertions++;
    }

    if (bytes[0] != 0x80)
    {
        printf("Remaining length 128: Expected byte[0] to be 0x80 but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    if (bytes[1] != 0x01)
    {
        printf("Remaining length 128: Expected byte[1] to be 0x01 but was 0x%X\n", bytes[1]);
        failed_assertions++;
    }

    size = pack_mqtt_remaining_length(bytes, 16383);

    if (size != 2)
    {
        printf("Remaining length 16383: Expected size to be 2 but was %i\n", size);
        failed_assertions++;
    }

    if (bytes[0] != 0xFF)
    {
        printf("Remaining length 16383: Expected byte[0] to be 0xFF but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    if (bytes[1] != 0x7F)
    {
        printf("Remaining length 16383: Expected byte[1] to be 0x7F but was 0x%X\n", bytes[1]);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_pack_the_remaining_length_with_3_bytes()
{
    printf("It should pack the remaining length with 3 bytes\n");
    uint16_t failed_assertions = 0;

    uint8_t bytes[4];
    uint32_t size = pack_mqtt_remaining_length(bytes, 16384);

    if (size != 3)
    {
        printf("Remaining length 16384: Expected size to be 3 but was %i\n", size);
        failed_assertions++;
    }

    if (bytes[0] != 0x80)
    {
        printf("Remaining length 16384: Expected byte[0] to be 0x80 but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    if (bytes[1] != 0x80)
    {
        printf("Remaining length 16384: Expected byte[1] to be 0x80 but was 0x%X\n", bytes[1]);
        failed_assertions++;
    }

    if (bytes[2] != 0x01)
    {
        printf("Remaining length 16384: Expected byte[2] to be 0x01 but was 0x%X\n", bytes[2]);
        failed_assertions++;
    }

    size = pack_mqtt_remaining_length(bytes, 2097151);

    if (size != 3)
    {
        printf("Remaining length 2097151: Expected size to be 3 but was %i\n", size);
        failed_assertions++;
    }

    if (bytes[0] != 0xFF)
    {
        printf("Remaining length 2097151: Expected byte[0] to be 0x80 but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    if (bytes[1] != 0xFF)
    {
        printf("Remaining length 2097151: Expected byte[1] to be 0xFF but was 0x%X\n", bytes[1]);
        failed_assertions++;
    }

    if (bytes[2] != 0x7F)
    {
        printf("Remaining length 2097151: Expected byte[2] to be 0x7F but was 0x%X\n", bytes[2]);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_pack_the_remaining_length_with_4_bytes()
{
    printf("It should pack the remaining length with 4 bytes\n");
    uint16_t failed_assertions = 0;

    uint8_t bytes[5];
    uint32_t size = pack_mqtt_remaining_length(bytes, 2097152);

    if (size != 4)
    {
        printf("Expected size to be 4 but was %i\n", size);
        failed_assertions++;
    }

    if (bytes[0] != 0x80)
    {
        printf("Expected byte[0] to be 0x80 but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    if (bytes[1] != 0x80)
    {
        printf("Expected byte[1] to be 0x80 but was 0x%X\n", bytes[1]);
        failed_assertions++;
    }

    if (bytes[2] != 0x80)
    {
        printf("Expected byte[2] to be 0x80 but was 0x%X\n", bytes[2]);
        failed_assertions++;
    }

    if (bytes[3] != 0x01)
    {
        printf("Expected byte[3] to be 0x01 but was 0x%X\n", bytes[3]);
        failed_assertions++;
    }

    size = pack_mqtt_remaining_length(bytes, 268435455);

    if (size != 4)
    {
        printf("Expected size to be 4 but was %i\n", size);
        failed_assertions++;
    }

    if (bytes[0] != 0xFF)
    {
        printf("Expected byte[0] to be 0x80 but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    if (bytes[1] != 0xFF)
    {
        printf("Expected byte[1] to be 0xFF but was 0x%X\n", bytes[1]);
        failed_assertions++;
    }

    if (bytes[2] != 0xFF)
    {
        printf("Expected byte[2] to be 0xFF but was 0x%X\n", bytes[2]);
        failed_assertions++;
    }

    if (bytes[3] != 0x7F)
    {
        printf("Expected byte[3] to be 0x7F but was 0x%X\n", bytes[3]);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_pack_connect()
{
    printf("It should pack CONNECT\n");
    uint16_t failed_assertions = 0;

    uint8_t client_identifier[] = "C";
    uint8_t will_topic[] = "TO";
    uint8_t will_message[] = "MES";
    uint8_t username[] = "USER";
    uint8_t password[] = "PASSW";

    mqtt_connect_packet_t packet = {
        .client_identifier = client_identifier,
        .client_identifier_size = 1,
        .clean_session = 1,
        .will_topic = will_topic,
        .will_topic_size = 2,
        .will_message = will_message,
        .will_message_size = 3,
        .will_qos = MQTT_QOS_EXACTLY_ONCE,
        .will_retain = 1,
        .username = username,
        .username_size = 4,
        .password = password,
        .password_size = 5,
        .keep_alive = 0xAABB
    };

    uint8_t bytes[37];
    uint32_t size = pack_mqtt_connect(bytes, &packet);

    if (size != 37)
    {
        printf("Expected size to be 37 but was %i\n", size);
        failed_assertions++;
    }

    // Fixed header - Packet type
    if (bytes[0] != 0x10)
    {
        printf("Expected byte[0] to be 0x10 but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    // Fixed header - Remaining length
    if (bytes[1] != 35)
    {
        printf("Expected byte[1] to be 35 but was %i\n", bytes[1]);
        failed_assertions++;
    }

    // Variable header - Protocol name - Length MSB
    if (bytes[2] != 0x00)
    {
        printf("Expected byte[2] to be 0x00 but was 0x%X\n", bytes[2]);
        failed_assertions++;
    }

    // Variable header - Protocol name - Length LSB
    if (bytes[3] != 0x04)
    {
        printf("Expected byte[3] to be 0x04 but was 0x%X\n", bytes[3]);
        failed_assertions++;
    }

    // Variable header - Protocol name - M
    if (bytes[4] != 0x4D)
    {
        printf("Expected byte[4] to be 0x4D but was 0x%X\n", bytes[4]);
        failed_assertions++;
    }

    // Variable header - Protocol name - Q
    if (bytes[5] != 0x51)
    {
        printf("Expected byte[5] to be 0x51 but was 0x%X\n", bytes[5]);
        failed_assertions++;
    }

    // Variable header - Protocol name - Q
    if (bytes[6] != 0x54)
    {
        printf("Expected byte[6] to be 0x54 but was 0x%X\n", bytes[6]);
        failed_assertions++;
    }

    // Variable header - Protocol name - Q
    if (bytes[7] != 0x54)
    {
        printf("Expected byte[7] to be 0x54 but was 0x%X\n", bytes[7]);
        failed_assertions++;
    }

    // Variable header - Protocol version
    if (bytes[8] != 0x04)
    {
        printf("Expected byte[8] to be 0x04 but was 0x%X\n", bytes[8]);
        failed_assertions++;
    }

    // Variable header - Connect flags
    if (bytes[9] != 0xF6)
    {
        printf("Expected byte[9] to be 0xF6 but was 0x%X\n", bytes[9]);
        failed_assertions++;
    }

    // Variable header - Keep alive MSB
    if (bytes[10] != 0xAA)
    {
        printf("Expected byte[10] to be 0xAA but was 0x%X\n", bytes[10]);
        failed_assertions++;
    }

    // Variable header - Keep alive LSB
    if (bytes[11] != 0xBB)
    {
        printf("Expected byte[11] to be 0xBB but was 0x%X\n", bytes[11]);
        failed_assertions++;
    }

    // Payload - Client identifier - Length MSB
    if (bytes[12] != 0x00)
    {
        printf("Expected byte[12] to be 0x00 but was 0x%X\n", bytes[12]);
        failed_assertions++;
    }

    // Payload - Client identifier - Length LSB
    if (bytes[13] != 0x01)
    {
        printf("Expected byte[13] to be 0x01 but was 0x%X\n", bytes[13]);
        failed_assertions++;
    }

    // Payload - Client identifier - C
    if (bytes[14] != 0x43)
    {
        printf("Expected byte[14] to be 0x43 but was 0x%X\n", bytes[14]);
        failed_assertions++;
    }

    // Payload - Will topic - Length MSB
    if (bytes[15] != 0x00)
    {
        printf("Expected byte[15] to be 0x00 but was 0x%X\n", bytes[15]);
        failed_assertions++;
    }

    // Payload - Will topic - Length LSB
    if (bytes[16] != 0x02)
    {
        printf("Expected byte[16] to be 0x02 but was 0x%X\n", bytes[16]);
        failed_assertions++;
    }

    // Payload - Will topic - T
    if (bytes[17] != 0x54)
    {
        printf("Expected byte[17] to be 0x54 but was 0x%X\n", bytes[17]);
        failed_assertions++;
    }

    // Payload - Will topic - O
    if (bytes[18] != 0x4F)
    {
        printf("Expected byte[18] to be 0x4F but was 0x%X\n", bytes[18]);
        failed_assertions++;
    }

    // Payload - Will message - Length MSB
    if (bytes[19] != 0x00)
    {
        printf("Expected byte[19] to be 0x00 but was 0x%X\n", bytes[19]);
        failed_assertions++;
    }

    // Payload - Will message - Length LSB
    if (bytes[20] != 0x03)
    {
        printf("Expected byte[20] to be 0x03 but was 0x%X\n", bytes[20]);
        failed_assertions++;
    }

    // Payload - Will message - M
    if (bytes[21] != 0x4D)
    {
        printf("Expected byte[21] to be 0x4D but was 0x%X\n", bytes[21]);
        failed_assertions++;
    }

    // Payload - Will message - E
    if (bytes[22] != 0x45)
    {
        printf("Expected byte[22] to be 0x45 but was 0x%X\n", bytes[22]);
        failed_assertions++;
    }

    // Payload - Will message - S
    if (bytes[23] != 0x53)
    {
        printf("Expected byte[23] to be 0x53 but was 0x%X\n", bytes[23]);
        failed_assertions++;
    }

    // Payload - User name - Length MSB
    if (bytes[24] != 0x00)
    {
        printf("Expected byte[24] to be 0x00 but was 0x%X\n", bytes[24]);
        failed_assertions++;
    }

    // Payload - User name - Length LSB
    if (bytes[25] != 0x04)
    {
        printf("Expected byte[25] to be 0x04 but was 0x%X\n", bytes[25]);
        failed_assertions++;
    }

    // Payload - User name - U
    if (bytes[26] != 0x55)
    {
        printf("Expected byte[26] to be 0x55 but was 0x%X\n", bytes[26]);
        failed_assertions++;
    }

    // Payload - User name - S
    if (bytes[27] != 0x53)
    {
        printf("Expected byte[27] to be 0x53 but was 0x%X\n", bytes[27]);
        failed_assertions++;
    }

    // Payload - User name - E
    if (bytes[28] != 0x45)
    {
        printf("Expected byte[28] to be 0x45 but was 0x%X\n", bytes[28]);
        failed_assertions++;
    }

    // Payload - User name - R
    if (bytes[29] != 0x52)
    {
        printf("Expected byte[29] to be 0x52 but was 0x%X\n", bytes[29]);
        failed_assertions++;
    }

    // Payload - Password - Length MSB
    if (bytes[30] != 0x00)
    {
        printf("Expected byte[30] to be 0x00 but was 0x%X\n", bytes[30]);
        failed_assertions++;
    }

    // Payload - Password - Length LSB
    if (bytes[31] != 0x05)
    {
        printf("Expected byte[31] to be 0x05 but was 0x%X\n", bytes[31]);
        failed_assertions++;
    }

    // Payload - Password - P
    if (bytes[32] != 0x50)
    {
        printf("Expected byte[32] to be 0x50 but was 0x%X\n", bytes[32]);
        failed_assertions++;
    }

    // Payload - Password - A
    if (bytes[33] != 0x41)
    {
        printf("Expected byte[33] to be 0x41 but was 0x%X\n", bytes[33]);
        failed_assertions++;
    }

    // Payload - Password - S
    if (bytes[34] != 0x53)
    {
        printf("Expected byte[34] to be 0x53 but was 0x%X\n", bytes[34]);
        failed_assertions++;
    }

    // Payload - Password - S
    if (bytes[35] != 0x53)
    {
        printf("Expected byte[35] to be 0x53 but was 0x%X\n", bytes[35]);
        failed_assertions++;
    }

    // Payload - Password - W
    if (bytes[36] != 0x57)
    {
        printf("Expected byte[36] to be 0x57 but was 0x%X\n", bytes[36]);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_pack_connect_with_client_identifier_size_of_0()
{
    printf("It should pack CONNECT with a client identifier size of 0\n");
    uint16_t failed_assertions = 0;

    mqtt_connect_packet_t packet = {
        .client_identifier = 0,
        .client_identifier_size = 0,
        .clean_session = 0,
        .will_topic = 0,
        .will_topic_size = 0,
        .will_message = 0,
        .will_message_size = 0,
        .will_qos = 0,
        .will_retain = 0,
        .username = 0,
        .username_size = 0,
        .password = 0,
        .password_size = 0,
        .keep_alive = 0xAABB
    };

    uint8_t bytes[14];
    uint32_t size = pack_mqtt_connect(bytes, &packet);

    if (size != 14)
    {
        printf("Expected size to be 14 but was %i\n", size);
        failed_assertions++;
    }

    // Fixed header - Packet type
    if (bytes[0] != 0x10)
    {
        printf("Expected byte[0] to be 0x10 but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    // Fixed header - Remaining length
    if (bytes[1] != 12)
    {
        printf("Expected byte[1] to be 12 but was %i\n", bytes[1]);
        failed_assertions++;
    }

    // Variable header - Protocol name - Length MSB
    if (bytes[2] != 0x00)
    {
        printf("Expected byte[2] to be 0x00 but was 0x%X\n", bytes[2]);
        failed_assertions++;
    }

    // Variable header - Protocol name - Length LSB
    if (bytes[3] != 0x04)
    {
        printf("Expected byte[3] to be 0x04 but was 0x%X\n", bytes[3]);
        failed_assertions++;
    }

    // Variable header - Protocol name - M
    if (bytes[4] != 0x4D)
    {
        printf("Expected byte[4] to be 0x4D but was 0x%X\n", bytes[4]);
        failed_assertions++;
    }

    // Variable header - Protocol name - Q
    if (bytes[5] != 0x51)
    {
        printf("Expected byte[5] to be 0x51 but was 0x%X\n", bytes[5]);
        failed_assertions++;
    }

    // Variable header - Protocol name - Q
    if (bytes[6] != 0x54)
    {
        printf("Expected byte[6] to be 0x54 but was 0x%X\n", bytes[6]);
        failed_assertions++;
    }

    // Variable header - Protocol name - Q
    if (bytes[7] != 0x54)
    {
        printf("Expected byte[7] to be 0x54 but was 0x%X\n", bytes[7]);
        failed_assertions++;
    }

    // Variable header - Protocol version
    if (bytes[8] != 0x04)
    {
        printf("Expected byte[8] to be 0x04 but was 0x%X\n", bytes[8]);
        failed_assertions++;
    }

    // Variable header - Connect flags
    if (bytes[9] != 0x02)
    {
        printf("Expected byte[9] to be 0x02 but was 0x%X\n", bytes[9]);
        failed_assertions++;
    }

    // Variable header - Keep alive MSB
    if (bytes[10] != 0xAA)
    {
        printf("Expected byte[10] to be 0xAA but was 0x%X\n", bytes[10]);
        failed_assertions++;
    }

    // Variable header - Keep alive LSB
    if (bytes[11] != 0xBB)
    {
        printf("Expected byte[11] to be 0xBB but was 0x%X\n", bytes[11]);
        failed_assertions++;
    }

    // Payload - Client identifier - Length MSB
    if (bytes[12] != 0x00)
    {
        printf("Expected byte[12] to be 0x00 but was 0x%X\n", bytes[12]);
        failed_assertions++;
    }

    // Payload - Client identifier - Length LSB
    if (bytes[13] != 0x00)
    {
        printf("Expected byte[13] to be 0x00 but was 0x%X\n", bytes[13]);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_pack_publish()
{
    printf("It should pack PUBLISH\n");
    uint16_t failed_assertions = 0;

    uint8_t topic_name[] = "TOPIC";
    uint8_t payload[] = { 0x00, 0x01, 0x02 };

    mqtt_publish_packet_t packet = {
        .packet_identifier = 0xAABB,
        .dup = 1,
        .qos = MQTT_QOS_EXACTLY_ONCE,
        .retain = 1,
        .topic_name = topic_name,
        .topic_name_size = 5,
        .payload = payload,
        .payload_size = 3,
    };

    uint8_t bytes[14];
    uint32_t size = pack_mqtt_publish(bytes, &packet);

    if (size != 14)
    {
        printf("Expected size to be 14 but was %i\n", size);
        failed_assertions++;
    }

    // Fixed header - Packet type
    if (bytes[0] != 0x3D)
    {
        printf("Expected byte[0] to be 0x3D but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    // Fixed header - Remaining length
    if (bytes[1] != 12)
    {
        printf("Expected byte[1] to be 12 but was %i\n", bytes[1]);
        failed_assertions++;
    }

    // Variable header - Topic name - Length MSB
    if (bytes[2] != 0x00)
    {
        printf("Expected byte[2] to be 0x00 but was 0x%X\n", bytes[2]);
        failed_assertions++;
    }

    // Variable header - Topic name - Length LSB
    if (bytes[3] != 0x05)
    {
        printf("Expected byte[3] to be 0x05 but was 0x%X\n", bytes[3]);
        failed_assertions++;
    }

    // Variable header - Topic name - T
    if (bytes[4] != 0x54)
    {
        printf("Expected byte[4] to be 0x54 but was 0x%X\n", bytes[4]);
        failed_assertions++;
    }

    // Variable header - Topic name - O
    if (bytes[5] != 0x4F)
    {
        printf("Expected byte[5] to be 0x4F but was 0x%X\n", bytes[5]);
        failed_assertions++;
    }

    // Variable header - Topic name - P
    if (bytes[6] != 0x50)
    {
        printf("Expected byte[6] to be 0x50 but was 0x%X\n", bytes[6]);
        failed_assertions++;
    }

    // Variable header - Topic name - I
    if (bytes[7] != 0x49)
    {
        printf("Expected byte[7] to be 0x49 but was 0x%X\n", bytes[7]);
        failed_assertions++;
    }

    // Variable header - Topic name - C
    if (bytes[8] != 0x43)
    {
        printf("Expected byte[8] to be 0x43 but was 0x%X\n", bytes[8]);
        failed_assertions++;
    }

    // Variable header - Packet identifier MSB
    if (bytes[9] != 0xAA)
    {
        printf("Expected byte[9] to be 0xAA but was 0x%X\n", bytes[9]);
        failed_assertions++;
    }

    // Variable header - Packet identifier LSB
    if (bytes[10] != 0xBB)
    {
        printf("Expected byte[10] to be 0xBB but was 0x%X\n", bytes[10]);
        failed_assertions++;
    }

    // Payload - Byte 1
    if (bytes[11] != 0x00)
    {
        printf("Expected byte[11] to be 0x00 but was 0x%X\n", bytes[11]);
        failed_assertions++;
    }

    // Payload - Byte 2
    if (bytes[12] != 0x01)
    {
        printf("Expected byte[12] to be 0x01 but was 0x%X\n", bytes[12]);
        failed_assertions++;
    }

    // Payload - Byte 3
    if (bytes[13] != 0x02)
    {
        printf("Expected byte[13] to be 0x02 but was 0x%X\n", bytes[13]);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_pack_puback()
{
    printf("It should pack PUBACK\n");
    uint16_t failed_assertions = 0;

    uint8_t bytes[4];
    uint32_t size = pack_mqtt_puback(bytes, 0xAABB);

    if (size != 4)
    {
        printf("Expected size to be 4 but was %i\n", size);
        failed_assertions++;
    }

    // Fixed header - Packet type
    if (bytes[0] != 0x40)
    {
        printf("Expected byte[0] to be 0x40 but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    // Fixed header - Remaining length
    if (bytes[1] != 2)
    {
        printf("Expected byte[1] to be 2 but was %i\n", bytes[1]);
        failed_assertions++;
    }

    // Variable header - Packet identifier - Length MSB
    if (bytes[2] != 0xAA)
    {
        printf("Expected byte[2] to be 0xAA but was 0x%X\n", bytes[2]);
        failed_assertions++;
    }

    // Variable header - Packet identifier - Length LSB
    if (bytes[3] != 0xBB)
    {
        printf("Expected byte[3] to be 0xBB but was 0x%X\n", bytes[3]);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_pack_pubrec()
{
    printf("It should pack PUBREC\n");
    uint16_t failed_assertions = 0;

    uint8_t bytes[4];
    uint32_t size = pack_mqtt_pubrec(bytes, 0xAABB);

    if (size != 4)
    {
        printf("Expected size to be 4 but was %i\n", size);
        failed_assertions++;
    }

    // Fixed header - Packet type
    if (bytes[0] != 0x50)
    {
        printf("Expected byte[0] to be 0x50 but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    // Fixed header - Remaining length
    if (bytes[1] != 2)
    {
        printf("Expected byte[1] to be 2 but was %i\n", bytes[1]);
        failed_assertions++;
    }

    // Variable header - Packet identifier - Length MSB
    if (bytes[2] != 0xAA)
    {
        printf("Expected byte[2] to be 0xAA but was 0x%X\n", bytes[2]);
        failed_assertions++;
    }

    // Variable header - Packet identifier - Length LSB
    if (bytes[3] != 0xBB)
    {
        printf("Expected byte[3] to be 0xBB but was 0x%X\n", bytes[3]);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_pack_pubrel()
{
    printf("It should pack PUBREL\n");
    uint16_t failed_assertions = 0;

    uint8_t bytes[4];
    uint32_t size = pack_mqtt_pubrel(bytes, 0xAABB);

    if (size != 4)
    {
        printf("Expected size to be 4 but was %i\n", size);
        failed_assertions++;
    }

    // Fixed header - Packet type
    if (bytes[0] != 0x62)
    {
        printf("Expected byte[0] to be 0x62 but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    // Fixed header - Remaining length
    if (bytes[1] != 2)
    {
        printf("Expected byte[1] to be 2 but was %i\n", bytes[1]);
        failed_assertions++;
    }

    // Variable header - Packet identifier - Length MSB
    if (bytes[2] != 0xAA)
    {
        printf("Expected byte[2] to be 0xAA but was 0x%X\n", bytes[2]);
        failed_assertions++;
    }

    // Variable header - Packet identifier - Length LSB
    if (bytes[3] != 0xBB)
    {
        printf("Expected byte[3] to be 0xBB but was 0x%X\n", bytes[3]);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_pack_pubcomp()
{
    printf("It should pack PUBCOMP\n");
    uint16_t failed_assertions = 0;

    uint8_t bytes[4];
    uint32_t size = pack_mqtt_pubcomp(bytes, 0xAABB);

    if (size != 4)
    {
        printf("Expected size to be 4 but was %i\n", size);
        failed_assertions++;
    }

    // Fixed header - Packet type
    if (bytes[0] != 0x70)
    {
        printf("Expected byte[0] to be 0x70 but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    // Fixed header - Remaining length
    if (bytes[1] != 2)
    {
        printf("Expected byte[1] to be 2 but was %i\n", bytes[1]);
        failed_assertions++;
    }

    // Variable header - Packet identifier - Length MSB
    if (bytes[2] != 0xAA)
    {
        printf("Expected byte[2] to be 0xAA but was 0x%X\n", bytes[2]);
        failed_assertions++;
    }

    // Variable header - Packet identifier - Length LSB
    if (bytes[3] != 0xBB)
    {
        printf("Expected byte[3] to be 0xBB but was 0x%X\n", bytes[3]);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_pack_subscribe()
{
    printf("It should pack SUBSCRIBE\n");
    uint16_t failed_assertions = 0;

    uint8_t topic_filter[] = "TOPIC";

    mqtt_un_subscribe_packet_t packet = {
        .packet_identifier = 0xAABB,
        .topic_filter = topic_filter,
        .topic_filter_size = 5,
        .qos = MQTT_QOS_EXACTLY_ONCE
    };

    uint8_t bytes[12];
    uint32_t size = pack_mqtt_subscribe(bytes, &packet);

    if (size != 12)
    {
        printf("Expected size to be 12 but was %i\n", size);
        failed_assertions++;
    }

    // Fixed header - Packet type
    if (bytes[0] != 0x82)
    {
        printf("Expected byte[0] to be 0x82 but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    // Fixed header - Remaining length
    if (bytes[1] != 10)
    {
        printf("Expected byte[1] to be 10 but was %i\n", bytes[1]);
        failed_assertions++;
    }

    // Variable header - Packet identifier - Length MSB
    if (bytes[2] != 0xAA)
    {
        printf("Expected byte[2] to be 0xAA but was 0x%X\n", bytes[2]);
        failed_assertions++;
    }

    // Variable header - Packet identifier - Length LSB
    if (bytes[3] != 0xBB)
    {
        printf("Expected byte[3] to be 0xBB but was 0x%X\n", bytes[3]);
        failed_assertions++;
    }

    // Payload - Topic filter - Length MSB
    if (bytes[4] != 0x00)
    {
        printf("Expected byte[4] to be 0x00 but was 0x%X\n", bytes[4]);
        failed_assertions++;
    }

    // Payload - Topic filter - Length LSB
    if (bytes[5] != 0x05)
    {
        printf("Expected byte[5] to be 0x05 but was 0x%X\n", bytes[5]);
        failed_assertions++;
    }

    // Payload - Topic filter - T
    if (bytes[6] != 0x54)
    {
        printf("Expected byte[6] to be 0x54 but was 0x%X\n", bytes[6]);
        failed_assertions++;
    }

    // Payload - Topic filter - O
    if (bytes[7] != 0x4F)
    {
        printf("Expected byte[7] to be 0x4F but was 0x%X\n", bytes[7]);
        failed_assertions++;
    }

    // Payload - Topic filter - P
    if (bytes[8] != 0x50)
    {
        printf("Expected byte[8] to be 0x50 but was 0x%X\n", bytes[8]);
        failed_assertions++;
    }

    // Payload - Topic filter - I
    if (bytes[9] != 0x49)
    {
        printf("Expected byte[9] to be 0x49 but was 0x%X\n", bytes[9]);
        failed_assertions++;
    }

    // Payload - Topic filter - C
    if (bytes[10] != 0x43)
    {
        printf("Expected byte[10] to be 0x43 but was 0x%X\n", bytes[10]);
        failed_assertions++;
    }

    // Payload - QoS
    if (bytes[11] != 0x02)
    {
        printf("Expected byte[11] to be 0x02 but was 0x%X\n", bytes[11]);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_pack_unsubscribe()
{
    printf("It should pack UBSUBSCRIBE\n");
    uint16_t failed_assertions = 0;

    uint8_t topic_filter[] = "TOPIC";

    mqtt_un_subscribe_packet_t packet = {
        .packet_identifier = 0xAABB,
        .topic_filter = topic_filter,
        .topic_filter_size = 5,
        .qos = MQTT_QOS_EXACTLY_ONCE
    };

    uint8_t bytes[11];
    uint32_t size = pack_mqtt_unsubscribe(bytes, &packet);

    if (size != 11)
    {
        printf("Expected size to be 11 but was %i\n", size);
        failed_assertions++;
    }

    // Fixed header - Packet type
    if (bytes[0] != 0xA2)
    {
        printf("Expected byte[0] to be 0xA2 but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    // Fixed header - Remaining length
    if (bytes[1] != 9)
    {
        printf("Expected byte[1] to be 9 but was %i\n", bytes[1]);
        failed_assertions++;
    }

    // Variable header - Packet identifier - Length MSB
    if (bytes[2] != 0xAA)
    {
        printf("Expected byte[2] to be 0xAA but was 0x%X\n", bytes[2]);
        failed_assertions++;
    }

    // Variable header - Packet identifier - Length LSB
    if (bytes[3] != 0xBB)
    {
        printf("Expected byte[3] to be 0xBB but was 0x%X\n", bytes[3]);
        failed_assertions++;
    }

    // Payload - Topic filter - Length MSB
    if (bytes[4] != 0x00)
    {
        printf("Expected byte[4] to be 0x00 but was 0x%X\n", bytes[4]);
        failed_assertions++;
    }

    // Payload - Topic filter - Length LSB
    if (bytes[5] != 0x05)
    {
        printf("Expected byte[5] to be 0x05 but was 0x%X\n", bytes[5]);
        failed_assertions++;
    }

    // Payload - Topic filter - T
    if (bytes[6] != 0x54)
    {
        printf("Expected byte[6] to be 0x54 but was 0x%X\n", bytes[6]);
        failed_assertions++;
    }

    // Payload - Topic filter - O
    if (bytes[7] != 0x4F)
    {
        printf("Expected byte[7] to be 0x4F but was 0x%X\n", bytes[7]);
        failed_assertions++;
    }

    // Payload - Topic filter - P
    if (bytes[8] != 0x50)
    {
        printf("Expected byte[8] to be 0x50 but was 0x%X\n", bytes[8]);
        failed_assertions++;
    }

    // Payload - Topic filter - I
    if (bytes[9] != 0x49)
    {
        printf("Expected byte[9] to be 0x49 but was 0x%X\n", bytes[9]);
        failed_assertions++;
    }

    // Payload - Topic filter - C
    if (bytes[10] != 0x43)
    {
        printf("Expected byte[10] to be 0x43 but was 0x%X\n", bytes[10]);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_pack_pingreq()
{
    printf("It should pack PINGREQ\n");
    uint16_t failed_assertions = 0;

    uint8_t bytes[2];
    uint32_t size = pack_mqtt_pingreq(bytes);

    if (size != 2)
    {
        printf("Expected size to be 2 but was %i\n", size);
        failed_assertions++;
    }

    // Fixed header - Packet type
    if (bytes[0] != 0xC0)
    {
        printf("Expected byte[0] to be 0xC0 but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    // Fixed header - Remaining length
    if (bytes[1] != 0)
    {
        printf("Expected byte[1] to be 0 but was %i\n", bytes[1]);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_pack_disconnect()
{
    printf("It should pack DISCONNECT\n");
    uint16_t failed_assertions = 0;

    uint8_t bytes[2];
    uint32_t size = pack_mqtt_disconnect(bytes);

    if (size != 2)
    {
        printf("Expected size to be 2 but was %i\n", size);
        failed_assertions++;
    }

    // Fixed header - Packet type
    if (bytes[0] != 0xE0)
    {
        printf("Expected byte[0] to be 0xE0 but was 0x%X\n", bytes[0]);
        failed_assertions++;
    }

    // Fixed header - Remaining length
    if (bytes[1] != 0)
    {
        printf("Expected byte[1] to be 0 but was %i\n", bytes[1]);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}
