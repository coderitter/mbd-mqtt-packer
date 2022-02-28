#ifndef MQTT_PACKER_UNPACK_H_
#define MQTT_PACKER_UNPACK_H_

#include <stdint.h>
#include "defines.h"

/**
 * @brief Decodes MQTT messages out of a stream of bytes which arrives in chunks.
 * 
 * This function gets the MqttPacket struct which contains a byte buffer which needs to be large
 * enough so that it can hold all the bytes of the MQTT packet currently decoded.
 * 
 * @param mqttMessage A pointer to a struct holding the information regarding an MQTT packet.
 * @param currentSize The current number of bytes in the MQTT packet.
 * @param chunkSize The size of the last added chunk.
 * @param onMqttPacketComplete A pointer to a function which is called as soon as an MQTT packet is complete.
 * @return int32_t The size of the MQTT packet which is currently decoded.
 */
int32_t unpackMqttChunk
(
    struct MqttPacket *mqttMessage, 
    int32_t *currentSize, 
    int32_t chunkSize,
    void (*onMqttPacketComplete) (struct MqttPacket *mqttMessage)
);

void unpackMqttPacketIdentifier(struct MqttPacket *packet, uint16_t *packetIdentifier);
void unpackMqttPublish(struct MqttPacket *packet, struct MqttPublishPacket *publishPacket);
void unpackMqttConnAck(struct MqttPacket *packet, struct MqttConnAckPacket *connAckPacket);
void unpackMqttSubAck(struct MqttPacket *packet, struct MqttSubAckPacket *subAckPacket);

#endif 