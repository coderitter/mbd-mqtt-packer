#include <stdio.h>
#include "decode.test.c"
#include "encode.test.c"
#include "getSize.test.c"

int main()
{
    int16_t failedAssertions = 0;

    printf("\n");
    printf("getMqttRemainingLengthSize\n");
    printf("--------------------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectRemainingLengthSizeForA1ByteLength();
    failedAssertions += itShouldGetTheCorrectRemainingLengthSizeForA2ByteLength();
    failedAssertions += itShouldGetTheCorrectRemainingLengthSizeForA3ByteLength();
    failedAssertions += itShouldGetTheCorrectRemainingLengthSizeForA4ByteLength();

    printf("\n");
    printf("getMqttConnectSize\n");
    printf("------------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectConnectSize();
    failedAssertions += itShouldGetTheCorrectConnectSizeWithClientIdentifierSizeOf0();
    
    printf("\n");
    printf("getMqttPublishSize\n");
    printf("------------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectPublishSize();

    printf("\n");
    printf("getMqttPubAckSize\n");
    printf("-----------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectPubAckSize();

    printf("\n");
    printf("getMqttPubRecSize\n");
    printf("-----------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectPubRecSize();

    printf("\n");
    printf("getMqttPubRelSize\n");
    printf("-----------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectPubRelSize();

    printf("\n");
    printf("getMqttPubCompSize\n");
    printf("------------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectPubCompSize();

    printf("\n");
    printf("getMqttSubscribeSize\n");
    printf("--------------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectSubscribeSize();

    printf("\n");
    printf("getMqttUnsubscribeSize\n");
    printf("----------------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectUnsubscribeSize();

    printf("\n");
    printf("getPingReqSize\n");
    printf("--------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectPingReqSize();

    printf("\n");
    printf("getDisconnectSize\n");
    printf("-----------------\n");
    printf("\n");

    failedAssertions += itShouldEncodeDisconnect();

    printf("\n");
    printf("encodeMqttRemainingLength\n");
    printf("-------------------------\n");
    printf("\n");

    failedAssertions += itShouldEncodeTheRemainingLengthWith1Byte();
    failedAssertions += itShouldEncodeTheRemainingLengthWith2Bytes();
    failedAssertions += itShouldEncodeTheRemainingLengthWith3Bytes();
    failedAssertions += itShouldEncodeTheRemainingLengthWith4Bytes();

    printf("\n");
    printf("encodeMqttConnect\n");
    printf("-----------------\n");
    printf("\n");

    failedAssertions += itShouldEncodeConnect();
    failedAssertions += itShouldEncodeConnectWithClientIdentifierSizeOf0();

    printf("\n");
    printf("encodeMqttPublish\n");
    printf("-----------------\n");
    printf("\n");

    failedAssertions += itShouldEncodePublish();

    printf("\n");
    printf("encodeMqttPubAck\n");
    printf("----------------\n");
    printf("\n");

    failedAssertions += itShouldEncodePubAck();

    printf("\n");
    printf("encodeMqttPubRec\n");
    printf("----------------\n");
    printf("\n");

    failedAssertions += itShouldEncodePubRec();

    printf("\n");
    printf("encodeMqttPubRel\n");
    printf("----------------\n");
    printf("\n");

    failedAssertions += itShouldEncodePubRel();

    printf("\n");
    printf("encodeMqttPubComp\n");
    printf("---------------\n");
    printf("\n");

    failedAssertions += itShouldEncodePubComp();

    printf("\n");
    printf("encodeMqttSubscribe\n");
    printf("-------------------\n");
    printf("\n");

    failedAssertions += itShouldEncodeSubscribe();

    printf("\n");
    printf("encodeMqttUnsubscribe\n");
    printf("---------------------\n");
    printf("\n");

    failedAssertions += itShouldEncodeUnsubscribe();

    printf("\n");
    printf("encodePingReq\n");
    printf("-------------\n");
    printf("\n");

    failedAssertions += itShouldEncodePingReq();

    printf("\n");
    printf("encodeDisconnect\n");
    printf("----------------\n");
    printf("\n");

    failedAssertions += itShouldEncodeDisconnect();

    printf("\n");
    printf("decodeMqttChunk\n");
    printf("---------------\n");
    printf("\n");

    failedAssertions += itShouldDecodeAMessageWithA2ByteHeaderAndAZeroLengthRemainingSize();
    failedAssertions += itShouldDecodeAMessageWithA2ByteHeader();
    failedAssertions += itShouldDecodeAMessageWithA3ByteHeader();
    failedAssertions += itShouldDecodeAMessageWithA4ByteHeader();
    failedAssertions += itShouldDecodeAMessageWithA5ByteHeader();
    failedAssertions += itShouldDecodeAMessageWithA5ByteHeaderGivenThrough1ByteChunks();
    failedAssertions += itShouldMoveTheBytesOfANewMessageToTheBeginning();

    printf("\n");
    printf("decodeMqttPacketIdentifier\n");
    printf("--------------------------\n");
    printf("\n");

    failedAssertions += itShouldDecodeThePacketIdentifierWhichIsAfterTheFixedHeader();
    failedAssertions += itShouldDecodeThePacketIdentifierOfPublish();

    printf("\n");
    printf("decodeMqttPublishTopicName\n");
    printf("--------------------------\n");
    printf("\n");

    failedAssertions += itShouldDecodeThePublishTopicName();

    printf("\n");
    printf("decodeMqttPublishPayload\n");
    printf("------------------------\n");
    printf("\n");

    failedAssertions += itShouldDecodeThePublishPayload();

    printf("\n");
    printf("Total failed assertions: %i\n", failedAssertions);
    printf("\n");
}