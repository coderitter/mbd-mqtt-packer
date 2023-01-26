#include <mqtt_packer.h>

void unpack_mqtt_packet
(
    uint8_t* bytes, 
    uint32_t size,
    mqtt_packet_t* packet
)
{
    // Read the first of two bytes of the fixed header which is always present
    if (size >= 1)
    {
        packet->fixed_header_size = 2;
        packet->type = (bytes[0] & 0xF0) >> 4;
        packet->remaining_size = 0;
    }

    // Read the second of two bytes of the fixed header which is always present and
    // which contains the first byte of the remaining length
    if (size >= 2)
    {
        packet->remaining_size = bytes[1] & 0x7F;

        if ((bytes[1] & 0x80) == 128)
        {
            packet->fixed_header_size++;
        }
    }

    // Read the second byte of the remaining length if present
    if (size >= 3 && packet->fixed_header_size >= 3)
    {
        packet->remaining_size += (bytes[2] & 0x7F) << 7;

        if ((bytes[2] & 0x80) == 128)
        {
            packet->fixed_header_size++;
        }
    }

    // Read the third byte of the remaining length if present
    if (size >= 4 && packet->fixed_header_size >= 4)
    {
        packet->remaining_size += (bytes[3] & 0x7F) << 14;
    
        if ((bytes[3] & 0x80) == 128)
        {
            packet->fixed_header_size++;
        }
    }

    // Read the fourth and last byte of the remaining length if present
    if (size >= 5 && packet->fixed_header_size >= 5)
    {
        packet->remaining_size += (bytes[4] & 0x7F) << 21;

        if ((bytes[4] & 0x80) == 128)
        {
            // The continuation bit of the fourth length byte is set
            // which represents a malformed length byte. Ignore it.
        }
    }

    // Calculate the size that we can calculate until now, even if it is incomplete
    packet->size = packet->fixed_header_size + packet->remaining_size;
}

void unpack_mqtt_packet_identifier(uint8_t* bytes, mqtt_packet_t* packet, uint16_t* packet_identifier)
{
    if
    (
        packet->type == MQTT_PACKET_PUBACK ||
        packet->type == MQTT_PACKET_PUBREC ||
        packet->type == MQTT_PACKET_PUBCOMP ||
        packet->type == MQTT_PACKET_SUBACK ||
        packet->type == MQTT_PACKET_UNSUBACK
    )
    {
        (*packet_identifier) = (bytes[packet->fixed_header_size] << 8) + bytes[packet->fixed_header_size + 1];
    }
}

void unpack_mqtt_connack(uint8_t* bytes, mqtt_packet_t* packet, mqtt_connack_packet_t* connack_packet)
{    
    connack_packet->session_present = bytes[2] & 0x01 ? 1 : 0;
    connack_packet->return_code = bytes[3];
}

void unpack_mqtt_publish(uint8_t* bytes, mqtt_packet_t* packet, mqtt_publish_packet_t* publish_packet)
{
    publish_packet->dup = bytes[0] & 0x08 ? 1 : 0;
    publish_packet->qos = (bytes[0] & 0x06) >> 1;
    publish_packet->retain = bytes[0] & 0x01 ? 1 : 0;

    publish_packet->topic_name = &(bytes[packet->fixed_header_size + 2]);
    publish_packet->topic_name_size = (bytes[packet->fixed_header_size] << 8) + bytes[packet->fixed_header_size + 1];

    uint8_t packet_identifier_length = 2;

    if (publish_packet->qos == 0) 
    {
        packet_identifier_length = 0;
        publish_packet->packet_identifier = 0;
    }
    else 
    {
        // Fixed header size + topic name size value + topic name size
        uint16_t packet_identifier_position = packet->fixed_header_size + 2 + publish_packet->topic_name_size;
        publish_packet->packet_identifier = (bytes[packet_identifier_position] << 8) + bytes[packet_identifier_position + 1];
    }

    // Fixed_header_size_size
    publish_packet->variable_header_size = publish_packet->topic_name_size + packet_identifier_length + 2;

    // Fixed header size + topic name size value + topic name size + packet identifier
    publish_packet->payload = &(bytes[packet->fixed_header_size + 2 + publish_packet->topic_name_size + packet_identifier_length]);
    // Remaining size - topic name size value - topic name size - packet identifier
    publish_packet->payload_size = packet->remaining_size - 2 - publish_packet->topic_name_size - packet_identifier_length;
}

void unpack_mqtt_suback(uint8_t* bytes, mqtt_packet_t* packet, mqtt_suback_packet_t* subAckPacket)
{
    subAckPacket->packet_identifier = (bytes[packet->fixed_header_size] << 8) + bytes[packet->fixed_header_size + 1];
    subAckPacket->return_code = bytes[packet->fixed_header_size + 2];
}
