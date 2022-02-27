#include <stdio.h>
#include <string.h>
#include "unpack.h"

int32_t unpackMqttChunk
(
    struct MqttPacket *packet, 
    int32_t *currentSize, 
    int32_t chunkSize,
    void (*onMqttPacketDecoded) (struct MqttPacket *packet)
)
{
    // Since one chunk might containt more than one MQTT packet, we need
    // to repeat parsing until all MQTT packets are processed.
    do
    {
        // If the size is zero, initialize everything
        if (packet->size == 0)
        {
            packet->fixedHeaderSize = 0;
            packet->controlPacketType = 0;
            packet->flags = 0;
            packet->remainingSize = 0;
        } 

        // Read the first of two bytes of the fixed header which is always present
        if (packet->size == 0 && (*currentSize) >= 1)
        {
            packet->fixedHeaderSize = 2;
            packet->controlPacketType = packet->bytes[0] & MQTT_CONTROL_PACKET_TYPE;
            packet->flags = packet->bytes[0] & 0xF;
        }

        // Read the second of two bytes of the fixed header which is always present and
        // which contains the first byte of the remaining length
        if (packet->size <= 1 && (*currentSize) >= 2)
        {
            packet->remainingSize = packet->bytes[1] & 0x7F;

            if ((packet->bytes[1] & 0x80) == 128)
            {
                packet->fixedHeaderSize++;
            }
        }

        // Read the second byte of the remaining length if present
        if (packet->size <= 2 && (*currentSize) >= 3 && packet->fixedHeaderSize >= 3)
        {
            packet->remainingSize += (packet->bytes[2] & 0x7F) << 7;

            if ((packet->bytes[2] & 0x80) == 128)
            {
                packet->fixedHeaderSize++;
            }
        }

        // Read the third byte of the remaining length if present
        if (packet->size <= 3 && (*currentSize) >= 4 && packet->fixedHeaderSize >= 4)
        {
            packet->remainingSize += (packet->bytes[3] & 0x7F) << 14;
        
            if ((packet->bytes[3] & 0x80) == 128)
            {
                packet->fixedHeaderSize++;
            }
        }

        // Read the fourth and last byte of the remaining length if present
        if (packet->size <= 4 && (*currentSize) >= 5 && packet->fixedHeaderSize >= 5)
        {
            packet->remainingSize += (packet->bytes[4] & 0x7F) << 21;

            if ((packet->bytes[4] & 0x80) == 128)
            {
                // The continuation bit of the fourth length byte is set
                // which represents a malformed length byte. Ignore it.
            }
        }

        // If the current used size the byte array inside the MqttPacket struct is
        // equal or larger than the current MQTT packet we are finished parsing the
        // MQTT packet.
        if ((*currentSize) >= packet->fixedHeaderSize + packet->remainingSize)
        {
            // At first we update the size stored in the MqttPacket struct to its final size
            packet->size = packet->fixedHeaderSize + packet->remainingSize;

            // Then we call the callback
            if (onMqttPacketDecoded != 0)
            {
                onMqttPacketDecoded(packet);
            }

            // If the used size of the byte array is even larger than the just parsed
            // MQTT packet, we have at least an additional MQTT packet.
            if ((*currentSize) > packet->fixedHeaderSize + packet->remainingSize)
            {
                // In that case we need to move the bytes that belong to the next packet
                // to the beginning.
                memmove
                (
                    packet->bytes, 
                    &(packet->bytes[packet->fixedHeaderSize + packet->remainingSize]),
                    (*currentSize) - (packet->fixedHeaderSize + packet->remainingSize)
                );

                // Now we calculate a new current used size representing the amount of bytes that
                // were moved to the beginning.
                (*currentSize) = (*currentSize) - (packet->fixedHeaderSize + packet->remainingSize);
            }

            // If the used size of the array is not larger than the MQTT packet that was just
            // parsed we set the current used size to zero. Note that this change propagates back
            // to the function which owns this information and which will most likely be the
            // function which fills in the bytes of the TCP stream. That way we tell that
            // function to fill the newly arriving bytes at the beginning of the byte array.
            else
            {
                (*currentSize) = 0;
            }

            // Reset the MQTT packet
            packet->size = 0;
            packet->fixedHeaderSize = 0;
            packet->controlPacketType = 0;
            packet->flags = 0;
            packet->remainingSize = 0;
        }
        else
        {
            // By setting the size found in the MqttPacket struct to the size of the currently used
            // bytes of the array we will tell the while-loop that we are done parsing for this chunk.
            packet->size = (*currentSize);
        }
    }
    while (packet->size != (*currentSize));

    // Return the packet size which was determined until now
    return packet->fixedHeaderSize + packet->remainingSize;
}

void unpackMqttPacketIdentifier(struct MqttPacket *packet, uint16_t *packetIdentifier)
{
    if
    (
        packet->controlPacketType == MQTT_CONTROL_PACKET_TYPE_PUBACK ||
        packet->controlPacketType == MQTT_CONTROL_PACKET_TYPE_PUBREC ||
        packet->controlPacketType == MQTT_CONTROL_PACKET_TYPE_PUBCOMP ||
        packet->controlPacketType == MQTT_CONTROL_PACKET_TYPE_SUBACK ||
        packet->controlPacketType == MQTT_CONTROL_PACKET_TYPE_UNSUBACK
    )
    {
        (*packetIdentifier) = (packet->bytes[packet->fixedHeaderSize] << 8) + packet->bytes[packet->fixedHeaderSize + 1];
    }
    else if (packet->controlPacketType == MQTT_CONTROL_PACKET_TYPE_PUBLISH)
    {
        uint8_t *topicName = 0;
        uint16_t topicNameSize = 0;
        unpackMqttPublishTopicName(packet, &topicName, &topicNameSize);

        // Fixed header size + topic name size value + topic name size
        uint16_t packetIdentifierPosition = packet->fixedHeaderSize + 2 + topicNameSize;

        (*packetIdentifier) = (packet->bytes[packetIdentifierPosition] << 8) + packet->bytes[packetIdentifierPosition + 1];
    }
}

void unpackMqttPublishTopicName(struct MqttPacket *packet, uint8_t **topicName, uint16_t *topicNameSize)
{
    (*topicName) = &(packet->bytes[packet->fixedHeaderSize + 2]);
    (*topicNameSize) = (packet->bytes[packet->fixedHeaderSize] << 8) + packet->bytes[packet->fixedHeaderSize + 1];
}

void unpackMqttPublishPayload(struct MqttPacket *packet, uint8_t **payload, uint16_t *payloadSize)
{
    uint8_t *topicName = 0;
    uint16_t topicNameSize = 0;
    unpackMqttPublishTopicName(packet, &topicName, &topicNameSize);

    // Fixed header size + topic name size value + topic name size + packet identifier
    uint16_t payloadPosition = packet->fixedHeaderSize + 2 + topicNameSize + 2;
    (*payload) = &(packet->bytes[payloadPosition]);

    // Remaining size - topic name size value - topic name size - packet identifier
    (*payloadSize) = packet->remainingSize - 2 - topicNameSize - 2;
}
