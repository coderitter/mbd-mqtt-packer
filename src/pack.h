#ifndef MQTT_PACKER_PACK_H_
#define MQTT_PACKER_PACK_H_

#include <stdint.h>
#include "defines.h"

uint8_t getMqttRemainingLengthSize(uint32_t remainingLength);
uint32_t getMqttConnectSize(struct MqttConnectPacket *packet);
uint32_t getMqttPublishSize(struct MqttPublishPacket *packet);
uint32_t getMqttPubAckSize();
uint32_t getMqttPubRecSize();
uint32_t getMqttPubRelSize();
uint32_t getMqttPubCompSize();
uint32_t getMqttSubscribeSize(struct MqttUnSubscribePacket *packet);
uint32_t getMqttUnsubscribeSize(struct MqttUnSubscribePacket *packet);
uint32_t getMqttPingReqSize();
uint32_t getMqttDisconnectSize();

/**
 * @brief Packs the remaining length into a given byte array.
 * 
 * The Remaining Length is the number of bytes remaining within the current packet, including data in the variable header and the payload. The Remaining Length does not include the bytes used to encode the Remaining Length.
 * The Remaining Length is encoded using a variable length encoding scheme which uses a single byte for values up to 127. Larger values are handled as follows. The least significant seven bits of each byte encode the data, and the most significant bit is used to indicate that there are following bytes in the representation. Thus each byte encodes 128 values and a "continuation bit". The maximum number of bytes in the Remaining Length field is four.
 * 
 * @param remainingLength The remaining length which is to by encoded into the bytes array
 * @param bytes A pointer to an array which needs to have up to 4 upcoming bytes
 * @return uint8_t Size of the resulting remaining length bytes
 */
uint8_t packMqttRemainingLength(uint32_t remainingLength, uint8_t *bytes);

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
uint32_t packMqttConnect(struct MqttConnectPacket *packet, uint8_t *bytes);

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
uint32_t packMqttPublish(struct MqttPublishPacket *packet, uint8_t *bytes);

/**
 * @brief Packs an MQTT PUBACK packet into a given byte array.
 * 
 * A PUBACK Packet is the response to a PUBLISH Packet with QoS level 1.
 * 
 * @param packetIdentifier 
 * @param bytes 
 * @return uint32_t 
 */
uint32_t packMqttPubAck(uint16_t packetIdentifier, uint8_t *bytes);

/**
 * @brief Packs an MQTT PUBREC packet into a given byte array.
 * 
 * A PUBREC Packet is the response to a PUBLISH Packet with QoS 2. It is the second packet of the QoS 2 protocol exchange.
 * 
 * @param packetIdentifier 
 * @param bytes 
 * @return uint32_t 
 */
uint32_t packMqttPubRec(uint16_t packetIdentifier, uint8_t *bytes);

/**
 * @brief Packs an MQTT PUBREL packet into a given byte array.
 * 
 * A PUBREL Packet is the response to a PUBREC Packet. It is the third packet of the QoS 2 protocol exchange.
 * 
 * @param packetIdentifier 
 * @param bytes 
 * @return uint32_t 
 */
uint32_t packMqttPubRel(uint16_t packetIdentifier, uint8_t *bytes);

/**
 * @brief Packs an MQTT PUBCOMP packet into a given byte array.
 * 
 * The PUBCOMP Packet is the response to a PUBREL Packet. It is the fourth and final packet of the QoS 2 protocol exchange.
 * 
 * @param packetIdentifier 
 * @param bytes 
 * @return uint32_t 
 */
uint32_t packMqttPubComp(uint16_t packetIdentifier, uint8_t *bytes);

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
uint32_t packMqttSubscribe(struct MqttUnSubscribePacket *packet, uint8_t *bytes);

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
uint32_t packMqttUnsubscribe(struct MqttUnSubscribePacket *packet, uint8_t *bytes);

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
uint32_t packMqttPingReq(uint8_t *bytes);

/**
 * @brief Packs an MQTT DISCONNECT packet into a given byte array.
 * 
 * The DISCONNECT Packet is the final Control Packet sent from the Client to the Server. 
 * It indicates that the Client is disconnecting cleanly.
 * 
 * @return uint32_t 
 */
uint32_t packMqttDisconnect(uint8_t *bytes);

#endif