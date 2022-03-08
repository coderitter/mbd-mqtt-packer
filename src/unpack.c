#include <stdio.h>
#include <string.h>
#include "mqtt_packer.h"

void unpackMqttPacket
(
    uint8_t *bytes, 
    uint32_t size,
    struct MqttPacket *packet
)
{
    // Read the first of two bytes of the fixed header which is always present
    if (size >= 1)
    {
        packet->fixedHeaderSize = 2;
        packet->type = (bytes[0] & 0xF0) >> 4;
        packet->remainingSize = 0;
    }

    // Read the second of two bytes of the fixed header which is always present and
    // which contains the first byte of the remaining length
    if (size >= 2)
    {
        packet->remainingSize = bytes[1] & 0x7F;

        if ((bytes[1] & 0x80) == 128)
        {
            packet->fixedHeaderSize++;
        }
    }

    // Read the second byte of the remaining length if present
    if (size >= 3 && packet->fixedHeaderSize >= 3)
    {
        packet->remainingSize += (bytes[2] & 0x7F) << 7;

        if ((bytes[2] & 0x80) == 128)
        {
            packet->fixedHeaderSize++;
        }
    }

    // Read the third byte of the remaining length if present
    if (size >= 4 && packet->fixedHeaderSize >= 4)
    {
        packet->remainingSize += (bytes[3] & 0x7F) << 14;
    
        if ((bytes[3] & 0x80) == 128)
        {
            packet->fixedHeaderSize++;
        }
    }

    // Read the fourth and last byte of the remaining length if present
    if (size >= 5 && packet->fixedHeaderSize >= 5)
    {
        packet->remainingSize += (bytes[4] & 0x7F) << 21;

        if ((bytes[4] & 0x80) == 128)
        {
            // The continuation bit of the fourth length byte is set
            // which represents a malformed length byte. Ignore it.
        }
    }

    // Calculate the size that we can calculate until now, even if it is incomplete
    packet->size = packet->fixedHeaderSize + packet->remainingSize;
}

void unpackMqttPacketIdentifier(uint8_t *bytes, struct MqttPacket *packet, uint16_t *packetIdentifier)
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
        (*packetIdentifier) = (bytes[packet->fixedHeaderSize] << 8) + bytes[packet->fixedHeaderSize + 1];
    }
}

void unpackMqttConnAck(uint8_t *bytes, struct MqttPacket *packet, struct MqttConnAckPacket *connAckPacket)
{    
    connAckPacket->sessionPresent = bytes[2] & 0x01 ? 1 : 0;
    connAckPacket->returnCode = bytes[3];
}

void unpackMqttPublish(uint8_t *bytes, struct MqttPacket *packet, struct MqttPublishPacket *publishPacket)
{
    publishPacket->dup = bytes[0] & 0x08 ? 1 : 0;
    publishPacket->qos = (bytes[0] & 0x06) >> 1;
    publishPacket->retain = bytes[0] & 0x01 ? 1 : 0;

    publishPacket->topicName = &(bytes[packet->fixedHeaderSize + 2]);
    publishPacket->topicNameSize = (bytes[packet->fixedHeaderSize] << 8) + bytes[packet->fixedHeaderSize + 1];

    // Fixed header size + topic name size value + topic name size
    uint16_t packetIdentifierPosition = packet->fixedHeaderSize + 2 + publishPacket->topicNameSize;
    publishPacket->packetIdentifier = (bytes[packetIdentifierPosition] << 8) + bytes[packetIdentifierPosition + 1];

    // Fixed header size + topic name size value + topic name size + packet identifier
    publishPacket->payload = &(bytes[packet->fixedHeaderSize + 2 + publishPacket->topicNameSize + 2]);
    // Remaining size - topic name size value - topic name size - packet identifier
    publishPacket->payloadSize = packet->remainingSize - 2 - publishPacket->topicNameSize - 2;
}

void unpackMqttSubAck(uint8_t *bytes, struct MqttPacket *packet, struct MqttSubAckPacket *subAckPacket)
{
    subAckPacket->packetIdentifier = (bytes[packet->fixedHeaderSize] << 8) + bytes[packet->fixedHeaderSize + 1];
    subAckPacket->returnCode = bytes[packet->fixedHeaderSize + 2];
}