#include <string.h>
#include <mqtt_packer.h>

uint8_t get_mqtt_remaining_length_size(uint32_t remaining_length)
{
    
    if (remaining_length <= 127)
    {
        return 1;
    }

    if (remaining_length <= 16383)
    {
        return 2;
    }

    if (remaining_length <= 2097151)
    {
        return 3;
    }

    return 4;
}

uint32_t get_mqtt_connect_size(mqtt_connect_packet_t* packet)
{
    // Variable header size: Protocol name + protocol level + connect flags + keep alive
    uint32_t size = 10;

    // Client identifier
    size += packet->client_identifier_size ? 2 + packet->client_identifier_size : 2;

    // Will topic
    size += packet->will_topic_size ? 2 + packet->will_topic_size : 0;

    // Will message
    size += packet->will_message_size ? 2 + packet->will_message_size : 0;

    // Username
    size += packet->username_size ? 2 + packet->username_size : 0;

    // Password
    size += packet->password_size ? 2 + packet->password_size : 0;

    // Fixed header - Remaining length
    size += get_mqtt_remaining_length_size(size);

    // Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
    size += 1;

    return size;
}

uint32_t get_mqtt_publish_size(mqtt_publish_packet_t* packet)
{
    // Topic name
    uint32_t size = 2 + packet->topic_name_size;

    // Packet identifier - The Packet Identifier field is only present in PUBLISH Packets where the QoS level is 1 or 2.
    if (packet->qos == MQTT_QOS_AT_LEAST_ONCE || packet->qos == MQTT_QOS_EXACTLY_ONCE)
    {
        size += 2;
    }

    // Payload
    size += packet->payload_size;

    // Fixed header - Remaining length
    size += get_mqtt_remaining_length_size(size);

    // Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
    size += 1;

    return size;
}

uint32_t get_mqtt_puback_size()
{
    return 4;
}

uint32_t get_mqtt_pubrec_size()
{
    return 4;
}

uint32_t get_mqtt_pubrel_size()
{
    return 4;
}

uint32_t get_mqtt_pubcomp_size()
{
    return 4;
}

uint32_t get_mqtt_subscribe_size(mqtt_un_subscribe_packet_t* packet)
{
    // Variable header
    uint32_t size = 2;

    // Payload - Topic filter + QoS
    size += 2 + packet->topic_filter_size + 1;

    // Fixed header - Remaining length
    size += get_mqtt_remaining_length_size(size);

    // Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
    size += 1;

    return size;
}

uint32_t get_mqtt_unsubscribe_size(mqtt_un_subscribe_packet_t* packet)
{
    // Variable header
    uint32_t size = 2;

    // Payload - Topic filter
    size += 2 + packet->topic_filter_size;

    // Fixed header - Remaining length
    size += get_mqtt_remaining_length_size(size);

    // Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
    size += 1;

    return size;
}

uint32_t get_mqtt_pingreq_size()
{
    return 2;
}

uint32_t get_mqtt_disconnect_size()
{
    return 2;
}

uint8_t pack_mqtt_remaining_length(uint8_t *bytes, uint32_t remaining_length)
{
    uint8_t size = 0;

    // If the remaining length is smaller than 128^1
    if (remaining_length <= 127)
    {
        // Set continuation bit (bit 7) to 0
        bytes[size] = (uint8_t) (remaining_length & 0x7F);
        size++;
    }
    // If the remaining length is larger than 128^1
    else
    {
        // Set continuation bit (bit 7) to 1
        bytes[size] = (uint8_t) ((remaining_length & 0x7F) | 0x80);
        size++;
    } 

    // If the remaining length is smaller than 128^2
    if (remaining_length >= 128 && remaining_length <= 16383)
    {
        // Set continuation bit to 0
        bytes[size] = (uint8_t) ((remaining_length >> 7) & 0x7F);
        size++;
    }
    // If the remaining length is larger than 128^1
    else if (remaining_length >= 128)
    {
        // Set continuation bit to 1
        bytes[size] = (uint8_t) ((((remaining_length >> 7) & 0x7F) | 0x80));
        size++;
    }

    // If the remaining length is smaller than 128^3
    if (remaining_length >= 16384 && remaining_length <= 2097151)
    {
        // Set continuation bit to 0
        bytes[size] = (uint8_t) ((remaining_length >> 14) & 0x7F);
        size++;
    }
    // If the remaining length is larger than 128^2
    else if (remaining_length >= 16384)
    {
        // Set continuation bit to 1
        bytes[size] = (uint8_t) ((((remaining_length >> 14) & 0x7F) | 0x80));
        size++;
    }

    // If the remaining length is larger than 128^3
    if (remaining_length >= 2097152)
    {
        // Set continuation bit to 0
        bytes[size] = (uint8_t) ((remaining_length >> 21) & 0x7F);
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
 * @param packet 
 * @param bytes 
 */
uint32_t pack_mqtt_connect(uint8_t *bytes, mqtt_connect_packet_t* packet)
{
    uint32_t size = 0;

    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     */

    bytes[size] = MQTT_PACKET_CONNECT << 4;
    size++;

    /**
     * Fixed header - Remaining length
     */

    uint32_t remaining_length = 0;

    // Variable header size: Protocol name + protocol level + connect flags + keep alive
    remaining_length += 10;

    // Client identifier
    remaining_length += packet->client_identifier_size ? 2 + packet->client_identifier_size : 2;

    // Will topic
    remaining_length += packet->will_topic_size ? 2 + packet->will_topic_size : 0;

    // Will message
    remaining_length += packet->will_message_size ? 2 + packet->will_message_size : 0;

    // Username
    remaining_length += packet->username_size ? 2 + packet->username_size : 0;

    // Password
    remaining_length += packet->password_size ? 2 + packet->password_size : 0;

    size += pack_mqtt_remaining_length(&(bytes[size]), remaining_length);

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
    uint8_t connect_flags = 0;

    // [MQTT-3.1.3-7] If the Client supplies a zero-byte ClientId, the Client MUST also set CleanSession to 1.
    if (packet->clean_session || packet->client_identifier_size == 0)
    {
        connect_flags |= 0x02;
    }

    if (packet->will_message_size > 0 || packet->will_topic_size > 0)
    {
        connect_flags |= 0x04;
    }

    if (packet->will_qos)
    {
        connect_flags |= (packet->will_qos << 3);
    }

    if (packet->will_retain)
    {
        connect_flags |= 0x20;
    }

    if (packet->username_size > 0)
    {
        connect_flags |= 0x80;
    }

    if (packet->password_size > 0)
    {
        connect_flags |= 0x40;
    }

    bytes[size] = connect_flags;
    size++;

    /**
     * Variable header - Keep alive
     */
    bytes[size] = (uint8_t) (packet->keep_alive >> 8);
    size++;
    bytes[size] = (uint8_t) (packet->keep_alive);
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
    bytes[size] = (uint8_t) (packet->client_identifier_size >> 8);
    size++;

    // String length LSB
    bytes[size] = (uint8_t) packet->client_identifier_size;
    size++;

    if (packet->client_identifier_size > 0)
    {
        // UTF-8 string
        memcpy(&(bytes[size]), packet->client_identifier, packet->client_identifier_size);
        size += packet->client_identifier_size;
    }

    /**
     * Payload - Will topic
     * 
     * If the Will Flag is set to 1, the Will Topic is the next field in the payload.
     * 
     * [MQTT-3.1.3-10] The Will Topic MUST be a UTF-8 encoded string as defined in Section 1.5.3.
     */

    if (packet->will_topic_size > 0)
    {
        // String length MSB + LSB
        bytes[size] = (uint8_t) (packet->will_topic_size >> 8);
        size++;
        bytes[size] = (uint8_t) packet->will_topic_size;
        size++;

        // UTF-8 string
        memcpy(&(bytes[size]), packet->will_topic, packet->will_topic_size);
        size += packet->will_topic_size;
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

    if (packet->will_message_size > 0)
    {
        // String length MSB + LSB
        bytes[size] = (uint8_t) (packet->will_message_size >> 8);
        size++;
        bytes[size] = (uint8_t) packet->will_message_size;
        size++;

        // UTF-8 string
        memcpy(&(bytes[size]), packet->will_message, packet->will_message_size);
        size += packet->will_message_size;
    }

    /**
     * Payload - Username
     * 
     * If the User Name Flag is set to 1, this is the next field in the payload.
     * It can be used by the Server for authentication and authorization.
     * 
     * [MQTT-3.1.3-11] The User Name MUST be a UTF-8 encoded string. 
     */

    if (packet->username_size > 0)
    {
        // String length MSB + LSB
        bytes[size] = (uint8_t) (packet->username_size >> 8);
        size++;
        bytes[size] = (uint8_t) packet->username_size;
        size++;

        // UTF-8 string
        memcpy(&(bytes[size]), packet->username, packet->username_size);
        size += packet->username_size;
    }

    /**
     * Payload - Password
     * 
     * If the Password Flag is set to 1, this is the next field in the payload. 
     * The Password field contains 0 to 65535 bytes of binary data prefixed with a two byte length field which indicates the number of bytes used by the binary data (it does not include the two bytes taken up by the length field itself).
     */

    if (packet->password_size > 0)
    {
        // String length MSB + LSB
        bytes[size] = (uint8_t) (packet->password_size >> 8);
        size++;
        bytes[size] = (uint8_t) packet->password_size;
        size++;

        // UTF-8 string
        memcpy(&(bytes[size]), packet->password, packet->password_size);
        size += packet->password_size;
    }

    return size;
}

uint32_t pack_mqtt_publish(uint8_t *bytes, mqtt_publish_packet_t* packet)
{
    uint32_t size = 0;

    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     * 
     * [MQTT-3.8.1-1] Bits 3,2,1 and 0 of the fixed header of the SUBSCRIBE Control Packet are reserved and MUST be set to 0,0,1 and 0 respectively. The Server MUST treat any other value as malformed and close the Network Connection.
     */

    bytes[0] = (MQTT_PACKET_PUBLISH << 4) | (packet->qos << 1);

    if (packet->dup)
    {
        bytes[0] |= 0x08;
    }

    if (packet->retain)
    {
        bytes[0] |= 0x01;
    }

    size++;

    /**
     * Fixed header - Remaining length
     */

    uint32_t remaining_length = 0;

    // Topic name
    remaining_length += 2 + packet->topic_name_size;

    // Packet identifier - The Packet Identifier field is only present in PUBLISH Packets where the QoS level is 1 or 2.
    if (packet->qos == MQTT_QOS_AT_LEAST_ONCE || packet->qos == MQTT_QOS_EXACTLY_ONCE)
    {
        remaining_length += 2;
    }

    // Payload
    remaining_length += packet->payload_size;

    size += pack_mqtt_remaining_length(&(bytes[size]), remaining_length);

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
    bytes[size] = (uint8_t) (packet->topic_name_size >> 8);
    size++;
    bytes[size] = (uint8_t) packet->topic_name_size;
    size++;
    
    // Topic name
    memcpy(&(bytes[size]), packet->topic_name, packet->topic_name_size);
    size += packet->topic_name_size;

    /**
     * Variable header - Packet Identifier
     * 
     * [MQTT-3.3.2.2] The Packet Identifier field is only present in PUBLISH Packets where the QoS level is 1 or 2. Section 2.3.1 provides more information about Packet Identifiers.
     */
    if (packet->qos != 0) {
        bytes[size] = (uint8_t) (packet->packet_identifier >> 8);
        size++;
        bytes[size] = (uint8_t) packet->packet_identifier;
        size++;
    }

    /**
     * Payload
     * 
     * The Payload contains the Application Message that is being published.
     * The content and format of the data is application specific.
     * The length of the payload can be calculated by subtracting the length of the variable header from the Remaining Length field that is in the Fixed Header.
     * It is valid for a PUBLISH Packet to contain a zero length payload.
     */

    memcpy(&(bytes[size]), packet->payload, packet->payload_size);
    size += packet->payload_size;

    return size;
}

uint32_t pack_mqtt_puback(uint8_t *bytes, uint16_t packet_identifier)
{
    uint32_t size = 0;

    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     */

    bytes[0] = MQTT_PACKET_PUBACK << 4;
    size++;

    /**
     * Fixed header - Remaining length
     */

    // Variable header
    uint32_t remaining_length = 2;
    size += pack_mqtt_remaining_length(&(bytes[size]), remaining_length);

    /**
     * Variable header - Packet Identifier
     * 
     * This contains the Packet Identifier from the PUBLISH Packet that is being acknowledged.
     */
    bytes[size] = (uint8_t) (packet_identifier >> 8);
    size++;
    bytes[size] = (uint8_t) packet_identifier;
    size++;

    return size;
}

uint32_t pack_mqtt_pubrec(uint8_t *bytes, uint16_t packet_identifier)
{
    uint32_t size = 0;

    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     */

    bytes[0] = MQTT_PACKET_PUBREC << 4;
    size++;

    /**
     * Fixed header - Remaining length
     */

    // Variable header
    uint32_t remaining_length = 2;
    size += pack_mqtt_remaining_length(&(bytes[size]), remaining_length);

    /**
     * Variable header - Packet Identifier
     * 
     * This contains the Packet Identifier from the PUBLISH Packet that is being acknowledged.
     */
    bytes[size] = (uint8_t) (packet_identifier >> 8);
    size++;
    bytes[size] = (uint8_t) packet_identifier;
    size++;

    return size;
}

uint32_t pack_mqtt_pubrel(uint8_t *bytes, uint16_t packet_identifier)
{
    uint32_t size = 0;

    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     */

    bytes[0] = (MQTT_PACKET_PUBREL << 4) | 0x02;
    size++;

    /**
     * Fixed header - Remaining length
     */

    // Variable header
    uint32_t remaining_length = 2;
    size += pack_mqtt_remaining_length(&(bytes[size]), remaining_length);

    /**
     * Variable header - Packet Identifier
     * 
     * The variable header contains the same Packet Identifier as the PUBREC Packet that is being acknowledged.
     */
    bytes[size] = (uint8_t) (packet_identifier >> 8);
    size++;
    bytes[size] = (uint8_t) packet_identifier;
    size++;

    return size;
}

uint32_t pack_mqtt_pubcomp(uint8_t *bytes, uint16_t packet_identifier)
{
    uint32_t size = 0;

    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     */

    bytes[0] = MQTT_PACKET_PUBCOMP << 4;
    size++;

    /**
     * Fixed header - Remaining length
     */

    // Variable header
    uint32_t remaining_length = 2;
    size += pack_mqtt_remaining_length(&(bytes[size]), remaining_length);

    /**
     * Variable header - Packet Identifier
     * 
     * The variable header contains the same Packet Identifier as the PUBREL Packet that is being acknowledged.
     */
    bytes[size] = (uint8_t) (packet_identifier >> 8);
    size++;
    bytes[size] = (uint8_t) packet_identifier;
    size++;

    return size;
}


uint32_t pack_mqtt_subscribe(uint8_t *bytes, mqtt_un_subscribe_packet_t* packet)
{
    uint32_t size = 0;

    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     * 
     * [MQTT-3.8.1-1] Bits 3,2,1 and 0 of the fixed header of the SUBSCRIBE Control Packet are reserved and MUST be set to 0,0,1 and 0 respectively. The Server MUST treat any other value as malformed and close the Network Connection.
     */

    bytes[0] = (MQTT_PACKET_SUBSCRIBE << 4) | 0x02;
    size++;

    /**
     * Fixed header - Remaining length
     */

    // Variable header length
    uint32_t remaining_length = 2;

    // Topic filter + QoS length
    remaining_length += 2 + packet->topic_filter_size + 1;

    size += pack_mqtt_remaining_length(&(bytes[size]), remaining_length);

    /**
     * Variable header - Packet Identifier
     */
    bytes[size] = (uint8_t) (packet->packet_identifier >> 8);
    size++;
    bytes[size] = (uint8_t) packet->packet_identifier;
    size++;

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
    bytes[size] = (uint8_t) (packet->topic_filter_size >> 8);
    size++;
    bytes[size] = (uint8_t) packet->topic_filter_size;
    size++;
    
    // Topic filter
    memcpy(&(bytes[size]), packet->topic_filter, packet->topic_filter_size);
    size += packet->topic_filter_size;

    // QoS
    bytes[size] = packet->qos;
    size++;

    return size;
}

uint32_t pack_mqtt_unsubscribe(uint8_t *bytes, mqtt_un_subscribe_packet_t* packet)
{
    uint32_t size = 0;

    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     */

    bytes[0] = (MQTT_PACKET_UNSUBSCRIBE << 4) | 0x02;
    size++;

    /**
     * Fixed header - Remaining length
     */

    // Variable header length
    uint32_t remaining_length = 2;

    // Topic filter
    remaining_length += 2 + packet->topic_filter_size;

    size += pack_mqtt_remaining_length(&(bytes[size]), remaining_length);

    /**
     * Variable header - Packet Identifier
     */
    bytes[size] = (uint8_t) (packet->packet_identifier >> 8);
    size++;
    bytes[size] = (uint8_t) packet->packet_identifier;
    size++;

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
    bytes[size] = (uint8_t) (packet->topic_filter_size >> 8);
    size++;
    bytes[size] = (uint8_t) packet->topic_filter_size;
    size++;
    
    // Topic filter
    memcpy(&(bytes[size]), packet->topic_filter, packet->topic_filter_size);
    size += packet->topic_filter_size;

    return size;
}

uint32_t pack_mqtt_pingreq(uint8_t *bytes)
{
    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     */

    bytes[0] = MQTT_PACKET_PINGREQ << 4;

    /**
     * Fixed header - Remaining length
     */

    bytes[1] = 0x00;

    return 2;
}

uint32_t pack_mqtt_disconnect(uint8_t *bytes)
{
    /**
     * Fixed header - MQTT Control Packet type + Flags specific to each MQTT Control Packet type
     */

    bytes[0] = MQTT_PACKET_DISCONNECT << 4;

    /**
     * Fixed header - Remaining length
     */

    bytes[1] = 0x00;

    return 2;
}