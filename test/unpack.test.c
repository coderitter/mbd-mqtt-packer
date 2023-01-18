#include <stdio.h>
#include <stdint.h>
#include <mqtt_packer.h>    

uint16_t failed_assertions = 0;

static uint16_t it_should_unpack_with_a_2_byte_header_and_a_zero_length_remaining_size()
{
    printf("It should unpack with a 2 byte header and a zero length remaining size\n");
    failed_assertions = 0;

    uint8_t bytes[10] = { 0x30, 0x00 };
    mqtt_packet_t packet;

    unpack_mqtt_packet(bytes, 2, &packet);

    if (packet.size != 2)
    {
        printf("Callback: Expected size to be 2 but was %i\n", packet.size);
        failed_assertions++;
    }

    if (packet.fixed_header_size != 2)
    {
        printf("Callback: Expected fixed_header_size to be 2 but was %i\n", packet.fixed_header_size);
        failed_assertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Callback: Expected type to be 0x03 but was 0x%X\n", packet.type);
        failed_assertions++;
    }
        
    if (packet.remaining_size != 0)
    {
        printf("Callback: Expected remaining_size to be 0 but was %i\n", packet.remaining_size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_unpack_with_a_2_byte_header()
{
    printf("It should unpack with a 2 byte header\n");
    failed_assertions = 0;

    uint8_t bytes[10] = { 0x30, 0x01, 0xFF };
    mqtt_packet_t packet;

    unpack_mqtt_packet(bytes, 3, &packet);

    if (packet.size != 3)
    {
        printf("Callback: Expected size to be 3 but was %i\n", packet.size);
        failed_assertions++;
    }

    if (packet.fixed_header_size != 2)
    {
        printf("Callback: Expected fixed_header_size to be 2 but was %i\n", packet.fixed_header_size);
        failed_assertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Callback: Expected type to be 0x03 but was 0x%X\n", packet.type);
        failed_assertions++;
    }

    if (packet.remaining_size != 1)
    {
        printf("Callback: Expected remaining_size to be 1 but was %i\n", packet.remaining_size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_unpack_with_a_3_byte_header()
{
    printf("It should unpack with a 3 byte header\n");
    failed_assertions = 0;

    uint8_t bytes[10] = { 0x30, 0x81, 0x02, 0xFF };
    mqtt_packet_t packet;

    unpack_mqtt_packet(bytes, 4, &packet);

    if (packet.size != 260)
    {
        printf("Expected size to be 260 but was %i\n", packet.size);
        failed_assertions++;
    }

    if (packet.fixed_header_size != 3)
    {
        printf("Expected fixed_header_size to be 3 but was %i\n", packet.fixed_header_size);
        failed_assertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Expected type to be 0x03 but was 0x%X\n", packet.type);
        failed_assertions++;
    }

    if (packet.remaining_size != 257)
    {
        printf("Expected remaining_size to be 257 but was %i\n", packet.remaining_size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_unpack_with_a_4_byte_header()
{
    printf("It should unpack with a 4 byte header\n");
    failed_assertions = 0;

    uint8_t bytes[10] = { 0x30, 0x81, 0x82, 0x03, 0xFF };
    mqtt_packet_t packet;

    unpack_mqtt_packet(bytes, 5, &packet);

    if (packet.size != 49413)
    {
        printf("Expected size to be 49413 but was %i\n", packet.size);
        failed_assertions++;
    }

    if (packet.fixed_header_size != 4)
    {
        printf("Expected fixed_header_size to be 4 but was %i\n", packet.fixed_header_size);
        failed_assertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Expected type to be 0x03 but was 0x%X\n", packet.type);
        failed_assertions++;
    }

    if (packet.remaining_size != 49409)
    {
        printf("Expected remaining_size to be 49409 but was %i\n", packet.remaining_size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_unpack_with_a_5_byte_header()
{
    printf("It should unpack with a 5 byte header\n");
    failed_assertions = 0;

    uint8_t bytes[10] = { 0x30, 0x81, 0x82, 0x83, 0x04, 0xFF };
    mqtt_packet_t packet;

    unpack_mqtt_packet(bytes, 6, &packet);

    if (packet.size != 8438022)
    {
        printf("Expected size to be 8438022 but was %i\n", packet.size);
        failed_assertions++;
    }

    if (packet.fixed_header_size != 5)
    {
        printf("Expected fixed_header_size to be 5 but was %i\n", packet.fixed_header_size);
        failed_assertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Expected type to be 0x03 but was 0x%X\n", packet.type);
        failed_assertions++;
    }

    if (packet.remaining_size != 8438017)
    {
        printf("Expected remaining_size to be 8438017 but was %i\n", packet.remaining_size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_unpack_with_a_5_byte_header_given_through_1_byte_chunks()
{
    printf("It should unpack with a 5 byte header given through 1 byte chunks\n");
    failed_assertions = 0;

    uint8_t bytes[10] = { 0x30, 0x81, 0x82, 0x83, 0x04, 0xFF };
    mqtt_packet_t packet;

    unpack_mqtt_packet(bytes, 1, &packet);

    if (packet.size != 2)
    {
        printf("Byte 1: Expected size to be 2 but was %i\n", packet.size);
        failed_assertions++;
    }

    if (packet.fixed_header_size != 2)
    {
        printf("Byte 1: Expected fixed_header_size to be 2 but was %i\n", packet.fixed_header_size);
        failed_assertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Byte 1: Expected type to be 0x03 but was 0x%X\n", packet.type);
        failed_assertions++;
    }

    if (packet.remaining_size != 0)
    {
        printf("Byte 1: Expected remaining_size to be 0 but was %i\n", packet.remaining_size);
        failed_assertions++;
    }

    unpack_mqtt_packet(bytes, 2, &packet);

    if (packet.size != 4)
    {
        printf("Byte 2: Expected size to be 4 but was %i\n", packet.size);
        failed_assertions++;
    }

    if (packet.fixed_header_size != 3)
    {
        printf("Byte 2: Expected fixed_header_size to be 3 but was %i\n", packet.fixed_header_size);
        failed_assertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Byte 2: Expected type to be 0x03 but was 0x%X\n", packet.type);
        failed_assertions++;
    }

    if (packet.remaining_size != 1)
    {
        printf("Byte 2: Expected remaining_size to be 1 but was %i\n", packet.remaining_size);
        failed_assertions++;
    }

    unpack_mqtt_packet(bytes, 3, &packet);

    if (packet.size != 261)
    {
        printf("Byte 3: Expected size to be 261 but was %i\n", packet.size);
        failed_assertions++;
    }

    if (packet.fixed_header_size != 4)
    {
        printf("Byte 3: Expected fixed_header_size to be 4 but was %i\n", packet.fixed_header_size);
        failed_assertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Byte 3: Expected type to be 0x03 but was 0x%X\n", packet.type);
        failed_assertions++;
    }

    if (packet.remaining_size != 257)
    {
        printf("Byte 3: Expected remaining_size to be 257 but was %i\n", packet.remaining_size);
        failed_assertions++;
    }

    unpack_mqtt_packet(bytes, 4, &packet);

    if (packet.size != 49414)
    {
        printf("Byte 4: Expected size to be 49414 but was %i\n", packet.size);
        failed_assertions++;
    }

    if (packet.fixed_header_size != 5)
    {
        printf("Byte 4: Expected fixed_header_size to be 5 but was %i\n", packet.fixed_header_size);
        failed_assertions++;
    }
    
    if (packet.remaining_size != 49409)
    {
        printf("Byte 4: Expected remaining_size to be 49409 but was %i\n", packet.remaining_size);
        failed_assertions++;
    }

    if (packet.type != 0x03)
    {
        printf("Byte 4: Expected type to be 0x03 but was 0x%X\n", packet.type);
        failed_assertions++;
    }

    unpack_mqtt_packet(bytes, 5, &packet);

    if (packet.size != 8438022)
    {
        printf("Byte 5: Expected size to be 8438022 but was %i\n", packet.size);
        failed_assertions++;
    }

    if (packet.fixed_header_size != 5)
    {
        printf("Byte 5: Expected fixed_header_size to be 5 but was %i\n", packet.fixed_header_size);
        failed_assertions++;
    }
    
    if (packet.type != 0x03)
    {
        printf("Byte 5: Expected type to be 0x03 but was 0x%X\n", packet.type);
        failed_assertions++;
    }

    if (packet.remaining_size != 8438017)
    {
        printf("Byte 5: Expected remaining_size to be 8438017 but was %i\n", packet.remaining_size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_unpack_only_one_packet()
{
    printf("It should unpack only one packet\n");
    failed_assertions = 0;

    uint8_t bytes[] = { 0xC0, 0x00, 0xD0, 0x00, 0x40, 0x02, 0xAA };
    mqtt_packet_t packet;

    unpack_mqtt_packet(bytes, 7, &packet);

    if (packet.size != 2)
    {
        printf("Callback 1: Expected size to be 2 but was %i\n", packet.size);
        failed_assertions++;
    }

    if (packet.fixed_header_size != 2)
    {
        printf("Callback 1: Expected fixed_header_size to be 2 but was %i\n", packet.fixed_header_size);
        failed_assertions++;
    }
    
    if (packet.type != 0x0C)
    {
        printf("Callback 1: Expected type to be 0x0C but was 0x%X\n", packet.type);
        failed_assertions++;
    }
        
    if (packet.remaining_size != 0)
    {
        printf("Callback 1: Expected remaining_size to be 0 but was %i\n", packet.remaining_size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_unpack_the_packet_identifier_which_is_after_the_fixed_header()
{
    printf("It should unpack the packet identifier which is after the fixed header\n");
    failed_assertions = 0;

    uint8_t bytes[] = { 0x40, 0x02, 0xAA, 0xBB };
    mqtt_packet_t packet;
    unpack_mqtt_packet(bytes, 4, &packet);

    uint16_t packet_identifier = 0;
    unpack_mqtt_packet_identifier(bytes, &packet, &packet_identifier);

    if (packet_identifier != 0xAABB)
    {
        printf("Expected packet_identifier to be 0xAABB but was 0x%X\n", packet_identifier);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_unpack_a_connack_packet()
{
    printf("It should unpack a CONNACK packet\n");
    failed_assertions = 0;

    uint8_t bytes[] = { 0x20, 0x02, 0x01, 0x02 };
    mqtt_packet_t packet;
    unpack_mqtt_packet(bytes, 4, &packet);

    mqtt_connack_packet_t connack;
    unpack_mqtt_connack(bytes, &packet, &connack);

    if (connack.session_present != 1)
    {
        printf("Expected session_present to be 1 but was %i\n", connack.session_present);
        failed_assertions++;
    }

    if (connack.return_code != 0x02)
    {
        printf("Expected return_code to be 0x02 but was 0x%X\n", connack.return_code);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_unpack_a_publish_packet()
{
    printf("It should unpack a PUBLISH packet\n");
    failed_assertions = 0;

    uint8_t bytes[] = { 0x3D, 0x0C, 0x00, 0x05, 0x54, 0x4F, 0x50, 0x49, 0x43, 0xAA, 0xBB, 0x00, 0x01, 0x02 };
    mqtt_packet_t packet;

    unpack_mqtt_packet(bytes, 14, &packet);

    mqtt_publish_packet_t publish;
    unpack_mqtt_publish(bytes, &packet, &publish);

    if (publish.dup != 1)
    {
        printf("Expected dup to be 1 but was %i\n", publish.dup);
        failed_assertions++;
    }

    if (publish.qos != MQTT_QOS_EXACTLY_ONCE)
    {
        printf("Expected qos to be 0x02 but was 0x%X\n", publish.qos);
        failed_assertions++;
    }

    if (publish.retain != 1)
    {
        printf("Expected retain to be 1 but was %i\n", publish.dup);
        failed_assertions++;
    }

    if (publish.topic_name != &(bytes[4]))
    {
        printf("Expected topic_name to be %p but was %p\n", &(bytes[4]), publish.topic_name);
        failed_assertions++;
    }

    if (publish.topic_name_size != 5)
    {
        printf("Expected topic_name_size to be 5 but was %i\n", publish.topic_name_size);
        failed_assertions++;
    }

    if (publish.packet_identifier != 0xAABB)
    {
        printf("Expected packet_identifier to be 0xAABB but was 0x%X\n", publish.packet_identifier);
        failed_assertions++;
    }

    if (publish.payload != &(bytes[11]))
    {
        printf("Expected payload to be %p but was %p\n", &(bytes[11]), publish.payload);
        failed_assertions++;
    }

    if (publish.payload_size != 3)
    {
        printf("Expected payload_size to be 3 but was %i\n", publish.payload_size);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_unpack_a_publish_packet_without_packet_identifier()
{
    printf("It should unpack a PUBLISH packet\n");
    failed_assertions = 0;

    uint8_t bytes[] = { 0x30, 0x2F, 0x00, 0x2C, 0x24, 0x53, 0x59, 0x53,
                        0x2F, 0x56, 0x65, 0x72, 0x6E, 0x65, 0x4D, 0x51,
                        0x40, 0x31, 0x37, 0x32, 0x2E, 0x32, 0x39, 0x2E,
                        0x30, 0x2E, 0x32, 0x2F, 0x6D, 0x71, 0x74, 0x74,
                        0x2F, 0x64, 0x69, 0x73, 0x63, 0x6F, 0x6E, 0x6E,
                        0x65, 0x63, 0x74, 0x2F, 0x73, 0x65, 0x6E, 0x74, 
                        0x30 };
    mqtt_packet_t packet;

    unpack_mqtt_packet(bytes, sizeof(bytes), &packet);
    if (packet.size != sizeof(bytes))
    {
        printf("Expected packet size to be %lu but was %i\n", sizeof(bytes), packet.size);
        failed_assertions++;
    }

    mqtt_publish_packet_t publish;
    unpack_mqtt_publish(bytes, &packet, &publish);

    if (publish.dup != 0)
    {
        printf("Expected dup to be 1 but was %i\n", publish.dup);
        failed_assertions++;
    }

    if (publish.qos != MQTT_QOS_AT_MOST_ONCE)
    {
        printf("Expected qos to be 0x02 but was 0x%X\n", publish.qos);
        failed_assertions++;
    }

    if (publish.retain != 0)
    {
        printf("Expected retain to be 1 but was %i\n", publish.dup);
        failed_assertions++;
    }

    if (publish.topic_name != &(bytes[4]))
    {
        printf("Expected topic_name to be %p but was %p\n", &(bytes[4]), publish.topic_name);
        failed_assertions++;
    }

    if (publish.topic_name_size != 44)
    {
        printf("Expected topic_name_size to be 44 but was %i\n", publish.topic_name_size);
        failed_assertions++;
    }

    if (publish.packet_identifier != 0x0000)
    {
        printf("Expected packet_identifier to be 0x0000 but was 0x%X\n", publish.packet_identifier);
        failed_assertions++;
    }

    if (publish.payload != &(bytes[48]))
    {
        printf("Expected payload to be %p but was %p\n", &(bytes[11]), publish.payload);
        failed_assertions++;
    }

    if (publish.payload_size != 1)
    {
        printf("Expected payload_size to be 1 but was %i\n", publish.payload_size);
        printf("%.*s\n", publish.payload_size, publish.payload);
        failed_assertions++;
    }

    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}

static uint16_t it_should_unpack_a_suback_packet()
{
    printf("It should unpack a SUBACK packet\n");
    failed_assertions = 0;

    uint8_t bytes[] = { 0x90, 0x03, 0xAA, 0xBB, 0x02 };
    mqtt_packet_t packet;

    unpack_mqtt_packet(bytes, 5, &packet);

    mqtt_suback_packet_t suback;
    unpack_mqtt_suback(bytes, &packet, &suback);

    if (suback.packet_identifier != 0xAABB)
    {
        printf("Expected packet_identifier to be 0xAABB but was 0x%X\n", suback.packet_identifier);
        failed_assertions++;
    }

    if (suback.return_code != 0x02)
    {
        printf("Expected return_code to be 0x02 but was 0x%X\n", suback.return_code);
        failed_assertions++;
    }
    
    printf("Failed assertions: %i\n\n", failed_assertions);
    return failed_assertions;
}
