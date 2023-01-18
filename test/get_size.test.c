#include <stdio.h>
#include <stdint.h>
#include <mqtt_packer.h>

static uint16_t it_should_get_the_correct_remaining_length_size_for_1_byte_length()
{
    printf("It should get the correct remaining length size for a 1 byte length\n");
    uint16_t failed_assertions = 0;

    uint32_t size = get_mqtt_remaining_length_size(0);

    if (size != 1)
    {
        printf("Expected size to be 1 but was %i\n", size);
        failed_assertions++;
    }

    size = get_mqtt_remaining_length_size(127);

    if (size != 1)
    {
        printf("Expected size to be 1 but was %i\n", size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_get_the_correct_remaining_length_size_for_2_byte_length()
{
    printf("It should get the correct remaining length size for a 2 byte length\n");
    uint16_t failed_assertions = 0;

    uint32_t size = get_mqtt_remaining_length_size(128);

    if (size != 2)
    {
        printf("Remaining length 128: Expected size to be 2 but was %i\n", size);
        failed_assertions++;
    }

    size = get_mqtt_remaining_length_size(16383);

    if (size != 2)
    {
        printf("Remaining length 16383: Expected size to be 2 but was %i\n", size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_get_the_correct_remaining_length_size_for_3_byte_length()
{
    printf("It should get the correct remaining length size for a 3 byte length\n");
    uint16_t failed_assertions = 0;

    uint32_t size = get_mqtt_remaining_length_size(16384);

    if (size != 3)
    {
        printf("Remaining length 16384: Expected size to be 3 but was %i\n", size);
        failed_assertions++;
    }

    size = get_mqtt_remaining_length_size(2097151);

    if (size != 3)
    {
        printf("Remaining length 2097151: Expected size to be 3 but was %i\n", size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_get_the_correct_remaining_length_size_for_4_byte_length()
{
    printf("It should get the correct remaining length size for a 4 byte length\n");
    uint16_t failed_assertions = 0;

    uint32_t size = get_mqtt_remaining_length_size(2097152);

    if (size != 4)
    {
        printf("Expected size to be 4 but was %i\n", size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_get_the_correct_connect_size()
{
    printf("It should get the correct CONNECT size\n");
    uint16_t failed_assertions = 0;

    uint8_t client_identifier[] = "C";
    uint8_t will_topic[] = "TO";
    uint8_t will_message[] = "MES";
    uint8_t username[] = "USER";
    uint8_t password[] = "PASSW";

    mqtt_connect_packet_t parameter = {
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

    uint32_t size = get_mqtt_connect_size(&parameter);

    if (size != 37)
    {
        printf("Expected size to be 37 but was %i\n", size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_get_the_correct_connect_size_with_client_identifier_size_of_0()
{
    printf("It should encode CONNECT with a client identifier size of 0\n");
    uint16_t failed_assertions = 0;

    mqtt_connect_packet_t parameter = {
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

    uint32_t size = get_mqtt_connect_size(&parameter);

    if (size != 14)
    {
        printf("Expected size to be 14 but was %i\n", size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_get_the_correct_publish_size()
{
    printf("It should get the correct PUBLISH size\n");
    uint16_t failed_assertions = 0;

    uint8_t topic_name[] = "TOPIC";
    uint8_t payload[] = { 0x00, 0x01, 0x02 };

    mqtt_publish_packet_t parameter = {
        .packet_identifier = 0xAABB,
        .dup = 1,
        .qos = MQTT_QOS_EXACTLY_ONCE,
        .retain = 1,
        .topic_name = topic_name,
        .topic_name_size = 5,
        .payload = payload,
        .payload_size = 3,
    };

    uint32_t size = get_mqtt_publish_size(&parameter);

    if (size != 14)
    {
        printf("Expected size to be 14 but was %i\n", size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_get_the_correct_puback_size()
{
    printf("It should get the correct PUBACK size\n");
    uint16_t failed_assertions = 0;

    uint32_t size = get_mqtt_puback_size(0xAABB);

    if (size != 4)
    {
        printf("Expected size to be 4 but was %i\n", size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_get_the_correct_pubrec_size()
{
    printf("It should get the correct PUBREC size\n");
    uint16_t failed_assertions = 0;

    uint32_t size = get_mqtt_pubrec_size(0xAABB);

    if (size != 4)
    {
        printf("Expected size to be 4 but was %i\n", size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_get_the_correct_pubrel_size()
{
    printf("It should get the correct PUBREL size\n");
    uint16_t failed_assertions = 0;

    uint32_t size = get_mqtt_pubrel_size(0xAABB);

    if (size != 4)
    {
        printf("Expected size to be 4 but was %i\n", size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_get_the_correct_pubcomp_size()
{
    printf("It should get the correct PUBCOMP size\n");
    uint16_t failed_assertions = 0;

    uint32_t size = get_mqtt_pubcomp_size(0xAABB);

    if (size != 4)
    {
        printf("Expected size to be 4 but was %i\n", size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_get_the_correct_subscribe_size()
{
    printf("It should get the correct SUBSCRIBE size\n");
    uint16_t failed_assertions = 0;

    uint8_t topic_filter[] = "TOPIC";

    mqtt_un_subscribe_packet_t parameter = {
        .packet_identifier = 0xAABB,
        .topic_filter = topic_filter,
        .topic_filter_size = 5,
        .qos = MQTT_QOS_EXACTLY_ONCE
    };

    uint32_t size = get_mqtt_subscribe_size(&parameter);

    if (size != 12)
    {
        printf("Expected size to be 12 but was %i\n", size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_get_the_correct_unsubscribe_size()
{
    printf("It should get the correct UBSUBSCRIBE size\n");
    uint16_t failed_assertions = 0;

    uint8_t topic_filter[] = "TOPIC";

    mqtt_un_subscribe_packet_t parameter = {
        .packet_identifier = 0xAABB,
        .topic_filter = topic_filter,
        .topic_filter_size = 5,
        .qos = MQTT_QOS_EXACTLY_ONCE
    };

    uint32_t size = get_mqtt_unsubscribe_size(&parameter);

    if (size != 11)
    {
        printf("Expected size to be 11 but was %i\n", size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_get_the_correct_pingreq_size()
{
    printf("It should get the correct PINGREQ size\n");
    uint16_t failed_assertions = 0;

    uint32_t size = get_mqtt_pingreq_size();

    if (size != 2)
    {
        printf("Expected size to be 2 but was %i\n", size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_get_the_correct_disconnect_size()
{
    printf("It should get the correct DISCONNECT size\n");
    uint16_t failed_assertions = 0;

    uint8_t bytes[2];
    uint32_t size = get_mqtt_disconnect_size();

    if (size != 2)
    {
        printf("Expected size to be 2 but was %i\n", size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}
