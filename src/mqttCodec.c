#include <stdio.h>
#include <string.h>
#include "mqttCodec.h"

int32_t decodeMqttChunk
(
    struct MqttMessage *mqttMessage, 
    int32_t *currentSize, 
    int32_t chunkSize,
    void (*onMqttMessageDecoded) (struct MqttMessage *mqttMessage)
)
{
    // Since one chunk might containt more than one MQTT message, we need
    // to repeat parsing until all MQTT messages are processed.
    do
    {
        // If the size is zero, initialize everything
        if (mqttMessage->size == 0)
        {
            mqttMessage->fixedHeaderSize = 0;
            mqttMessage->controlPacketType = 0;
            mqttMessage->flags = 0;
            mqttMessage->remainingSize = 0;
        } 

        // Read the first of two bytes of the fixed header which is always present
        if (mqttMessage->size == 0 && (*currentSize) >= 1)
        {
            mqttMessage->fixedHeaderSize = 2;
            mqttMessage->controlPacketType = mqttMessage->bytes[0] & MQTT_CONTROL_PACKET_TYPE;
            mqttMessage->flags = mqttMessage->bytes[0] & 0xF;
        }

        // Read the second of two bytes of the fixed header which is always present and
        // which contains the first byte of the remaining length
        if (mqttMessage->size <= 1 && (*currentSize) >= 2)
        {
            mqttMessage->remainingSize = mqttMessage->bytes[1] & 0x7F;

            if ((mqttMessage->bytes[1] & 0x80) == 128)
            {
                mqttMessage->fixedHeaderSize++;
            }
        }

        // Read the second byte of the remaining length if present
        if (mqttMessage->size <= 2 && (*currentSize) >= 3 && mqttMessage->fixedHeaderSize >= 3)
        {
            mqttMessage->remainingSize += (mqttMessage->bytes[2] & 0x7F) << 7;

            if ((mqttMessage->bytes[2] & 0x80) == 128)
            {
                mqttMessage->fixedHeaderSize++;
            }
        }

        // Read the third byte of the remaining length if present
        if (mqttMessage->size <= 3 && (*currentSize) >= 4 && mqttMessage->fixedHeaderSize >= 4)
        {
            mqttMessage->remainingSize += (mqttMessage->bytes[3] & 0x7F) << 14;
        
            if ((mqttMessage->bytes[3] & 0x80) == 128)
            {
                mqttMessage->fixedHeaderSize++;
            }
        }

        // Read the fourth and last byte of the remaining length if present
        if (mqttMessage->size <= 4 && (*currentSize) >= 5 && mqttMessage->fixedHeaderSize >= 5)
        {
            mqttMessage->remainingSize += (mqttMessage->bytes[4] & 0x7F) << 21;

            if ((mqttMessage->bytes[4] & 0x80) == 128)
            {
                // The continuation bit of the fourth length byte is set
                // which represents a malformed length byte. Ignore it.
            }
        }

        // If the current used size the byte array inside the MqttMessage struct is
        // equal or larger than the current MQTT message we are finished parsing the
        // MQTT message.
        if ((*currentSize) >= mqttMessage->fixedHeaderSize + mqttMessage->remainingSize)
        {
            // At first we update the size stored in the MqttMessage struct to its final size
            mqttMessage->size = mqttMessage->fixedHeaderSize + mqttMessage->remainingSize;

            // Then we call the callback
            if (onMqttMessageDecoded != 0)
            {
                onMqttMessageDecoded(mqttMessage);
            }

            // If the used size of the byte array is even larger than the just parsed
            // MQTT message, we have at least an additional MQTT message.
            if ((*currentSize) > mqttMessage->fixedHeaderSize + mqttMessage->remainingSize)
            {
                // In that case we need to move the bytes that belong to the next message
                // to the beginning.
                memmove
                (
                    mqttMessage->bytes, 
                    &(mqttMessage->bytes[mqttMessage->fixedHeaderSize + mqttMessage->remainingSize]),
                    (*currentSize) - (mqttMessage->fixedHeaderSize + mqttMessage->remainingSize)
                );

                // Now we calculate a new current used size representing the amount of bytes that
                // were moved to the beginning.
                (*currentSize) = (*currentSize) - (mqttMessage->fixedHeaderSize + mqttMessage->remainingSize);
            }

            // If the used size of the array is not larger than the MQTT message that was just
            // parsed we set the current used size to zero. Note that this change propagates back
            // to the function which owns this information and which will most likely be the
            // function which fills in the bytes of the TCP stream. That way we tell that
            // function to fill the newly arriving bytes at the beginning of the byte array.
            else
            {
                (*currentSize) = 0;
            }

            // Reset the MQTT message
            mqttMessage->size = 0;
            mqttMessage->fixedHeaderSize = 0;
            mqttMessage->controlPacketType = 0;
            mqttMessage->flags = 0;
            mqttMessage->remainingSize = 0;
        }
        else
        {
            // By setting the size found in the MqttMessage struct to the size of the currently used
            // bytes of the array we will tell the while-loop that we are done parsing for this chunk.
            mqttMessage->size = (*currentSize);
        }
    }
    while (mqttMessage->size != (*currentSize));

    // Return the message size which was determined until now
    return mqttMessage->fixedHeaderSize + mqttMessage->remainingSize;
}

void decodeMqttPacketIdentifier(uint8_t *bytes, uint16_t *packetIdentifier)
{

}

void decodeMqttPublishTopicName(uint8_t *bytes, uint8_t *topicName, uint16_t *topicNameSize)
{

}

void decodeMqttPublishPacketIdentifier(uint8_t *bytes, uint8_t *packetIdentifier)
{

}

void decodeMqttPublishPayload(uint8_t *bytes, uint8_t *payload, uint16_t *payloadSize)
{
    
}

uint8_t getMqttRemainingLengthSize(uint32_t remainingLength)
{
    
    if (remainingLength <= 0x7F)
    {
        return 1;
    }

    if (remainingLength <= 0x3FFF)
    {
        return 2;
    }

    if (remainingLength <= 0x1FFFF)
    {
        return 3;
    }

    return 4;
}

uint32_t getMqttConnectSize(struct MqttConnectParameter *parameter)
{
    // Variable header size: Protocol name + protocol level + connect flags + keep alive
    uint32_t size = 11;

    // Client identifier
    size += parameter->clientIdentifierSize ? 2 + parameter->clientIdentifierSize : 0;

    // Will topic
    size += parameter->willTopicSize ? 2 + parameter->willTopicSize : 0;

    // Will message
    size += parameter->willMessageSize ? 2 + parameter->willMessageSize : 0;

    // Username
    size += parameter->userNameSize ? 2 + parameter->userNameSize : 0;

    // Password
    size += parameter->passwordSize ? 2 + parameter->passwordSize : 0;

    // Fixed header - Remaining length
    size += getMqttRemainingLengthSize(size);

    // Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
    size += 1;

    return size;
}

uint32_t getMqttPublishSize(struct MqttPublishParameter *parameter)
{
    // Topic name
    uint32_t size = 2 + parameter->topicNameSize;

    // Packet identifier - The Packet Identifier field is only present in PUBLISH Packets where the QoS level is 1 or 2.
    if (parameter->qos == MQTT_PUBLISH_FIXED_HEADER_FLAG_QOS_AT_LEAST_ONCE || parameter->qos == MQTT_PUBLISH_FIXED_HEADER_FLAG_QOS_EXACTLY_ONCE)
    {
        size += 2;
    }

    // Payload
    size += parameter->payloadSize;

    // Fixed header - Remaining length
    size += getMqttRemainingLengthSize(size);

    // Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
    size += 1;

    return size;
}

uint32_t getMqttPubAckSize()
{
    return 4;
}

uint32_t getMqttPubRecSize()
{
    return 4;
}

uint32_t getMqttPubRelSize()
{
    return 4;
}

uint32_t getMqttPubCompSize()
{
    return 4;
}

uint32_t getMqttSubscribeSize(struct MqttUnSubscribeParameter *parameter)
{
    // Variable header
    uint32_t size = 2;

    // Payload - Topic filter + QoS
    size += 2 + parameter->topicFilterSize + 1;

    // Fixed header - Remaining length
    size += getMqttRemainingLengthSize(size);

    // Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
    size += 1;

    return size;
}

uint32_t getMqttUnsubscribeSize(struct MqttUnSubscribeParameter *parameter)
{
    // Variable header
    uint32_t size = 2;

    // Payload - Topic filter
    size += 2 + parameter->topicFilterSize;

    // Fixed header - Remaining length
    size += getMqttRemainingLengthSize(size);

    // Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
    size += 1;

    return size;
}

uint32_t getMqttPingReqSize()
{
    return 2;
}

uint32_t getMqttDisconnectSize()
{
    return 2;
}

uint8_t encodeMqttRemainingLength(uint32_t remainingLength, uint8_t *bytes)
{
    uint8_t size = 0;

    // If the remaining length is smaller than 127^1
    if (remainingLength <= 0x7F)
    {
        // Set continuation bit (bit 7) to 0
        bytes[size] = (uint8_t) (remainingLength & 0x7F);
        size++;
    }
    // If the remaining length is larger than 127^1
    else
    {
        // Set continuation bit (bit 7) to 1
        bytes[size] = (uint8_t) ((remainingLength & 0x7F) | 0x80);
        size++;
    } 

    // If the remaining length is smaller than 127^2
    if (remainingLength > 0x7F && remainingLength <= 0x3FFF)
    {
        // Set continuation bit to 0
        bytes[size] = (uint8_t) ((remainingLength >> 7) & 0x7F);
        size++;
    }
    // If the remaining length is larger than 127^1
    else if (remainingLength > 0x7F)
    {
        // Set continuation bit to 1
        bytes[size] = (uint8_t) ((((remainingLength >> 7) & 0x7F) | 0x80));
        size++;
    }

    // If the remaining length is smaller than 127^3
    if (remainingLength > 0x3FFF && remainingLength <= 0x1FFFF)
    {
        // Set continuation bit to 0
        bytes[size] = (uint8_t) ((remainingLength >> 14) & 0x7F);
        size++;
    }
    // If the remaining length is larger than 127^2
    else if (remainingLength > 0x3FFF)
    {
        // Set continuation bit to 1
        bytes[size] = (uint8_t) ((((remainingLength >> 14) & 0x7F) | 0x80));
        size++;
    }

    // If the remaining length is larger than 127^3
    if (remainingLength > 0x1FFFF)
    {
        // Set continuation bit to 0
        bytes[size] = (uint8_t) ((remainingLength >> 21) & 0x7F);
        size++;
    }

    return size;
}

/**
 * @brief Client requests a connection to a Server.
 * 
 * [MQTT-3.1.0-1] After a Network Connection is established by a Client to a Server, the first Packet sent from the Client to the Server MUST be a CONNECT Packet.
 * [MQTT-3.1.0-2] The Server MUST process a second CONNECT Packet sent from a Client as a protocol violation and disconnect the Client.
 * 
 * @param parameter 
 * @param bytes 
 */
uint32_t encodeMqttConnect(struct MqttConnectParameter *parameter, uint8_t *bytes)
{
    uint32_t size = 0;

    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     */

    bytes[size] = MQTT_CONTROL_PACKET_TYPE_CONNECT;
    size++;

    /**
     * Fixed header - Remaining length
     */

    uint32_t remainingLength = 0;

    // Variable header size: Protocol name + protocol level + connect flags + keep alive
    remainingLength += 10;

    // Client identifier
    remainingLength += parameter->clientIdentifierSize ? 2 + parameter->clientIdentifierSize : 2;

    // Will topic
    remainingLength += parameter->willTopicSize ? 2 + parameter->willTopicSize : 0;

    // Will message
    remainingLength += parameter->willMessageSize ? 2 + parameter->willMessageSize : 0;

    // Username
    remainingLength += parameter->userNameSize ? 2 + parameter->userNameSize : 0;

    // Password
    remainingLength += parameter->passwordSize ? 2 + parameter->passwordSize : 0;

    size += encodeMqttRemainingLength(remainingLength, &(bytes[size]));

    /**
     * Variable header - Protocol name
     * 
     * The Protocol Name is a UTF-8 encoded string that represents the protocol name “MQTT”, capitalized as shown. The string, its offset and length will not be changed by future versions of the MQTT specification.
     * 
     * [MQTT-3.1.2-1] If the protocol name is incorrect the Server MAY disconnect the Client, or it MAY continue processing the CONNECT packet in accordance with some other specification. In the latter case, the Server MUST NOT continue to process the CONNECT packet in line with this specification.
     */
    
    // Length MSB
    bytes[size] = 0x00;
    size++;
    // Length LSB
    bytes[size] = 0x04;
    size++;
    // Protocol name 
    memcpy(&(bytes[size]), "MQTT", 4);
    size += 4;
    
    /**
     * Variable header - Protocol level
     * 
     * The 8 bit unsigned value that represents the revision level of the protocol used by the Client. The value of the Protocol Level field for the version 3.1.1 of the protocol is 4 (0x04).
     * 
     * [MQTT-3.1.2-2] The Server MUST respond to the CONNECT Packet with a CONNACK return code 0x01 (unacceptable protocol level) and then disconnect the Client if the Protocol Level is not supported by the Server.
     */
    bytes[size] = 0x04;
    size++;

    /**
     * Variable header - Connect flags
     * 
     * The Connect Flags byte contains a number of parameters specifying the behavior of the MQTT connection. It also indicates the presence or absence of fields in the payload.
     * 
     * [MQTT-3.1.2-3] The Server MUST validate that the reserved flag in the CONNECT Control Packet is set to zero and disconnect the Client if it is not zero.
     */
    uint8_t connectFlags = 0;

    // [MQTT-3.1.3-7] If the Client supplies a zero-byte ClientId, the Client MUST also set CleanSession to 1.
    if (parameter->cleanSession || parameter->clientIdentifierSize == 0)
    {
        connectFlags |= MQTT_CONNECT_VARIABLE_HEADER_CONNECT_FLAG_CLEAN_SESSION;
    }

    if (parameter->willMessageSize > 0 || parameter->willTopicSize > 0)
    {
        connectFlags |= MQTT_CONNECT_VARIABLE_HEADER_CONNECT_FLAG_WILL_FLAG;
    }

    if (parameter->willQos)
    {
        connectFlags |= parameter->willQos;
    }

    if (parameter->willRetain)
    {
        connectFlags |= MQTT_CONNECT_VARIABLE_HEADER_CONNECT_FLAG_WILL_RETAIN;
    }

    if (parameter->userNameSize > 0)
    {
        connectFlags |= MQTT_CONNECT_VARIABLE_HEADER_CONNECT_FLAG_WILL_USERNAME;
    }

    if (parameter->passwordSize > 0)
    {
        connectFlags |= MQTT_CONNECT_VARIABLE_HEADER_CONNECT_FLAG_WILL_PASSWORD;
    }

    bytes[size] = connectFlags;
    size++;

    /**
     * Variable header - Keep alive
     */
    bytes[size] = (uint8_t) (parameter->keepAlive >> 8);
    size++;
    bytes[size] = (uint8_t) (parameter->keepAlive);
    size++;

    /**
     * Payload - Client identifier
     * 
     * The Client Identifier (ClientId) identifies the Client to the Server.
     * 
     * [MQTT-3.1.3-2] Each Client connecting to the Server has a unique ClientId. The ClientId MUST be used by Clients and by Servers to identify state that they hold relating to this MQTT Session between the Client and the Server.
     * [MQTT-3.1.3-3] The Client Identifier (ClientId) MUST be present and MUST be the first field in the CONNECT packet payload.
     * [MQTT-3.1.3-4] The ClientId MUST be a UTF-8 encoded string.
     * [MQTT-3.1.3-5] The Server MUST allow ClientIds which are between 1 and 23 UTF-8 encoded bytes in length, and that contain only the characters "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ".
     * [MQTT-3.1.3-6] A Server MAY allow a Client to supply a ClientId that has a length of zero bytes, however if it does so the Server MUST treat this as a special case and assign a unique ClientId to that Client. It MUST then process the CONNECT packet as if the Client had provided that unique ClientId.
     * [MQTT-3.1.3-7] If the Client supplies a zero-byte ClientId, the Client MUST also set CleanSession to 1.
     * [MQTT-3.1.3-8] If the Client supplies a zero-byte ClientId with CleanSession set to 0, the Server MUST respond to the CONNECT Packet with a CONNACK return code 0x02 (Identifier rejected) and then close the Network Connection.
     * [MQTT-3.1.3-9] If the Server rejects the ClientId it MUST respond to the CONNECT Packet with a CONNACK return code 0x02 (Identifier rejected) and then close the Network Connection.
     */

    // String length MSB
    bytes[size] = (uint8_t) (parameter->clientIdentifierSize >> 8);
    size++;

    // String length LSB
    bytes[size] = (uint8_t) parameter->clientIdentifierSize;
    size++;

    if (parameter->clientIdentifierSize > 0)
    {
        // UTF-8 string
        memcpy(&(bytes[size]), parameter->clientIdentifier, parameter->clientIdentifierSize);
        size += parameter->clientIdentifierSize;
    }

    /**
     * Payload - Will topic
     * 
     * If the Will Flag is set to 1, the Will Topic is the next field in the payload.
     * 
     * [MQTT-3.1.3-10] The Will Topic MUST be a UTF-8 encoded string as defined in Section 1.5.3.
     */

    if (parameter->willTopicSize > 0)
    {
        // String length MSB + LSB
        bytes[size] = (uint8_t) (parameter->willTopicSize >> 8);
        size++;
        bytes[size] = (uint8_t) parameter->willTopicSize;
        size++;

        // UTF-8 string
        memcpy(&(bytes[size]), parameter->willTopic, parameter->willTopicSize);
        size += parameter->willTopicSize;
    }

    /**
     * Payload - Will message
     * 
     * If the Will Flag is set to 1 the Will Message is the next field in the payload.
     * The Will Message defines the Application Message that is to be published to the Will Topic. 
     * This field consists of a two byte length followed by the payload for the Will Message expressed as a sequence of zero or more bytes. 
     * The length gives the number of bytes in the data that follows and does not include the 2 bytes taken up by the length itself.
     * 
     * When the Will Message is published to the Will Topic its payload consists only of the data portion of this field, not the first two length bytes.
     */

    if (parameter->willMessageSize > 0)
    {
        // String length MSB + LSB
        bytes[size] = (uint8_t) (parameter->willMessageSize >> 8);
        size++;
        bytes[size] = (uint8_t) parameter->willMessageSize;
        size++;

        // UTF-8 string
        memcpy(&(bytes[size]), parameter->willMessage, parameter->willMessageSize);
        size += parameter->willMessageSize;
    }

    /**
     * Payload - Username
     * 
     * If the User Name Flag is set to 1, this is the next field in the payload.
     * It can be used by the Server for authentication and authorization.
     * 
     * [MQTT-3.1.3-11] The User Name MUST be a UTF-8 encoded string. 
     */

    if (parameter->userNameSize > 0)
    {
        // String length MSB + LSB
        bytes[size] = (uint8_t) (parameter->userNameSize >> 8);
        size++;
        bytes[size] = (uint8_t) parameter->userNameSize;
        size++;

        // UTF-8 string
        memcpy(&(bytes[size]), parameter->userName, parameter->userNameSize);
        size += parameter->userNameSize;
    }

    /**
     * Payload - Password
     * 
     * If the Password Flag is set to 1, this is the next field in the payload. 
     * The Password field contains 0 to 65535 bytes of binary data prefixed with a two byte length field which indicates the number of bytes used by the binary data (it does not include the two bytes taken up by the length field itself).
     */

    if (parameter->passwordSize > 0)
    {
        // String length MSB + LSB
        bytes[size] = (uint8_t) (parameter->passwordSize >> 8);
        size++;
        bytes[size] = (uint8_t) parameter->passwordSize;
        size++;

        // UTF-8 string
        memcpy(&(bytes[size]), parameter->password, parameter->passwordSize);
        size += parameter->passwordSize;
    }

    return size;
}

uint32_t encodeMqttPublish(struct MqttPublishParameter *parameter, uint8_t *bytes)
{
    uint32_t size = 0;

    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     * 
     * [MQTT-3.8.1-1] Bits 3,2,1 and 0 of the fixed header of the SUBSCRIBE Control Packet are reserved and MUST be set to 0,0,1 and 0 respectively. The Server MUST treat any other value as malformed and close the Network Connection.
     */

    bytes[0] = MQTT_CONTROL_PACKET_TYPE_PUBLISH | parameter->qos;

    if (parameter->dup)
    {
        bytes[0] |= MQTT_PUBLISH_FIXED_HEADER_FLAG_DUP;
    }

    if (parameter->retain)
    {
        bytes[0] |= MQTT_PUBLISH_FIXED_HEADER_FLAG_RETAIN;
    }

    size++;

    /**
     * Fixed header - Remaining length
     */

    uint32_t remainingLength = 0;

    // Topic name
    remainingLength += 2 + parameter->topicNameSize;

    // Packet identifier - The Packet Identifier field is only present in PUBLISH Packets where the QoS level is 1 or 2.
    if (parameter->qos == MQTT_PUBLISH_FIXED_HEADER_FLAG_QOS_AT_LEAST_ONCE || parameter->qos == MQTT_PUBLISH_FIXED_HEADER_FLAG_QOS_EXACTLY_ONCE)
    {
        remainingLength += 2;
    }

    // Payload
    remainingLength += parameter->payloadSize;

    size += encodeMqttRemainingLength(remainingLength, &(bytes[size]));

    /**
     * Variable header - Topic name
     * 
     * The Topic Name identifies the information channel to which payload data is published.
     * 
     * [MQTT-3.3.2-1] The Topic Name MUST be present as the first field in the PUBLISH Packet Variable header. It MUST be a UTF-8 encoded string.
     * [MQTT-3.3.2-2] The Topic Name in the PUBLISH Packet MUST NOT contain wildcard characters.
     * [MQTT-3.3.2-3] The Topic Name in a PUBLISH Packet sent by a Server to a subscribing Client MUST match the Subscription’s Topic Filter according to the matching process.
     */

    // String length MSB + LSB
    bytes[size] = (uint8_t) (parameter->topicNameSize >> 8);
    size++;
    bytes[size] = (uint8_t) parameter->topicNameSize;
    size++;
    
    // Topic name
    memcpy(&(bytes[size]), parameter->topicName, parameter->topicNameSize);
    size += parameter->topicNameSize;

    /**
     * Variable header - Packet Identifier
     */
    bytes[size] = (uint8_t) (parameter->packetIdentifier >> 8);
    size++;
    bytes[size] = (uint8_t) parameter->packetIdentifier;
    size++;

    /**
     * Payload
     * 
     * The Payload contains the Application Message that is being published.
     * The content and format of the data is application specific.
     * The length of the payload can be calculated by subtracting the length of the variable header from the Remaining Length field that is in the Fixed Header.
     * It is valid for a PUBLISH Packet to contain a zero length payload.
     */

    memcpy(&(bytes[size]), parameter->payload, parameter->payloadSize);
    size += parameter->payloadSize;

    return size;
}

uint32_t encodeMqttPubAck(uint16_t packetIdentifier, uint8_t *bytes)
{
    uint32_t size = 0;

    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     */

    bytes[0] = MQTT_CONTROL_PACKET_TYPE_PUBACK;
    size++;

    /**
     * Fixed header - Remaining length
     */

    // Variable header
    uint32_t remainingLength = 2;
    size += encodeMqttRemainingLength(remainingLength, &(bytes[size]));

    /**
     * Variable header - Packet Identifier
     * 
     * This contains the Packet Identifier from the PUBLISH Packet that is being acknowledged.
     */
    bytes[size] = (uint8_t) (packetIdentifier >> 8);
    size++;
    bytes[size] = (uint8_t) packetIdentifier;
    size++;

    return size;
}

uint32_t encodeMqttPubRec(uint16_t packetIdentifier, uint8_t *bytes)
{
    uint32_t size = 0;

    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     */

    bytes[0] = MQTT_CONTROL_PACKET_TYPE_PUBREC;
    size++;

    /**
     * Fixed header - Remaining length
     */

    // Variable header
    uint32_t remainingLength = 2;
    size += encodeMqttRemainingLength(remainingLength, &(bytes[size]));

    /**
     * Variable header - Packet Identifier
     * 
     * This contains the Packet Identifier from the PUBLISH Packet that is being acknowledged.
     */
    memcpy(&(bytes[size]), (uint8_t*) &packetIdentifier, 2);
    size += 2;

    return size;
}

uint32_t encodeMqttPubRel(uint16_t packetIdentifier, uint8_t *bytes)
{
    uint32_t size = 0;

    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     */

    bytes[0] = MQTT_CONTROL_PACKET_TYPE_PUBREL & 0x02;
    size++;

    /**
     * Fixed header - Remaining length
     */

    // Variable header
    uint32_t remainingLength = 2;
    size += encodeMqttRemainingLength(remainingLength, &(bytes[size]));

    /**
     * Variable header - Packet Identifier
     * 
     * The variable header contains the same Packet Identifier as the PUBREC Packet that is being acknowledged.
     */
    memcpy(&(bytes[size]), (uint8_t*) &packetIdentifier, 2);
    size += 2;

    return size;
}

uint32_t encodeMqttPubComp(uint16_t packetIdentifier, uint8_t *bytes)
{
    uint32_t size = 0;

    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     */

    bytes[0] = MQTT_CONTROL_PACKET_TYPE_PUBCOMP;
    size++;

    /**
     * Fixed header - Remaining length
     */

    // Variable header
    uint32_t remainingLength = 2;
    size += encodeMqttRemainingLength(remainingLength, &(bytes[size]));

    /**
     * Variable header - Packet Identifier
     * 
     * The variable header contains the same Packet Identifier as the PUBREL Packet that is being acknowledged.
     */
    memcpy(&(bytes[size]), (uint8_t*) &packetIdentifier, 2);
    size += 2;

    return size;
}


uint32_t encodeMqttSubscribe(struct MqttUnSubscribeParameter *parameter, uint8_t *bytes)
{
    uint32_t size = 0;

    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     * 
     * [MQTT-3.8.1-1] Bits 3,2,1 and 0 of the fixed header of the SUBSCRIBE Control Packet are reserved and MUST be set to 0,0,1 and 0 respectively. The Server MUST treat any other value as malformed and close the Network Connection.
     */

    bytes[0] = MQTT_CONTROL_PACKET_TYPE_SUBSCRIBE & 0x02;
    size++;

    /**
     * Fixed header - Remaining length
     */

    // Variable header length
    uint32_t remainingLength = 2;

    // Topic filter + QoS length
    remainingLength += 2 + parameter->topicFilterSize + 1;

    size += encodeMqttRemainingLength(remainingLength, &(bytes[size]));

    /**
     * Variable header - Packet Identifier
     */
    memcpy(&(bytes[size]), (uint8_t*) &parameter->packetIdentifier, 2);
    size += 2;

    /**
     * Payload
     * 
     * The payload of a SUBSCRIBE Packet contains a list of Topic Filters indicating the Topics to which the Client wants to subscribe.
     * Each filter is followed by a byte called the Requested QoS.
     * This gives the maximum QoS level at which the Server can send Application Messages to the Client.
     * The requested maximum QoS field is encoded in the byte following each UTF-8 encoded topic name, and these Topic Filter / QoS pairs are packed contiguously.
     * The upper 6 bits of the Requested QoS byte are not used in the current version of the protocol. They are reserved for future use.
     * 
     * [MQTT-3.8.3-1] The Topic Filters in a SUBSCRIBE packet payload MUST be UTF-8 encoded strings.
     * [MQTT-3.8.3-2] A Server SHOULD support Topic filters that contain the wildcard characters defined. If it chooses not to support topic filters that contain wildcard characters it MUST reject any Subscription request whose filter contains them.
     * [MQTT-3.8.3-3] The payload of a SUBSCRIBE packet MUST contain at least one Topic Filter / QoS pair. A SUBSCRIBE packet with no payload is a protocol violation.
     * [MQTT-3-8.3-4] The Server MUST treat a SUBSCRIBE packet as malformed and close the Network Connection if any of Reserved bits in the payload are non-zero, or QoS is not 0,1 or 2.
     */

    // String length MSB + LSB
    memcpy(&(bytes[size]), (uint8_t*) &parameter->topicFilterSize, 2);
    size += 2;
    
    // Topic filter
    memcpy(&(bytes[size]), parameter->topicFilter, parameter->topicFilterSize);
    size += parameter->topicFilterSize;

    // QoS
    bytes[size] = parameter->qos;
    size++;

    return size;
}

uint32_t encodeMqttUnsubscribe(struct MqttUnSubscribeParameter *parameter, uint8_t *bytes)
{
    uint32_t size = 0;

    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     */

    bytes[0] = MQTT_CONTROL_PACKET_TYPE_UNSUBSCRIBE & 0x02;
    size++;

    /**
     * Fixed header - Remaining length
     */

    // Variable header length
    uint32_t remainingLength = 2;

    // Topic filter + QoS length
    remainingLength += 2 + parameter->topicFilterSize + 1;

    size += encodeMqttRemainingLength(remainingLength, &(bytes[size]));

    /**
     * Variable header - Packet Identifier
     */
    memcpy(&(bytes[size]), (uint8_t*) &parameter->packetIdentifier, 2);
    size += 2;

    /**
     * Payload
     * 
     * The payload of a SUBSCRIBE Packet contains a list of Topic Filters indicating the Topics to which the Client wants to subscribe.
     * Each filter is followed by a byte called the Requested QoS.
     * This gives the maximum QoS level at which the Server can send Application Messages to the Client.
     * The requested maximum QoS field is encoded in the byte following each UTF-8 encoded topic name, and these Topic Filter / QoS pairs are packed contiguously.
     * The upper 6 bits of the Requested QoS byte are not used in the current version of the protocol. They are reserved for future use.
     * 
     * [MQTT-3.8.3-1] The Topic Filters in a SUBSCRIBE packet payload MUST be UTF-8 encoded strings.
     * [MQTT-3.8.3-2] A Server SHOULD support Topic filters that contain the wildcard characters defined. If it chooses not to support topic filters that contain wildcard characters it MUST reject any Subscription request whose filter contains them.
     * [MQTT-3.8.3-3] The payload of a SUBSCRIBE packet MUST contain at least one Topic Filter / QoS pair. A SUBSCRIBE packet with no payload is a protocol violation.
     * [MQTT-3-8.3-4] The Server MUST treat a SUBSCRIBE packet as malformed and close the Network Connection if any of Reserved bits in the payload are non-zero, or QoS is not 0,1 or 2.
     */

    // String length MSB + LSB
    memcpy(&(bytes[size]), (uint8_t*) &parameter->topicFilterSize, 2);
    size += 2;
    
    // Topic filter
    memcpy(&(bytes[size]), parameter->topicFilter, parameter->topicFilterSize);
    size += parameter->topicFilterSize;

    return size;
}

uint32_t encodeMqttPingReq(uint8_t *bytes)
{
    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     */

    bytes[0] = MQTT_CONTROL_PACKET_TYPE_PINGREQ;

    /**
     * Fixed header - Remaining length
     */

    bytes[1] = 0x00;

    return 2;
}

uint32_t encodeMqttDisconnect(uint8_t *bytes)
{
    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     */

    bytes[0] = MQTT_CONTROL_PACKET_TYPE_DISCONNECT;

    /**
     * Fixed header - Remaining length
     */

    bytes[1] = 0x00;

    return 2;
}