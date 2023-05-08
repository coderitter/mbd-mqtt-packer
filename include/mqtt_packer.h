#ifndef MQTT_PACKER_H_
#define MQTT_PACKER_H_

#include <stdint.h>

/**
 * @brief After a Network Connection is established by a Client to a Server, the first Packet sent from the Client to the Server MUST be a CONNECT Packet.
 */
#define MQTT_PACKET_CONNECT 0x01

/**
 * @brief The CONNACK Packet is the packet sent by the Server in response to a CONNECT Packet received from a Client.
 */
#define MQTT_PACKET_CONNACK 0x02

/**
 * @brief A PUBLISH Control Packet is sent from a Client to a Server or from Server to a Client to transport an Application Message.
 */
#define MQTT_PACKET_PUBLISH 0x03

/**
 * @brief A PUBACK Packet is the response to a PUBLISH Packet with QoS level 1.
 */
#define MQTT_PACKET_PUBACK 0x04

/**
 * @brief A PUBREC Packet is the response to a PUBLISH Packet with QoS 2. It is the second packet of the QoS 2 protocol exchange.
 */
#define MQTT_PACKET_PUBREC 0x05

/**
 * @brief A PUBREL Packet is the response to a PUBREC Packet. It is the third packet of the QoS 2 protocol exchange.
 */
#define MQTT_PACKET_PUBREL 0x06

/**
 * @brief The PUBCOMP Packet is the response to a PUBREL Packet. It is the fourth and final packet of the QoS 2 protocol exchange.
 */
#define MQTT_PACKET_PUBCOMP 0x07

/**
 * @brief The SUBSCRIBE Packet is sent from the Client to the Server to create one or more Subscriptions. Each Subscription registers a Client’s interest in one or more Topics. The Server sends PUBLISH Packets to the Client in order to forward Application Messages that were published to Topics that match these Subscriptions. The SUBSCRIBE Packet also specifies (for each Subscription) the maximum QoS with which the Server can send Application Messages to the Client.
 */
#define MQTT_PACKET_SUBSCRIBE 0x08

/**
 * @brief A SUBACK Packet is sent by the Server to the Client to confirm receipt and processing of a SUBSCRIBE Packet.
 */
#define MQTT_PACKET_SUBACK 0x09

/**
 * @brief The payload for the UNSUBSCRIBE Packet contains the list of Topic Filters that the Client wishes to unsubscribe from.
 */
#define MQTT_PACKET_UNSUBSCRIBE 0x0A

/**
 * @brief The UNSUBACK Packet is sent by the Server to the Client to confirm receipt of an UNSUBSCRIBE Packet.
 */
#define MQTT_PACKET_UNSUBACK 0x0B

/**
 * @brief The PINGREQ Packet is sent from a Client to the Server.
 */
#define MQTT_PACKET_PINGREQ 0x0C

/**
 * @brief A PINGRESP Packet is sent by the Server to the Client in response to a PINGREQ Packet. It indicates that the Server is alive.
 */
#define MQTT_PACKET_PINGRESP 0x0D

/**
 * @brief The DISCONNECT Packet is the final Control Packet sent from the Client to the Server. It indicates that the Client is disconnecting cleanly.
 */
#define MQTT_PACKET_DISCONNECT 0x0E

/**
 * @brief Connection accepted
 */
#define MQTT_CONNACK_RETURN_CODE_ACCEPTED 0x00

/**
 * @brief The Server does not support the level of the MQTT protocol requested by the Client
 */
#define MQTT_CONNACK_RETURN_CODE_UNACCEPTABLE_PROTOCOL_VERSION 0x01

/**
 * @brief The Client identifier is correct UTF-8 but not allowed by the Server
 */
#define MQTT_CONNACK_RETURN_CODE_IDENTIFIER_REJECTED 0x02

/**
 * @brief The Network Connection has been made but the MQTT service is unavailable
 */
#define MQTT_CONNACK_RETURN_CODE_SERVER_UNAVAILABLE 0x03

/**
 * @brief The data in the user name or password is malformed
 */
#define MQTT_CONNACK_RETURN_CODE_BAD_USER_NAME_OR_PASSWORD 0x04

/**
 * @brief The Client is not authorized to connect
 */
#define MQTT_CONNACK_RETURN_CODE_BAD_NOT_AUTHORIZED 0x05

#define MQTT_QOS_AT_MOST_ONCE 0x00
#define MQTT_QOS_AT_LEAST_ONCE 0x01
#define MQTT_QOS_EXACTLY_ONCE 0x02
#define MQTT_QOS_RESERVED 0x03

/**
 * @brief Contains the bytes, sizes and pointers regarding an MQTT packet.
 */
typedef struct mqtt_packet
{
    /**
     * @brief The size of the MQTT packet which is the sum of fixed_header_size and remaining_size
     */
    uint32_t size;

    /**
     * @brief The size of the fixed header containing the Control Packet type and the remaining length. Can be 2 to 5 bytes.
     */
    uint16_t fixed_header_size;

    /**
     * Use one the following defines to determine the Control Packet type.
     * 
     *  - MQTT_PACKET_CONNECT
     *  - MQTT_PACKET_CONNACK
     *  - MQTT_PACKET_PUBLISH
     *  - MQTT_PACKET_PUBACK
     *  - MQTT_PACKET_PUBREC
     *  - MQTT_PACKET_PUBREL
     *  - MQTT_PACKET_PUBCOMP
     *  - MQTT_PACKET_SUBSCRIBE
     *  - MQTT_PACKET_SUBACK
     *  - MQTT_PACKET_UNSUBSCRIBE
     *  - MQTT_PACKET_UNSUBACK
     *  - MQTT_PACKET_PINGREQ
     *  - MQTT_PACKET_PINGRESP
     *  - MQTT_PACKET_DISCONNECT
     */
    uint8_t type;
    
    /**
     * @brief The remaining size which contains the size of the variable header and the payload.
     */
    uint32_t remaining_size;
} mqtt_packet_t;

typedef struct mqtt_connect_packet
{
    /**
     * @brief The Client Identifier (ClientId) identifies the Client to the Server.
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
    const uint8_t* client_identifier;

    /**
     * @brief The size of the Client Identifier
     */
    uint16_t client_identifier_size;

    /**
     * @brief The Client and Server can store Session state to enable reliable messaging to continue across a sequence of Network Connections. This bit is used to control the lifetime of the Session state.
     * 
     * The Session state in the Client consists of:
     *  - QoS 1 and QoS 2 messages which have been sent to the Server, but have not been completely acknowledged.
     *  - QoS 2 messages which have been received from the Server, but have not been completely acknowledged.
     * 
     * When CleanSession is set to 1 the Client and Server need not process the deletion of state atomically.
     * 
     * Non normative comment
     * To ensure consistent state in the event of a failure, the Client should repeat its attempts to connect with CleanSession set to 1, until it connects successfully.
     * 
     * Non normative comment
     * Typically, a Client will always connect using CleanSession set to 0 or CleanSession set to 1 and not swap between the two values. The choice will depend on the application. A Client using CleanSession set to 1 will not receive old Application Messages and has to subscribe afresh to any topics that it is interested in each time it connects. A Client using CleanSession set to 0 will receive all QoS 1 or QoS 2 messages that were published while it was disconnected. Hence, to ensure that you do not lose messages while disconnected, use QoS 1 or QoS 2 with CleanSession set to 0.
     * 
     * Non normative comment
     * When a Client connects with CleanSession set to 0, it is requesting that the Server maintain its MQTT session state after it disconnects. Clients should only connect with CleanSession set to 0, if they intend to reconnect to the Server at some later point in time. When a Client has determined that it has no further use for the session it should do a final connect with CleanSession set to 1 and then disconnect.
     * 
     * [MQTT-3.1.2-4] If CleanSession is set to 0, the Server MUST resume communications with the Client based on state from the current Session (as identified by the Client identifier). If there is no Session associated with the Client identifier the Server MUST create a new Session. The Client and Server MUST store the Session after the Client and Server are disconnected.
     * [MQTT-3.1.2-5] After the disconnection of a Session that had CleanSession set to 0, the Server MUST store further QoS 1 and QoS 2 messages that match any subscriptions that the client had at the time of disconnection as part of the Session state. It MAY also store QoS 0 messages that meet the same criteria.
     * [MQTT-3.1.2-6] If CleanSession is set to 1, the Client and Server MUST discard any previous Session and start a new one. This Session lasts as long as the Network Connection. State data associated with this Session MUST NOT be reused in any subsequent Session.
     * [MQTT-3.1.2.7] Retained messages do not form part of the Session state in the Server, they MUST NOT be deleted when the Session ends.
     */
    uint8_t clean_session;

    /**
     * [MQTT-3.1.2-8] If the Will Flag is set to 1 this indicates that, if the Connect request is accepted, a Will Message MUST be stored on the Server and associated with the Network Connection. The Will Message MUST be published when the Network Connection is subsequently closed unless the Will Message has been deleted by the Server on receipt of a DISCONNECT Packet.
     * [MQTT-3.1.3-10] The Will Topic MUST be a UTF-8 encoded string.
     */
    const uint8_t* will_topic;
    uint16_t will_topic_size;

    /**
     * @brief The Will Message defines the Application Message that is to be published to the Will Topic.
     * 
     * Situations in which the Will Message is published include, but are not limited to:
     *  - An I/O error or network failure detected by the Server.
     *  - The Client fails to communicate within the Keep Alive time.
     *  - The Client closes the Network Connection without first sending a DISCONNECT Packet.
     *  - The Server closes the Network Connection because of a protocol error.
     * 
     * The Server SHOULD publish Will Messages promptly. In the case of a Server shutdown or failure the server MAY defer publication of Will Messages until a subsequent restart. If this happens there might be a delay between the time the server experienced failure and a Will Message being published.
     * 
     * [MQTT-3.1.2-8] If the Will Flag is set to 1 this indicates that, if the Connect request is accepted, a Will Message MUST be stored on the Server and associated with the Network Connection. The Will Message MUST be published when the Network Connection is subsequently closed unless the Will Message has been deleted by the Server on receipt of a DISCONNECT Packet.
     * [MQTT-3.1.2-9] If the Will Flag is set to 1, the Will QoS and Will Retain fields in the Connect Flags will be used by the Server, and the Will Topic and Will Message fields MUST be present in the payload.
     * [MQTT-3.1.2-10] The Will Message MUST be removed from the stored Session state in the Server once it has been published or the Server has received a DISCONNECT packet from the Client.
     * [MQTT-3.1.2-11] If the Will Flag is set to 0 the Will QoS and Will Retain fields in the Connect Flags MUST be set to zero and the Will Topic and Will Message fields MUST NOT be present in the payload.
     * [MQTT-3.1.2-12] If the Will Flag is set to 0, a Will Message MUST NOT be published when this Network Connection ends.
     */
    const uint8_t* will_message;
    uint8_t will_message_size;

    /**
     * @brief Specify the QoS level to be used when publishing the Will Message.
     * 
     * Use one of the defines
     *  - MQTT_QOS_AT_MOST_ONCE
     *  - MQTT_QOS_AT_LEAST_ONCE
     *  - MQTT_QOS_EXACTLY_ONCE
     * 
     * [MQTT-3.1.2-13] If the Will Flag is set to 0, then the Will QoS MUST be set to 0 (0x00).
     * [MQTT-3.1.2-14] If the Will Flag is set to 1, the value of Will QoS can be 0 (0x00), 1 (0x01), or 2 (0x02). It MUST NOT be 3 (0x03).
     */
    uint8_t will_qos;

    /**
     * @brief Specifies if the Will Message is to be Retained when it is published.
     * 
     * [MQTT-3.1.2-15] If the Will Flag is set to 0, then the Will Retain Flag MUST be set to 0.
     * [MQTT-3.1.2-16] If the Will Flag is set to 1 and if Will Retain is set to 0, the Server MUST publish the Will Message as a non-retained message.
     * [MQTT-3.1.2-17] If the Will Flag is set to 1 and if Will Retain is set to 1, the Server MUST publish the Will Message as a retained message.
     */
    uint8_t will_retain;

    /**
     * @brief 
     * 
     * [MQTT-3.1.2-18] If the User Name Flag is set to 0, a user name MUST NOT be present in the payload.
     * [MQTT-3.1.2-19] If the User Name Flag is set to 1, a user name MUST be present in the payload.
     */
    const uint8_t* username;
    uint16_t username_size;

    /**
     * @brief 
     * 
     * [MQTT-3.1.2-20] If the Password Flag is set to 0, a password MUST NOT be present in the payload.
     * [MQTT-3.1.2-21] If the Password Flag is set to 1, a password MUST be present in the payload.
     * [MQTT-3.1.2-22] If the User Name Flag is set to 0, the Password Flag MUST be set to 0.
     */
    const uint8_t* password;
    uint16_t password_size;

    /**
     * @brief The Keep Alive is a time interval measured in seconds. Expressed as a 16-bit word, it is the maximum time interval that is permitted to elapse between the point at which the Client finishes transmitting one Control Packet and the point it starts sending the next.
     * 
     * The Client can send PINGREQ at any time, irrespective of the Keep Alive value, and use the PINGRESP to determine that the network and the Server are working.
     * If a Client does not receive a PINGRESP Packet within a reasonable amount of time after it has sent a PINGREQ, it SHOULD close the Network Connection to the Server.
     * A Keep Alive value of zero (0) has the effect of turning off the keep alive mechanism. This means that, in this case, the Server is not required to disconnect the Client on the grounds of inactivity. Note that a Server is permitted to disconnect a Client that it determines to be inactive or non-responsive at any time, regardless of the Keep Alive value provided by that Client.
     * 
     * [MQTT-3.1.2-23] It is the responsibility of the Client to ensure that the interval between Control Packets being sent does not exceed the Keep Alive value. In the absence of sending any other Control Packets, the Client MUST send a PINGREQ Packet.
     * [MQTT-3.1.2-24] If the Keep Alive value is non-zero and the Server does not receive a Control Packet from the Client within one and a half times the Keep Alive time period, it MUST disconnect the Network Connection to the Client as if the network had failed.
     */
    uint16_t keep_alive;
} mqtt_connect_packet_t;

typedef struct mqtt_connack_packet
{
    /**
     * @brief The Session Present flag enables a Client to establish whether the Client and Server have a consistent view about whether there is already stored Session state. 
     * 
     * Once the initial setup of a Session is complete, a Client with stored Session state will expect the Server to maintain its stored Session state.
     * In the event that the value of Session Present received by the Client from the Server is not as expected, the Client can choose whether to proceed with the Session or to disconnect.
     * The Client can discard the Session state on both Client and Server by disconnecting, connecting with Clean Session set to 1 and then disconnecting again. 
     * 
     * [MQTT-3.2.2-1] If the Server accepts a connection with CleanSession set to 1, the Server MUST set Session Present to 0 in the CONNACK packet in addition to setting a zero return code in the CONNACK packet.
     * [MQTT-3.2.2-2] If the Server accepts a connection with CleanSession set to 0, the value set in Session Present depends on whether the Server already has stored Session state for the supplied client ID. If the Server has stored Session state, it MUST set Session Present to 1 in the CONNACK packet.
     * [MQTT-3.2.2-3] If the Server does not have stored Session state, it MUST set Session Present to 0 in the CONNACK packet. This is in addition to setting a zero return code in the CONNACK packet.
     * [MQTT-3.2.2-4] If a server sends a CONNACK packet containing a non-zero return code it MUST set Session Present to 0.
     */
    uint8_t session_present;

    /**
     * @brief If a well formed CONNECT Packet is received by the Server, but the Server is unable to process it for some reason, then the Server SHOULD attempt to send a CONNACK packet containing the appropriate non-zero Connect return code.
     * 
     * - MQTT_CONNACK_RETURN_CODE_ACCEPTED
     * - MQTT_CONNACK_RETURN_CODE_UNACCEPTABLE_PROTOCOL_VERSION
     * - MQTT_CONNACK_RETURN_CODE_IDENTIFIER_REJECTED
     * - MQTT_CONNACK_RETURN_CODE_SERVER_UNAVAILABLE
     * - MQTT_CONNACK_RETURN_CODE_BAD_USER_NAME_OR_PASSWORD
     * - MQTT_CONNACK_RETURN_CODE_BAD_NOT_AUTHORIZED
     * 
     * [MQTT-3.2.2-5] If a server sends a CONNACK packet containing a non-zero return code it MUST then close the Network Connection.
     * [MQTT-3.2.2-6] If none of the defined return code values are deemed applicable, then the Server MUST close the Network Connection without sending a CONNACK.
     */
    uint8_t return_code;
} mqtt_connack_packet_t;

/**
 * @brief The parameter for the Control Packet type PUBLISH.
 */
typedef struct mqtt_publish_packet
{
    /**
     * @brief The variable header component of many of the Control Packet types includes a 2 byte Packet Identifier field. These Control Packets are PUBLISH (where QoS > 0), PUBACK, PUBREC, PUBREL, PUBCOMP, SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK.
     * 
     * The Packet Identifier field is only present in PUBLISH Packets where the QoS level is 1 or 2. Section 2.3.1 provides more information about Packet Identifiers.
     * 
     * [MQTT-2.3.1-1] SUBSCRIBE, UNSUBSCRIBE, and PUBLISH (in cases where QoS > 0) Control Packets MUST contain a non-zero 16-bit Packet Identifier.
     * [MQTT-2.3.1-2] Each time a Client sends a new packet of one of these types it MUST assign it a currently unused Packet Identifier. 
     * [MQTT-2.3.1-3] If a Client re-sends a particular Control Packet, then it MUST use the same Packet Identifier in subsequent re-sends of that packet. The Packet Identifier becomes available for reuse after the Client has processed the corresponding acknowledgement packet. In the case of a QoS 1 PUBLISH this is the corresponding PUBACK; in the case of QoS 2 it is PUBCOMP. For SUBSCRIBE or UNSUBSCRIBE it is the corresponding SUBACK or UNSUBACK.
     * [MQTT-2.3.1-4] The same conditions apply to a Server when it sends a PUBLISH with QoS > 0.
     * [MQTT-2.3.1-5] A PUBLISH Packet MUST NOT contain a Packet Identifier if its QoS value is set to 0.
     * [MQTT-2.3.1-7] A PUBACK, PUBREC or PUBREL Packet MUST contain the same Packet Identifier as the PUBLISH Packet that was originally sent [MQTT-2.3.1-6]. Similarly SUBACK and UNSUBACK MUST contain the Packet Identifier that was used in the corresponding SUBSCRIBE and UNSUBSCRIBE Packet respectively.
     * [MQTT-3.8.4-2] The SUBACK Packet MUST have the same Packet Identifier as the SUBSCRIBE Packet that it is acknowledging.
     */
    uint16_t packet_identifier;

    /**
     * @brief If the DUP flag is set to 0, it indicates that this is the first occasion that the Client or Server has attempted to send this MQTT PUBLISH Packet. If the DUP flag is set to 1, it indicates that this might be re-delivery of an earlier attempt to send the Packet.
     * 
     * [MQTT-3.3.1-1] The DUP flag MUST be set to 1 by the Client or Server when it attempts to re-deliver a PUBLISH Packet.
     * [MQTT-3.3.1-2] The DUP flag MUST be set to 0 for all QoS 0 messages.
     * [MQTT-3.3.1-3] The value of the DUP flag from an incoming PUBLISH packet is not propagated when the PUBLISH Packet is sent to subscribers by the Server. The DUP flag in the outgoing PUBLISH packet is set independently to the incoming PUBLISH packet, its value MUST be determined solely by whether the outgoing PUBLISH packet is a retransmission.
     */
    uint8_t dup;

    /**
     * @brief This field indicates the level of assurance for delivery of an Application Message.
     * 
     * Use one of the defines
     *  - MQTT_QOS_AT_MOST_ONCE
     *  - MQTT_QOS_AT_LEAST_ONCE
     *  - MQTT_QOS_EXACTLY_ONCE
     * 
     * [MQTT-3.3.1-4] A PUBLISH Packet MUST NOT have both QoS bits set to 1. If a Server or Client receives a PUBLISH Packet which has both QoS bits set to 1 it MUST close the Network Connection.
     */
    uint8_t qos;

    /**
     * @brief Contains the length of variable header, is only used in unpack_mqtt_publish 
    */
    uint16_t variable_header_size;

    /**
     * @brief 
     * 
     * [MQTT-3.3.1-5] If the RETAIN flag is set to 1, in a PUBLISH Packet sent by a Client to a Server, the Server MUST store the Application Message and its QoS, so that it can be delivered to future subscribers whose subscriptions match its topic name.
     * [MQTT-3.3.1-6] When a new subscription is established, the last retained message, if any, on each matching topic name MUST be sent to the subscriber.
     * [MQTT-3.3.1-7] If the Server receives a QoS 0 message with the RETAIN flag set to 1 it MUST discard any message previously retained for that topic. It SHOULD store the new QoS 0 message as the new retained message for that topic, but MAY choose to discard it at any time - if this happens there will be no retained message for that topic.
     * [MQTT-3.3.1-8] When sending a PUBLISH Packet to a Client the Server MUST set the RETAIN flag to 1 if a message is sent as a result of a new subscription being made by a Client.
     * [MQTT-3.3.1-9] It MUST set the RETAIN flag to 0 when a PUBLISH Packet is sent to a Client because it matches an established subscription regardless of how the flag was set in the message it received.
     * [MQTT-3.3.1-10] A PUBLISH Packet with a RETAIN flag set to 1 and a payload containing zero bytes will be processed as normal by the Server and sent to Clients with a subscription matching the topic name. Additionally any existing retained message with the same topic name MUST be removed and any future subscribers for the topic will not receive a retained message. “As normal” means that the RETAIN flag is not set in the message received by existing Clients.
     * [MQTT-3.3.1-11] A zero byte retained message MUST NOT be stored as a retained message on the Server.
     * [MQTT-3.3.1-12] If the RETAIN flag is 0, in a PUBLISH Packet sent by a Client to a Server, the Server MUST NOT store the message and MUST NOT remove or replace any existing retained message.
     */
    uint8_t retain;

    /**
     * @brief The Topic Name identifies the information channel to which payload data is published.
     * 
     * Since the Server is permitted to override the Topic Name, it might not be the same as the Topic Name in the original PUBLISH Packet.
     * 
     * [MQTT-3.3.2-1] The Topic Name MUST be present as the first field in the PUBLISH Packet Variable header. It MUST be a UTF-8 encoded string.
     * [MQTT-3.3.2-2] The Topic Name in the PUBLISH Packet MUST NOT contain wildcard characters.
     * [MQTT-3.3.2-3] The Topic Name in a PUBLISH Packet sent by a Server to a subscribing Client MUST match the Subscription’s Topic Filter according to the matching process.
     */
    const uint8_t* topic_name;
    uint16_t topic_name_size;

    /**
     * @brief The Payload contains the Application Message that is being published.
     * 
     * The content and format of the data is application specific.
     * The length of the payload can be calculated by subtracting the length of the variable header from the Remaining Length field that is in the Fixed Header.
     * It is valid for a PUBLISH Packet to contain a zero length payload.
     */
    const uint8_t* payload;
    uint32_t payload_size;
} mqtt_publish_packet_t;

/**
 * @brief The parameter for the Control Packet types SUBSCRIBE and UNSUBSCRIBE.
 */
typedef struct mqtt_un_subscribe_packet
{
    /**
     * @brief The variable header component of many of the Control Packet types includes a 2 byte Packet Identifier field. These Control Packets are PUBLISH (where QoS > 0), PUBACK, PUBREC, PUBREL, PUBCOMP, SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK.
     * 
     * [MQTT-2.3.1-1] SUBSCRIBE, UNSUBSCRIBE, and PUBLISH (in cases where QoS > 0) Control Packets MUST contain a non-zero 16-bit Packet Identifier.
     * [MQTT-2.3.1-2] Each time a Client sends a new packet of one of these types it MUST assign it a currently unused Packet Identifier. 
     * [MQTT-2.3.1-3] If a Client re-sends a particular Control Packet, then it MUST use the same Packet Identifier in subsequent re-sends of that packet. The Packet Identifier becomes available for reuse after the Client has processed the corresponding acknowledgement packet. In the case of a QoS 1 PUBLISH this is the corresponding PUBACK; in the case of QoS 2 it is PUBCOMP. For SUBSCRIBE or UNSUBSCRIBE it is the corresponding SUBACK or UNSUBACK.
     * [MQTT-2.3.1-4] The same conditions apply to a Server when it sends a PUBLISH with QoS > 0.
     * [MQTT-2.3.1-5] A PUBLISH Packet MUST NOT contain a Packet Identifier if its QoS value is set to 0.
     * [MQTT-2.3.1-7] A PUBACK, PUBREC or PUBREL Packet MUST contain the same Packet Identifier as the PUBLISH Packet that was originally sent [MQTT-2.3.1-6]. Similarly SUBACK and UNSUBACK MUST contain the Packet Identifier that was used in the corresponding SUBSCRIBE and UNSUBSCRIBE Packet respectively.
     * [MQTT-3.8.4-2] The SUBACK Packet MUST have the same Packet Identifier as the SUBSCRIBE Packet that it is acknowledging.
     */
    uint16_t packet_identifier;

    /**
     * @brief The payload of a SUBSCRIBE Packet contains a list of Topic Filters indicating the Topics to which the Client wants to subscribe.
     * 
     * [MQTT-3.8.3-1] The Topic Filters in a SUBSCRIBE packet payload MUST be UTF-8 encoded strings as defined in Section 1.5.3.
     * [MQTT-3.8.3-2] A Server SHOULD support Topic filters that contain the wildcard characters. If it chooses not to support topic filters that contain wildcard characters it MUST reject any Subscription request whose filter contains them.
     * [MQTT-3.8.3-3] The payload of a SUBSCRIBE packet MUST contain at least one Topic Filter / QoS pair. A SUBSCRIBE packet with no payload is a protocol violation.
     * [MQTT-3-8.3-4] The Server MUST treat a SUBSCRIBE packet as malformed and close the Network Connection if any of Reserved bits in the payload are non-zero, or QoS is not 0,1 or 2.
     * [MQTT-3.8.4-3] If a Server receives a SUBSCRIBE Packet containing a Topic Filter that is identical to an existing Subscription’s Topic Filter then it MUST completely replace that existing Subscription with a new Subscription. The Topic Filter in the new Subscription will be identical to that in the previous Subscription, although its maximum QoS value could be different. Any existing retained messages matching the Topic Filter MUST be re-sent, but the flow of publications MUST NOT be interrupted.
     */
    const uint8_t* topic_filter;
    uint16_t topic_filter_size;

    /**
     * @brief 
     * 
     * Use one of the defines
     *  - MQTT_QOS_AT_MOST_ONCE
     *  - MQTT_QOS_AT_LEAST_ONCE
     *  - MQTT_QOS_EXACTLY_ONCE
     * 
     * [MQTT-3.8.4-5] The SUBACK Packet sent by the Server to the Client MUST contain a return code for each Topic Filter/QoS pair. This return code MUST either show the maximum QoS that was granted for that Subscription or indicate that the subscription failed.
     * [MQTT-3.8.4-6] The Server might grant a lower maximum QoS than the subscriber requested. The QoS of Payload Messages sent in response to a Subscription MUST be the minimum of the QoS of the originally published message and the maximum QoS granted by the Server. The server is permitted to send duplicate copies of a message to a subscriber in the case where the original message was published with QoS 1 and the maximum QoS granted was QoS 0.
     */
    uint8_t qos;
} mqtt_un_subscribe_packet_t;

typedef struct mqtt_suback_packet
{
    /**
     * @brief The variable header contains the Packet Identifier from the SUBSCRIBE Packet that is being acknowledged.
     * 
     * [MQTT-3.8.4-2] The SUBACK Packet MUST have the same Packet Identifier as the SUBSCRIBE Packet that it is acknowledging.
     */
    uint16_t packet_identifier;

    /**
     * @brief The payload contains a list of return codes. Each return code corresponds to a Topic Filter in the SUBSCRIBE Packet being acknowledged.
     * 
     * [MQTT-3.9.3-1] The order of return codes in the SUBACK Packet MUST match the order of Topic Filters in the SUBSCRIBE Packet.
     * [MQTT-3.9.3-2] SUBACK return codes other than 0x00, 0x01, 0x02 and 0x80 are reserved and MUST NOT be used.
     */
    uint8_t return_code;
} mqtt_suback_packet_t;

uint8_t get_mqtt_remaining_length_size(uint32_t remaining_length);
uint32_t get_mqtt_connect_size(mqtt_connect_packet_t* packet);
uint32_t get_mqtt_publish_size(mqtt_publish_packet_t* packet);
uint32_t get_mqtt_puback_size();
uint32_t get_mqtt_pubrec_size();
uint32_t get_mqtt_pubrel_size();
uint32_t get_mqtt_pubcomp_size();
uint32_t get_mqtt_subscribe_size(mqtt_un_subscribe_packet_t* packet);
uint32_t get_mqtt_unsubscribe_size(mqtt_un_subscribe_packet_t* packet);
uint32_t get_mqtt_pingreq_size();
uint32_t get_mqtt_disconnect_size();

/**
 * @brief Packs the remaining length into a given byte array.
 * 
 * The Remaining Length is the number of bytes remaining within the current packet, including data in the variable header and the payload. The Remaining Length does not include the bytes used to encode the Remaining Length.
 * The Remaining Length is encoded using a variable length encoding scheme which uses a single byte for values up to 127. Larger values are handled as follows. The least significant seven bits of each byte encode the data, and the most significant bit is used to indicate that there are following bytes in the representation. Thus each byte encodes 128 values and a "continuation bit". The maximum number of bytes in the Remaining Length field is four.
 * 
 * @param remaining_length The remaining length which is to by encoded into the bytes array
 * @param bytes A pointer to an array which needs to have up to 4 upcoming bytes
 * @return uint8_t Size of the resulting remaining length bytes
 */
uint8_t pack_mqtt_remaining_length(uint8_t* bytes, uint32_t remaining_length);

/**
 * @brief Packs an MQTT CONNECT packet into a given byte array.
 * 
 * [MQTT-3.1.0-1] After a Network Connection is established by a Client to a Server, the first Packet sent from the Client to the Server MUST be a CONNECT Packet.
 * [MQTT-3.1.0-2] A Client can only send the CONNECT Packet once over a Network Connection. The Server MUST process a second CONNECT Packet sent from a Client as a protocol violation and disconnect the Client.
 * 
 * @param packet 
 * @param bytes 
 * @return uint32_t 
 */
uint32_t pack_mqtt_connect(uint8_t* bytes, mqtt_connect_packet_t* packet);

/**
 * @brief Packs an MQTT PUBLISH packet into a given byte array.
 * 
 * A PUBLISH Control Packet is sent from a Client to a Server or from Server to a Client to transport an Application Message.
 * The Client uses a PUBLISH Packet to send an Application Message to the Server, for distribution to Clients with matching subscriptions.
 * The Server uses a PUBLISH Packet to send an Application Message to each Client which has a matching subscription.
 * When Clients make subscriptions with Topic Filters that include wildcards, it is possible for a Client’s subscriptions to overlap so that a published message might match multiple filters.
 * The action of the recipient when it receives a PUBLISH Packet depends on the QoS level.
 * 
 * [MQTT-3.3.4-1] The receiver of a PUBLISH Packet MUST respond according to Expected Publish Packet response as determined by the QoS in the PUBLISH Packet. (QoS 0 = None, QoS 1 = PUBACK, QoS 2 = PUBREC)
 * [MQTT-3.3.5-1] When Clients make subscriptions with Topic Filters that include wildcards, it is possible for a Client’s subscriptions to overlap so that a published message might match multiple filters. In this case the Server MUST deliver the message to the Client respecting the maximum QoS of all the matching subscriptions. In addition, the Server MAY deliver further copies of the message, one for each additional matching subscription and respecting the subscription’s QoS in each case.
 * [MQTT-3.3.5-2] If a Server implementation does not authorize a PUBLISH to be performed by a Client; it has no way of informing that Client. It MUST either make a positive acknowledgement, according to the normal QoS rules, or close the Network Connection.
 * 
 * @param packet 
 * @param bytes 
 * @return uint32_t 
 */
uint32_t pack_mqtt_publish(uint8_t* bytes, mqtt_publish_packet_t* packet);

/**
 * @brief Packs an MQTT PUBACK packet into a given byte array.
 * 
 * A PUBACK Packet is the response to a PUBLISH Packet with QoS level 1.
 * 
 * @param packet_identifier 
 * @param bytes 
 * @return uint32_t 
 */
uint32_t pack_mqtt_puback(uint8_t* bytes, uint16_t packet_identifier);

/**
 * @brief Packs an MQTT PUBREC packet into a given byte array.
 * 
 * A PUBREC Packet is the response to a PUBLISH Packet with QoS 2. It is the second packet of the QoS 2 protocol exchange.
 * 
 * @param packet_identifier 
 * @param bytes 
 * @return uint32_t 
 */
uint32_t pack_mqtt_pubrec(uint8_t* bytes, uint16_t packet_identifier);

/**
 * @brief Packs an MQTT PUBREL packet into a given byte array.
 * 
 * A PUBREL Packet is the response to a PUBREC Packet. It is the third packet of the QoS 2 protocol exchange.
 * 
 * @param packet_identifier 
 * @param bytes 
 * @return uint32_t 
 */
uint32_t pack_mqtt_pubrel(uint8_t* bytes, uint16_t packet_identifier);

/**
 * @brief Packs an MQTT PUBCOMP packet into a given byte array.
 * 
 * The PUBCOMP Packet is the response to a PUBREL Packet. It is the fourth and final packet of the QoS 2 protocol exchange.
 * 
 * @param packet_identifier 
 * @param bytes 
 * @return uint32_t 
 */
uint32_t pack_mqtt_pubcomp(uint8_t* bytes, uint16_t packet_identifier);

/**
 * @brief Packs an MQTT SUBSCRIBE packet into a given byte array.
 * 
 * The SUBSCRIBE Packet is sent from the Client to the Server to create one or more Subscriptions. Each Subscription registers a Client’s interest in one or more Topics. The Server sends PUBLISH Packets to the Client in order to forward Application Messages that were published to Topics that match these Subscriptions. The SUBSCRIBE Packet also specifies (for each Subscription) the maximum QoS with which the Server can send Application Messages to the Client.
 * The Server is permitted to start sending PUBLISH packets matching the Subscription before the Server sends the SUBACK Packet.
 * Where the Topic Filter is not identical to any existing Subscription’s filter, a new Subscription is created and all matching retained messages are sent.
 * 
 * [MQTT-3.8.4-1] When the Server receives a SUBSCRIBE Packet from a Client, the Server MUST respond with a SUBACK Packet.
 * [MQTT-3.8.4-2] The SUBACK Packet MUST have the same Packet Identifier as the SUBSCRIBE Packet that it is acknowledging.
 * [MQTT-3.8.4-3] If a Server receives a SUBSCRIBE Packet containing a Topic Filter that is identical to an existing Subscription’s Topic Filter then it MUST completely replace that existing Subscription with a new Subscription. The Topic Filter in the new Subscription will be identical to that in the previous Subscription, although its maximum QoS value could be different. Any existing retained messages matching the Topic Filter MUST be re-sent, but the flow of publications MUST NOT be interrupted.
 * [MQTT-3.8.4-4] If a Server receives a SUBSCRIBE packet that contains multiple Topic Filters it MUST handle that packet as if it had received a sequence of multiple SUBSCRIBE packets, except that it combines their responses into a single SUBACK response.
 * [MQTT-3.8.4-5] The SUBACK Packet sent by the Server to the Client MUST contain a return code for each Topic Filter/QoS pair. This return code MUST either show the maximum QoS that was granted for that Subscription or indicate that the subscription failed.
 * [MQTT-3.8.4-6] The Server might grant a lower maximum QoS than the subscriber requested. The QoS of Payload Messages sent in response to a Subscription MUST be the minimum of the QoS of the originally published message and the maximum QoS granted by the Server. The server is permitted to send duplicate copies of a message to a subscriber in the case where the original message was published with QoS 1 and the maximum QoS granted was QoS 0.
 * 
 * @param packet 
 * @param bytes 
 */
uint32_t pack_mqtt_subscribe(uint8_t* bytes, mqtt_un_subscribe_packet_t* packet);

/**
 * @brief Packs an MQTT UNSUBSCRIBE packet into a given byte array.
 * 
 * The payload for the UNSUBSCRIBE Packet contains the list of Topic Filters that the Client wishes to unsubscribe from.
 * If a Server deletes a Subscription, it MAY continue to deliver any existing messages buffered for delivery to the Client.
 * 
 * [MQTT-3.10.3-1] The Topic Filters in an UNSUBSCRIBE packet MUST be UTF-8 encoded strings as defined in Section 1.5.3, packed contiguously.
 * [MQTT-3.10.3-2] The Payload of an UNSUBSCRIBE packet MUST contain at least one Topic Filter. An UNSUBSCRIBE packet with no payload is a protocol violation.
 * [MQTT-3.10.4-1] The Topic Filters (whether they contain wildcards or not) supplied in an UNSUBSCRIBE packet MUST be compared character-by-character with the current set of Topic Filters held by the Server for the Client. If any filter matches exactly then its owning Subscription is deleted, otherwise no additional processing occurs.
 * [MQTT-3.10.4-2] If a Server deletes a Subscription, it MUST stop adding any new messages for delivery to the Client.
 * [MQTT-3.10.4-3] If a Server deletes a Subscription, it MUST complete the delivery of any QoS 1 or QoS 2 messages which it has started to send to the Client.
 * [MQTT-3.10.4-4] The Server MUST respond to an UNSUBSUBCRIBE request by sending an UNSUBACK packet. The UNSUBACK Packet MUST have the same Packet Identifier as the UNSUBSCRIBE Packet.
 * [MQTT-3.10.4-5] Even where no Topic Subscriptions are deleted, the Server MUST respond with an UNSUBACK.
 * [MQTT-3.10.4-6] If a Server receives an UNSUBSCRIBE packet that contains multiple Topic Filters it MUST handle that packet as if it had received a sequence of multiple UNSUBSCRIBE packets, except that it sends just one UNSUBACK response.
 * 
 * @return uint32_t 
 */
uint32_t pack_mqtt_unsubscribe(uint8_t* bytes, mqtt_un_subscribe_packet_t* packet);

/**
 * @brief Packs an MQTT PINGREQ packet into a given byte array.
 * 
 * The PINGREQ Packet is sent from a Client to the Server. It can be used to:
 *  - Indicate to the Server that the Client is alive in the absence of any other Control Packets being sent from the Client to the Server.
 *  - Request that the Server responds to confirm that it is alive.
 *  - Exercise the network to indicate that the Network Connection is active.
 *
 * @return uint32_t 
 */
uint32_t pack_mqtt_pingreq(uint8_t* bytes);

/**
 * @brief Packs an MQTT DISCONNECT packet into a given byte array.
 * 
 * The DISCONNECT Packet is the final Control Packet sent from the Client to the Server. 
 * It indicates that the Client is disconnecting cleanly.
 * 
 * @return uint32_t 
 */
uint32_t pack_mqtt_disconnect(uint8_t* bytes);

/**
 * @brief Unpacks MQTT packets out of a stream of bytes which arrives in chunks.
 * 
 * This function gets the mqtt_packet struct which contains a byte buffer which needs to be large
 * enough so that it can hold all the bytes of the MQTT packet currently decoded.
 * 
 * @param bytes A pointer to a struct holding the information regarding an MQTT packet.
 * @param size The current number of bytes in the MQTT packet.
 * @param packet The struct which will hold the unpacked information about the MQTT packet.ss
 */
void unpack_mqtt_packet(const uint8_t* bytes, uint32_t size, mqtt_packet_t* packet);
void unpack_mqtt_packet_identifier(const uint8_t* bytes, mqtt_packet_t* packet, uint16_t* packet_identifier);
void unpack_mqtt_publish(const uint8_t* bytes, mqtt_packet_t* packet, mqtt_publish_packet_t* publish_packet);
void unpack_mqtt_connack(const uint8_t* bytes, mqtt_packet_t* packet, mqtt_connack_packet_t* connack_packet);
void unpack_mqtt_suback(const uint8_t* bytes, mqtt_packet_t* packet, mqtt_suback_packet_t* suback_packet);

#endif